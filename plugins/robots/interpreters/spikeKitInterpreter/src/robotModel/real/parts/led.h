/* Copyright 2015 CyberTech Labs Ltd.
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

#include <spikeKit/robotModel/parts/spikeLed.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace spike {
namespace robotModel {
namespace real {
namespace parts {

/// An implementation of the real SPIKE LED device.
class Led : public robotModel::parts::SpikeLed
{
	Q_OBJECT

public:
	Led(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);


	void setColor(spike::robotModel::parts::SpikeLedColor color) override;

private:
	qint8 colorByte(spike::robotModel::parts::SpikeLedColor color) const;

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
