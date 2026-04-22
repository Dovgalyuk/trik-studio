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

#include "spikePythonGeneratorLibrary/spikePythonGeneratorPluginBase.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QStateMachine>
#include <QtCore/QState>
#include <QtCore/QFinalState>

#include <qrkernel/logging.h>
#include <spikeGeneratorBase/spikeGeneratorPluginBase.h>
#include <spikeGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <spikeKit/robotModel/spikeRobotModel.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>
#include <utils/robotCommunication/runProgramProtocol.h>
#include <utils/robotCommunication/stopRobotProtocol.h>
#include <utils/robotCommunication/uploadProgramProtocol.h>
#include <utils/robotCommunication/networkCommunicationErrorReporter.h>
#include <qrgui/textEditor/qscintillaTextEdit.h>
#include <qrgui/textEditor/textManagerInterface.h>

#include "spikePythonMasterGenerator.h"
#include "emptyShell.h"

using namespace spike::python;
using namespace kitBase::robotModel;
using namespace qReal;
using namespace utils::robotCommunication;

SpikePythonGeneratorPluginBase::SpikePythonGeneratorPluginBase(
		spike::robotModel::SpikeRobotModel * const robotModel
		, const QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> &blocksFactory
		, const QStringList &pathsToTemplates)
	: SpikeGeneratorPluginBase(robotModel, blocksFactory)
	, mGenerateCodeAction(new QAction(this))
	, mUploadProgramAction(new QAction(this))
	, mRunProgramAction(new QAction(this))
	, mStopRobotAction(new QAction(this))
	, mRobotModel(*robotModel)
	, mPathsToTemplates(pathsToTemplates)
{
}

SpikePythonGeneratorPluginBase::~SpikePythonGeneratorPluginBase()
{
}

void SpikePythonGeneratorPluginBase::init(const kitBase::KitPluginConfigurator &configurer)
{
	const auto errorReporter = configurer.qRealConfigurator().mainWindowInterpretersInterface().errorReporter();
	RobotsGeneratorPluginBase::init(configurer);
	mCommunicator.reset(new TcpRobotCommunicator("SpikeTcpServer"));
	NetworkCommunicationErrorReporter::connectErrorReporter(*mCommunicator, *errorReporter);

	mUploadProgramProtocol.reset(new UploadProgramProtocol(*mCommunicator));
	//mRunProgramProtocol.reset(new RunProgramProtocol(*mCommunicator, mRobotModel.robotConfigFileVersion()));
	mStopRobotProtocol.reset(new StopRobotProtocol(*mCommunicator));

	const auto timeout = [this, errorReporter]() {
		errorReporter->addError(tr("Network operation timed out"));
		onProtocolFinished();
	};

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::timeout, this, timeout);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::timeout, this, timeout);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::timeout, this, timeout);

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::error
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::error
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::error
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::success
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::success
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::success
			, this, &SpikePythonGeneratorPluginBase::onProtocolFinished);

	connect(mRunProgramProtocol.data(), &RunProgramProtocol::configVersionMismatch
			, this, [errorReporter](const QString &expected, const QString &actual) {
				Q_UNUSED(expected)
				Q_UNUSED(actual)
				errorReporter->addError(
						QString(tr("Casing model mismatch, check SPIKE Studio settings, \"Robots\" page. It seems that "
								"SPIKE casing version selected in SPIKE Studio differs from version on robot.")));
			}
	);
}

QList<ActionInfo> SpikePythonGeneratorPluginBase::customActions()
{
	mGenerateCodeAction->setObjectName("generatePythonSpikeCode");
	mGenerateCodeAction->setText(tr("Generate python code"));
	mGenerateCodeAction->setIcon(QIcon(":/spike/python/images/generatePythonCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, &QAction::triggered, this
			, &SpikePythonGeneratorPluginBase::generateCode, Qt::UniqueConnection);

	mUploadProgramAction->setObjectName("uploadPythonSpikeProgram");
	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/spike/python/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, &QAction::triggered, this
			, &SpikePythonGeneratorPluginBase::uploadProgram, Qt::UniqueConnection);

	mRunProgramAction->setObjectName("runPythonSpikeProgram");
	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/spike/python/images/run.png"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	connect(mRunProgramAction, &QAction::triggered, this
			, &SpikePythonGeneratorPluginBase::runProgram, Qt::UniqueConnection);

	mStopRobotAction->setObjectName("stopPythonSpikeRobot");
	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/spike/python/images/stop.png"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	connect(mStopRobotAction, &QAction::triggered, this
			, &SpikePythonGeneratorPluginBase::stopRobot, Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> SpikePythonGeneratorPluginBase::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::Key_F5));
	mStopRobotAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateSpike", tr("Generate Python code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadSpike", tr("Upload Python program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunSpike", tr("Run Python program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopSpike", tr("Stop Python program"), mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

