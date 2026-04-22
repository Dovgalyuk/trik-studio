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

#include "display.h"

#include <spikeKit/communication/commandConstants.h>
#include <spikeKit/communication/spikeDirectCommand.h>

using namespace spike::robotModel::real::parts;
using namespace spike::communication;
using namespace kitBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Display::Display(const DeviceInfo &info, const PortInfo &port, RobotCommunicator &robotCommunicator)
	: robotModel::parts::SpikeDisplay(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Display::drawPixel(int x, int y)
{
	QByteArray command = SpikeDirectCommand::formCommand(19, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::UI_DRAW_PIXEL, command, index);
	SpikeDirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	SpikeDirectCommand::addShortParameter(x, command, index);
	SpikeDirectCommand::addShortParameter(y, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	QByteArray command = SpikeDirectCommand::formCommand(25, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_LINE, command, index);
	SpikeDirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	SpikeDirectCommand::addShortParameter(x1, command, index);
	SpikeDirectCommand::addShortParameter(y1, command, index);
	SpikeDirectCommand::addShortParameter(x2, command, index);
	SpikeDirectCommand::addShortParameter(y2, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height, bool filled)
{

	QByteArray command = SpikeDirectCommand::formCommand(25, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	if (filled) {
		SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_RECT, command, index);
	} else {
		SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_RECT, command, index);
	}
	SpikeDirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	SpikeDirectCommand::addShortParameter(x, command, index);
	SpikeDirectCommand::addShortParameter(y, command, index);
	SpikeDirectCommand::addShortParameter(width, command, index);
	SpikeDirectCommand::addShortParameter(height, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::drawCircle(int x, int y, int radius, bool filled)
{
	QByteArray command = SpikeDirectCommand::formCommand(22, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	if (filled) {
		SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_CIRCLE, command, index);
	} else {
		SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CIRCLE, command, index);
	}
	SpikeDirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	SpikeDirectCommand::addShortParameter(x, command, index);
	SpikeDirectCommand::addShortParameter(y, command, index);
	SpikeDirectCommand::addShortParameter(radius, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}


void Display::printText(int x, int y, const QString &text, int fontSize)
{
	Q_UNUSED(fontSize)
	QByteArray command = SpikeDirectCommand::formCommand(21 + text.length(), 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_TEXT, command, index);
	SpikeDirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	SpikeDirectCommand::addShortParameter(x, command, index);
	SpikeDirectCommand::addShortParameter(y, command, index);
	SpikeDirectCommand::addStringParameter(text, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::clearScreen()
{
	QByteArray command = SpikeDirectCommand::formCommand(11, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CLEAN, command, index);
	SpikeDirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);

	mRobotCommunicator.send(this, command, 3);
}

void Display::redraw()
{
}
