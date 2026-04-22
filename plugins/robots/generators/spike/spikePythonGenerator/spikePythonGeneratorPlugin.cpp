/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "spikePythonGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDirIterator>
#include <QtCore/QProcess>

#include <qrutils/widgets/qRealMessageBox.h>
#include <qrkernel/logging.h>
#include <spikeKit/communication/spikeRobotCommunicationThread.h>
#include <spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/platformInfo.h>
#include "spikePythonMasterGenerator.h"
#include <QsLog.h>

using namespace spike::python;
using namespace qReal;

SpikePythonGeneratorPlugin::SpikePythonGeneratorPlugin()
	: SpikeGeneratorPluginBase("SpikePythonUsbGeneratorRobotModel", tr("Autonomous mode (USB)"), 9
			, "SpikePythonBluetoothGeneratorRobotModel", tr("Autonomous mode (Bluetooth)"), 8)
	, mGenerateCodeAction(new QAction(this))
	, mUploadProgramAction(new QAction(this))
	, mRunProgramAction(new QAction(this))
	, mStopRobotAction(new QAction(this))
{
	mGenerateCodeAction->setText(tr("Generate to Spike Robot Byte Code File"));
	mGenerateCodeAction->setIcon(QIcon(":/spike/python/images/generatePythonCode.svg"));
	connect(mGenerateCodeAction, &QAction::triggered, this, &SpikePythonGeneratorPlugin::generateCode);

	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/spike/python/images/uploadProgram.svg"));
	connect(mUploadProgramAction
			, &QAction::triggered
			, this
			, [this](){
		auto rp = static_cast<RunPolicy>(SettingsManager::value("spikeRunPolicy").toInt());
		uploadAndRunProgram(rp);
	});

	mRunProgramAction->setObjectName("runSpikePythonProgram");
	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/spike/python/images/run.png"));
	connect(mRunProgramAction, &QAction::triggered
			, this
			, [this]{ uploadAndRunProgram(RunPolicy::AlwaysRun); }
	);

	mStopRobotAction->setObjectName("stopSpikePythonRobot");
	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/spike/python/images/stop.png"));
	connect(mStopRobotAction, &QAction::triggered, this, &SpikePythonGeneratorPlugin::stopRobot, Qt::UniqueConnection);

	text::Languages::registerLanguage(text::LanguageInfo{ "lms"
			, tr("SPIKE Source Code language")
			, true
			, 4
			, 2
			, "//"
			, QString()
			, "/*"
			, "*/"
			, nullptr
			, {}
	});
}

QList<ActionInfo> SpikePythonGeneratorPlugin::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	const ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	const ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> SpikePythonGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::Key_F5));
	mStopRobotAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateActionInfo("Generator.GenerateSpikePython"
			, tr("Generate Spike Robot Byte Code File"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadSpike", tr("Upload SPIKE Program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunSpike", tr("Run SPIKE Program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopSpike", tr("Stop SPIKE Program"), mStopRobotAction);

	return { generateActionInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo };
}

QIcon SpikePythonGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/spike/python/images/switch-to-spike-python.svg");
}

int SpikePythonGeneratorPlugin::priority() const
{
	return 9;
}

QString SpikePythonGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("spike-python/%1/%1.lms").arg(projectName);
}

text::LanguageInfo SpikePythonGeneratorPlugin::language() const
{
	return text::Languages::pickByExtension("lms");
}

QString SpikePythonGeneratorPlugin::generatorName() const
{
	return "spike/python";
}

QString SpikePythonGeneratorPlugin::uploadProgram()
{
	QFileInfo const fileInfo = generateCodeForProcessing();
	if (!fileInfo.exists()) {
		return {};
	}

	if (!copySystemFiles(fileInfo.absolutePath())) {
		mMainWindowInterface->errorReporter()->addError(tr("Can't write source code files to disk!"));
		return {};
	}

	if (!compile(fileInfo)) {
		QLOG_ERROR() << "SPIKE bytecode compillation process failed!";
		mMainWindowInterface->errorReporter()->addError(tr("Compilation error occured."));
		return {};
	}

	return upload(fileInfo);
}

void SpikePythonGeneratorPlugin::uploadAndRunProgram(RunPolicy runPolicy)
{
	const QString fileOnRobot = uploadProgram();
	const auto &communicator = currentCommunicator();

	if (fileOnRobot.isEmpty() || !communicator) {
		return;
	}

	switch (runPolicy) {
		case RunPolicy::Ask:
			if (utils::QRealMessageBox::question(mMainWindowInterface->windowWidget()
												, tr("The program has been uploaded")
												, tr("Do you want to run it?")
			) != QMessageBox::Yes) {
				return;
			}
			Q_FALLTHROUGH();
		case RunPolicy::AlwaysRun:
			QMetaObject::invokeMethod(communicator.get(), [=](){ communicator->runProgram(fileOnRobot); });
			Q_FALLTHROUGH();
		case RunPolicy::NeverRun:
			return;
	}
}