QIcon SpikePythonGeneratorPluginBase::iconForFastSelector(const RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)

	return QIcon(":/spike/python/images/switch-to-spike-python.svg");
}

generatorBase::MasterGeneratorBase *SpikePythonGeneratorPluginBase::masterGenerator()
{
	return new SpikePythonMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, mPathsToTemplates);
}

QString SpikePythonGeneratorPluginBase::defaultFilePath(const QString &projectName) const
{
	return QString("spike/%1/%1.py").arg(projectName);
}

text::LanguageInfo SpikePythonGeneratorPluginBase::language() const
{
	return qReal::text::Languages::python({"brick"});
}

QString SpikePythonGeneratorPluginBase::generatorName() const
{
	return "spikePython";
}

void SpikePythonGeneratorPluginBase::addShellDevice(robotModel::GeneratorModelExtensionInterface &robotModel) const
{
	const PortInfo shellPort("ShellPort", output);
	EmptyShell * const shell = new EmptyShell(DeviceInfo::create<spike::robotModel::parts::SpikeShell>(), shellPort);
	connect(mCommunicator.data(), &TcpRobotCommunicator::printText, shell, &EmptyShell::print);
	robotModel.addDevice(shellPort, shell);
}

void SpikePythonGeneratorPluginBase::uploadProgram()
{
	if (mUploadProgramProtocol) {
		if (mMainWindowInterface->activeDiagram() != Id())
		{
			const QFileInfo fileInfo = generateCodeForProcessing();
            return;
			if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
				disableButtons();
				mUploadProgramProtocol->run({fileInfo});
			}
		} else {
			QList<QFileInfo> files;
			auto const &tabs = mMainWindowInterface->allTabs();
			for (auto &&tab : tabs) {
				if (auto * code = dynamic_cast<qReal::text::QScintillaTextEdit *>(tab)) {
					auto const &ext = code->currentLanguage().extension;
					if (ext == "js" || ext == "py") {
						files << QFileInfo(mTextManager->path(code));
					}
				}
			}
			if (!files.isEmpty()) {
				disableButtons();
				mUploadProgramProtocol->run(files);
			} else {
				mMainWindowInterface->errorReporter()->addError(
						tr("There are no files to upload. You must open or generate at least one *.js or *.py file."));
			}
		}
	} else {
		QLOG_ERROR() << "Upload program protocol is not initialized";
	}
}

void SpikePythonGeneratorPluginBase::runProgram()
{
	const QFileInfo fileInfo = generateCodeForProcessing();
    return;
	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		if (mRunProgramProtocol) {
			disableButtons();
			mRunProgramProtocol->run(fileInfo);
		} else {
			QLOG_ERROR() << "Run program protocol is not initialized";
		}
	} else {
		QLOG_ERROR() << "Code generation failed, aborting";
	}
}

void SpikePythonGeneratorPluginBase::stopRobot()
{
	if (mStopRobotProtocol) {
		disableButtons();
		mStopRobotProtocol->run(
				"script.system(\"killall python\"); \n"
				"script.system(\"killall aplay\"); \n"
				"script.system(\"killall vlc\");"
				);
	} else {
		QLOG_ERROR() << "Stop robot protocol is not initialized";
	}
}

void SpikePythonGeneratorPluginBase::onProtocolFinished()
{
	mUploadProgramAction->setEnabled(true);
	mRunProgramAction->setEnabled(true);
	mStopRobotAction->setEnabled(true);
}

void SpikePythonGeneratorPluginBase::disableButtons()
{
	mUploadProgramAction->setEnabled(false);
	mRunProgramAction->setEnabled(false);
	mStopRobotAction->setEnabled(false);
}
