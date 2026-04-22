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

#include "led.h"

#include <QtCore/QDebug>

#include <spikeKit/communication/commandConstants.h>
#include <spikeKit/communication/spikeDirectCommand.h>

using namespace spike::robotModel::real::parts;
using namespace spike::communication;
using namespace kitBase::robotModel;

Led::Led(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotModel::parts::SpikeLed(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Led::setColor(spike::robotModel::parts::SpikeLedColor color)
{
	QByteArray command = SpikeDirectCommand::formCommand(11, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_WRITE_LED, command, index);
	SpikeDirectCommand::addByteParameter(colorByte(color), command, index);

	mRobotCommunicator.send(this, command, 3);
}

qint8 Led::colorByte(spike::robotModel::parts::SpikeLedColor color) const
{
	switch (color) {
	case spike::robotModel::parts::SpikeLedColor::off:
		return 0;
	case spike::robotModel::parts::SpikeLedColor::green:
		return 1;
	case spike::robotModel::parts::SpikeLedColor::red:
		return 2;
	case spike::robotModel::parts::SpikeLedColor::orange:
		return 3;
	case spike::robotModel::parts::SpikeLedColor::greenFlash:
		return 4;
	case spike::robotModel::parts::SpikeLedColor::redFlash:
		return 5;
	case spike::robotModel::parts::SpikeLedColor::orangeFlash:
		return 6;
	case spike::robotModel::parts::SpikeLedColor::greenPulse:
		return 7;
	case spike::robotModel::parts::SpikeLedColor::redPulse:
		return 8;
	case spike::robotModel::parts::SpikeLedColor::orangePulse:
		return 9;
	default:
		qWarning() << "Unknown SPIKE LED color:" << static_cast<int>(color);
		return 0;
	}
}
