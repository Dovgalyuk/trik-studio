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

#include "motor.h"

#include <spikeKit/communication/commandConstants.h>
#include <spikeKit/communication/spikeDirectCommand.h>

using namespace spike::robotModel::real::parts;
using namespace spike::communication;
using namespace kitBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Motor::Motor(const DeviceInfo &info, const PortInfo &port, RobotCommunicator &robotCommunicator)
	: SpikeMotor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Motor::on(int speed)
{
	QByteArray command = SpikeDirectCommand::formCommand(19, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_POWER, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);
	SpikeDirectCommand::addByteParameter(speed, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_START, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Motor::stop()
{
	on(0);
}

void Motor::off()
{
	QByteArray command = SpikeDirectCommand::formCommand(14, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_STOP, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);
	SpikeDirectCommand::addByteParameter(0x00, command, index); // brake (0 = coast, 1 = brake)

	mRobotCommunicator.send(this, command, 3);
}

char Motor::parsePort(QChar portName)
{
	if (portName == 'A') {
		return enums::outputPort::OutputPortEnum::A;
	} else if (portName == 'B') {
		return enums::outputPort::OutputPortEnum::B;
	} else if (portName == 'C') {
		return enums::outputPort::OutputPortEnum::C;
	} else if (portName == 'D') {
		return enums::outputPort::OutputPortEnum::D;
	}
	return 0x00;
}

