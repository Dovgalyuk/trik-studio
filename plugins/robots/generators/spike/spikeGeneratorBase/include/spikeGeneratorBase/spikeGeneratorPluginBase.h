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

#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h"

namespace spike {

namespace robotModel {
class SpikeGeneratorRobotModel;
}

namespace blocks {
class SpikeBlocksFactory;
}

namespace communication {
class SpikeRobotCommunicationThread;
}

/// A base class for every generator from the SPIKE kit.
class ROBOTS_SPIKE_GENERATOR_BASE_EXPORT SpikeGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	enum class RunPolicy {
		Ask
		, AlwaysRun
		, NeverRun
	};

	SpikeGeneratorPluginBase(const QString &usbRobotName, const QString &usbRobotFriendlyName, int usbPriority
			, const QString &bluetoothRobotName, const QString &bluetoothRobotFriendlyName, int bluetoothPriority);

	~SpikeGeneratorPluginBase() override;

	QString kitId() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;
	QSharedPointer<communication::SpikeRobotCommunicationThread> currentCommunicator();

private:
	QScopedPointer<robotModel::SpikeGeneratorRobotModel> mUsbRobotModel;
	QScopedPointer<robotModel::SpikeGeneratorRobotModel> mBluetoothRobotModel;
	QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> mBlocksFactory;
};

}

