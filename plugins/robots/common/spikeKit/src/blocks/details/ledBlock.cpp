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

#include "ledBlock.h"

using namespace spike;
using namespace blocks;
using namespace details;
using namespace kitBase::robotModel;

LedBlock::LedBlock(RobotModelInterface &robotModel)
	: kitBase::blocksBase::common::DeviceBlock<robotModel::parts::SpikeLed>(robotModel)
{
}

void LedBlock::doJob(robotModel::parts::SpikeLed &led)
{
	led.setColor(toLedColor(stringProperty("Color")));
	Q_EMIT done(mNextBlockId);
}

robotModel::parts::SpikeLedColor LedBlock::toLedColor(const QString &color) const
{
	if (color == "black") {
		return robotModel::parts::SpikeLedColor::off;
	}

	if (color == "red") {
		return robotModel::parts::SpikeLedColor::red;
	}

	if (color == "green") {
		return robotModel::parts::SpikeLedColor::green;
	}

	if (color == "orange") {
		return robotModel::parts::SpikeLedColor::orange;
	}

	if (color == "red_flash") {
		return robotModel::parts::SpikeLedColor::redFlash;
	}

	if (color == "green_flash") {
		return robotModel::parts::SpikeLedColor::greenFlash;
	}

	if (color == "orange_flash") {
		return robotModel::parts::SpikeLedColor::orangeFlash;
	}

	if (color == "red_pulse") {
		return robotModel::parts::SpikeLedColor::redPulse;
	}

	if (color == "green_pulse") {
		return robotModel::parts::SpikeLedColor::greenPulse;
	}

	if (color == "orange_pulse") {
		return robotModel::parts::SpikeLedColor::orangePulse;
	}

	return robotModel::parts::SpikeLedColor::off;
}
