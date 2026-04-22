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

#include "speaker.h"

#include <spikeKit/communication/commandConstants.h>
#include <spikeKit/communication/spikeDirectCommand.h>

using namespace spike::robotModel::real::parts;
using namespace spike::communication;
using namespace kitBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
		: robotModel::parts::SpikeSpeaker(info, port)
		, mRobotCommunicator(robotCommunicator)
{
}

void Speaker::playTone(int volume, int frequency, int duration)
{
	QByteArray command = SpikeDirectCommand::formCommand(17, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::SOUND_TONE, command, index);
	SpikeDirectCommand::addByteParameter(volume, command, index);
	SpikeDirectCommand::addShortParameter(frequency, command, index);
	SpikeDirectCommand::addShortParameter(duration, command, index);

	mRobotCommunicator.send(this, command, 3);
}