void SpikePythonGeneratorPlugin::stopRobot()
{
	if (const auto &communicator = currentCommunicator()) {
		QMetaObject::invokeMethod(communicator.get(), &communication::SpikeRobotCommunicationThread::stopProgram);
	}
}

bool SpikePythonGeneratorPlugin::copySystemFiles(const QString &destination)
{
	QDirIterator iterator(":/spike/python/thirdparty");
	while (iterator.hasNext()) {
		const QFileInfo fileInfo(iterator.next());
		const QString destFile = destination + "/" + fileInfo.fileName();
		if (!QFile::exists(destFile) && !QFile::copy(fileInfo.absoluteFilePath(), destFile)) {
			return false;
		}
	}

	return true;
}

QString SpikePythonGeneratorPlugin::getLmsasmExecutable() const
{
	const QDir dir(PlatformInfo::invariantSettingsPath("pathToSpikeTools"));

	if (!dir.exists()) {
		QLOG_ERROR() << "Directory for spike tools with name" << dir << "does not exist";
		return {};
	}

	const auto &toolName = PlatformInfo::osType() == "windows" ? "lmsasm.exe": "lmsasm";
	const auto &toolPath = dir.filePath(toolName);

	QFileInfo toolFile(toolPath);
	if (!toolFile.exists() || !toolFile.isFile()) {
		QLOG_ERROR() << "Tool not found at path:" << toolPath;
		return {};
	}
	return toolPath;
}

bool SpikePythonGeneratorPlugin::compile(const QFileInfo &lmsFile)
{
	QFile pythonFile(lmsFile.absolutePath() + "/" + lmsFile.baseName() + ".python");
	if (pythonFile.exists()) {
		pythonFile.remove();
	}

	const auto &lmsasmExecutable = getLmsasmExecutable();
	QProcess lmsasm;
	QEventLoop loop;
	lmsasm.setEnvironment(QProcess::systemEnvironment());
	lmsasm.setWorkingDirectory(lmsFile.absolutePath());
	lmsasm.setProgram(lmsasmExecutable);
#ifdef SPIKE_COMPILE_DEBUG_MODE
	lmsasm.setArguments({"-debug", "-output", pythonFile.fileName(), lmsFile.fileName()});
#else
	lmsasm.setArguments({"-output", pythonFile.fileName(), lmsFile.fileName()});
#endif
	connect(&lmsasm, &QProcess::readyRead, &loop, [&lmsasm]() { QLOG_INFO() << lmsasm.readAll(); });
	connect(&lmsasm, &QProcess::errorOccurred, &loop, [&lmsasm, &loop](QProcess::ProcessError e) {
		QLOG_ERROR() << "Failed to start process (status" << e << "):"
					 << lmsasm.program() << lmsasm.arguments();
		loop.quit();
	});
	connect(&lmsasm, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished)
		, &loop, [&loop, &lmsasm](int e, QProcess::ExitStatus s) {
		if (e || s != QProcess::ExitStatus::NormalExit) {
			QLOG_ERROR() << "Failed to execute process (errCode:" << e << ", exitStatus:" << s << "):"
						 << lmsasm.program() << lmsasm.arguments();
		}
		loop.quit();
	});

	lmsasm.start();
	loop.exec();
	return true;
}

QString SpikePythonGeneratorPlugin::upload(const QFileInfo &lmsFile)
{
	const QString folderName = SettingsManager::value("SpikeCommonFolderChecboxChecked", false).toBool()
			? SettingsManager::value("SpikeCommonFolderName", "ts").toString() : lmsFile.baseName();
	const QString targetPath = "../prjs/" + folderName;
	const QString pythonPath = lmsFile.absolutePath() + "/" + lmsFile.baseName() + ".python";
	bool connected = false;
	const auto &communicator = currentCommunicator();
	if (!communicator) {
		return {};
	}
	auto errorReporter = connect(
			communicator.get(), &utils::robotCommunication::RobotCommunicationThreadInterface::errorOccured,
			this, [this](const QString &message){
				mMainWindowInterface->errorReporter()->addError(message);
			});
	auto blockingConnectionType = communicator->thread() == QThread::currentThread()
			? Qt::DirectConnection : Qt::BlockingQueuedConnection;
	QMetaObject::invokeMethod(communicator.get(), &communication::SpikeRobotCommunicationThread::connect
			, blockingConnectionType
			, &connected);
	if (!connected) {
		const bool isUsb = mRobotModelManager->model().name().contains("usb", Qt::CaseInsensitive);
		mMainWindowInterface->errorReporter()->addError(tr("Could not upload file to robot. "\
				"Connect to a robot via %1.").arg(isUsb ? tr("USB") : tr("Bluetooth")));
		return {};
	}

	QString res;
	QMetaObject::invokeMethod(communicator.get(), [=](){ return communicator->uploadFile(pythonPath, targetPath); }
			, blockingConnectionType
			, &res);
	disconnect(errorReporter);
	return res;
}

generatorBase::MasterGeneratorBase *SpikePythonGeneratorPlugin::masterGenerator()
{
	return new SpikePythonMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}
