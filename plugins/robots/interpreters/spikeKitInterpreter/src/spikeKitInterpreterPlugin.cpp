/* Copyright 2014-2016 CyberTech Labs Ltd.
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

#include "spikeKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <twoDModel/engine/twoDModelEngineFacade.h>
#include <utils/widgets/comPortPicker.h>

using namespace spike;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

SpikeKitInterpreterPlugin::SpikeKitInterpreterPlugin()
	: mUsbRealRobotModel(kitId(), "spikeKitUsbRobot") // todo: somewhere generate robotId for each robot
	, mBluetoothRealRobotModel(kitId(), "spikeKitBluetoothRobot")
	, mTwoDRobotModel(mUsbRealRobotModel)
	, mBlocksFactory(new blocks::SpikeBlocksFactory({"spikeKitUsbRobot", "spikeKitBluetoothRobot"}))
{
	mAdditionalPreferences = new SpikeAdditionalPreferences;

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &SpikeAdditionalPreferences::settingsChanged
			, &mUsbRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &SpikeAdditionalPreferences::settingsChanged
			, &mBluetoothRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &SpikeAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

SpikeKitInterpreterPlugin::~SpikeKitInterpreterPlugin()
{
	release();
}

void SpikeKitInterpreterPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	connect(&configurator.eventsForKitPlugin(), &kitBase::EventsForKitPluginInterface::robotModelChanged
			, this, [this](const QString &modelName)
	{
		mCurrentlySelectedModelName = modelName;
	});

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurator.qRealConfigurator().mainWindowInterpretersInterface();

	connect(&mUsbRealRobotModel, &robotModel::real::RealRobotModel::errorOccured
			, this, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addError(message);
	});
	connect(&mUsbRealRobotModel, &robotModel::real::RealRobotModel::messageArrived
			, this, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addInformation(message);
	});
	connect(&mBluetoothRealRobotModel, &robotModel::real::RealRobotModel::errorOccured
			, this, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addError(message);
	});
	connect(&mBluetoothRealRobotModel, &robotModel::real::RealRobotModel::messageArrived
			, this, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addInformation(message);
	});

	mTwoDModel->init(configurator.eventsForKitPlugin()
			, configurator.qRealConfigurator().systemEvents()
			, configurator.qRealConfigurator().logicalModelApi()
			, configurator.qRealConfigurator().controller()
			, interpretersInterface
			, configurator.qRealConfigurator().mainWindowDockInterface()
			, configurator.qRealConfigurator().projectManager()
			, configurator.interpreterControl());
}

void SpikeKitInterpreterPlugin::release()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
		mAdditionalPreferences = nullptr;
	}
	mTwoDModel.reset();
}

QString SpikeKitInterpreterPlugin::kitId() const
{
	return "spikeKit";
}

QString SpikeKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Lego SPIKE");
}

QList<kitBase::robotModel::RobotModelInterface *> SpikeKitInterpreterPlugin::robotModels()
{
	return {&mUsbRealRobotModel, &mBluetoothRealRobotModel, &mTwoDRobotModel};
}

QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> SpikeKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	if (robotModels().contains(const_cast<kitBase::robotModel::RobotModelInterface *>(model))) {
		return mBlocksFactory;
	} else {
		return nullptr;
	}
}

kitBase::robotModel::RobotModelInterface *SpikeKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

QList<kitBase::AdditionalPreferences *> SpikeKitInterpreterPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return { mAdditionalPreferences };
}

QWidget *SpikeKitInterpreterPlugin::quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model)
{
	return model.name().toLower().contains("bluetooth")
			? produceBluetoothPortConfigurer()
			: nullptr;
}

QList<qReal::ActionInfo> SpikeKitInterpreterPlugin::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> SpikeKitInterpreterPlugin::hotKeyActions()
{
	return {};
}

QIcon SpikeKitInterpreterPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	return &robotModel == &mUsbRealRobotModel
			? QIcon(":/spike/interpreter/images/switch-real-spike-usb.svg")
			: &robotModel == &mBluetoothRealRobotModel
					? QIcon(":/spike/interpreter/images/switch-real-spike-bluetooth.svg")
					: QIcon(":/spike/interpreter/images/switch-2d.svg");
}

kitBase::DevicesConfigurationProvider *SpikeKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

QString SpikeKitInterpreterPlugin::defaultSettingsFile() const
{
	return ":/spike/interpreter/spikeDefaultSettings.ini";
}

QWidget *SpikeKitInterpreterPlugin::produceBluetoothPortConfigurer()
{
	QWidget * const result = new ui::ComPortPicker("SpikeBluetoothPortName", this);
	return result;
}
