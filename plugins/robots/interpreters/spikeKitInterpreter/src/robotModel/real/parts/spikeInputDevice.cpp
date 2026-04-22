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

#include "spikeInputDevice.h"

#include <spikeKit/communication/commandConstants.h>
#include <spikeKit/communication/spikeDirectCommand.h>

using namespace spike::robotModel::real::parts;
using namespace spike::communication;
using namespace kitBase::robotModel;
using namespace utils;

SpikeInputDevice::SpikeInputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
		, const kitBase::robotModel::PortInfo &port)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
{
}

void SpikeInputDevice::send(const QByteArray &command, const unsigned responseSize, QByteArray reading)
{
	mRobotCommunicator.send(command, responseSize, reading);
}

char SpikeInputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}

QByteArray SpikeInputDevice::readySiCommand(int port, int sensorMode)
{
	QByteArray command = SpikeDirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_SI, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(port, command, index);
	SpikeDirectCommand::addByteParameter(0x00, command, index);        //type (0 = Don’t change type)
	SpikeDirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	SpikeDirectCommand::addByteParameter(0x01, command, index);        // # values
	SpikeDirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}

QByteArray SpikeInputDevice::readyRawCommand(int port, int sensorMode)
{
	QByteArray command = SpikeDirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(port, command, index);
	SpikeDirectCommand::addByteParameter(0x00, command, index);        // type (0 = Don’t change type)
	SpikeDirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	SpikeDirectCommand::addByteParameter(0x01, command, index);        // # values
	SpikeDirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}

QByteArray SpikeInputDevice::readyPercentCommand(int port, int sensorMode)
{
	QByteArray command = SpikeDirectCommand::formCommand(21, 2, 4, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_PCT, command, index);
	SpikeDirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::SPIKE, command, index);
	SpikeDirectCommand::addByteParameter(port, command, index);
	SpikeDirectCommand::addByteParameter(0x00, command, index);        // type (0 = Don’t change type)
	SpikeDirectCommand::addByteParameter(sensorMode, command, index);  // mode – Device mode [0-7]
	SpikeDirectCommand::addByteParameter(0x01, command, index);        // # values
	SpikeDirectCommand::addGlobalIndex(0x00, command, index);          // index for return data
	return command;
}
