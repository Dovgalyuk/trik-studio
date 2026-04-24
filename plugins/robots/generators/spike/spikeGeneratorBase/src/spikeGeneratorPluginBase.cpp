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

#include "spikeGeneratorBase/spikeGeneratorPluginBase.h"

#include <qrutils/singleton.h>
#include <spikeKit/communication/bluetoothRobotCommunicationThread.h>
#include <spikeKit/communication/usbRobotCommunicationThread.h>
#include <spikeKit/blocks/spikeBlocksFactory.h>

#include "spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h"

#include <qrutils/inFile.h>

using namespace spike;

SpikeGeneratorPluginBase::SpikeGeneratorPluginBase(const QString &usbRobotName, const QString &usbRobotFriendlyName, int usbPriority
			, const QString &bluetoothRobotName, const QString &bluetoothRobotFriendlyName, int bluetoothPriority)
	: mUsbRobotModel(new robotModel::SpikeGeneratorRobotModel(kitId(), "spikeUsbGeneratorRobot"
			, usbRobotName, usbRobotFriendlyName, usbPriority
			, utils::Singleton<communication::UsbRobotCommunicationThread>::instance()))
	, mBluetoothRobotModel(new robotModel::SpikeGeneratorRobotModel(kitId(), "spikeBluetoothGeneratorRobot"
			, bluetoothRobotName, bluetoothRobotFriendlyName, bluetoothPriority
			, utils::Singleton<communication::BluetoothRobotCommunicationThread>::instance()))
	, mBlocksFactory(new blocks::SpikeBlocksFactory({}))
{
}

SpikeGeneratorPluginBase::~SpikeGeneratorPluginBase()
{
}

QString SpikeGeneratorPluginBase::kitId() const
{
	return "spikeKit";
}

QList<kitBase::robotModel::RobotModelInterface *> SpikeGeneratorPluginBase::robotModels()
{
	return { mUsbRobotModel.data(), mBluetoothRobotModel.data() };
}

QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> SpikeGeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	if (robotModels().contains(const_cast<kitBase::robotModel::RobotModelInterface *>(model))) {
		return mBlocksFactory;
	} else {
		return nullptr;
	}
}

QList<kitBase::AdditionalPreferences *> SpikeGeneratorPluginBase::settingsWidgets()
{
	return {};
}

void SpikeGeneratorPluginBase::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QSharedPointer<communication::SpikeRobotCommunicationThread> SpikeGeneratorPluginBase::currentCommunicator()
{
	if (auto robotModel = dynamic_cast<robotModel::SpikeGeneratorRobotModel *>(&mRobotModelManager->model())) {
		return robotModel->communicator();
	}

	return nullptr;
}
