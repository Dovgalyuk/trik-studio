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

#include "spikeKit/robotModel/spikeRobotModelBase.h"

#include "spikeKit/robotModel/parts/spikeMotor.h"
#include "spikeKit/robotModel/parts/spikeSpeaker.h"
#include "spikeKit/robotModel/parts/spikeDisplay.h"
#include "spikeKit/robotModel/parts/spikeLed.h"
#include "spikeKit/robotModel/parts/spikeCompass.h"
#include "spikeKit/robotModel/parts/spikeACIRSeeker.h"
#include "spikeKit/robotModel/parts/spikeDCIRSeeker.h"
#include "spikeKit/robotModel/parts/spikeNXTColorSensorV2Color.h"
#include "spikeKit/robotModel/parts/spikeNXTColorSensorV2Passive.h"
#include "spikeKit/robotModel/parts/spikeNXTColorSensorV2RGB.h"
#include "spikeKit/robotModel/parts/spikeNXTColorSensorV2Raw.h"

#include <kitBase/robotModel/robotParts/display.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>
#include <kitBase/robotModel/robotParts/colorSensorAmbient.h>
#include <kitBase/robotModel/robotParts/colorSensorReflected.h>
#include <kitBase/robotModel/robotParts/colorSensorRaw.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>

using namespace spike::robotModel;
using namespace kitBase::robotModel;

SpikeRobotModelBase::SpikeRobotModelBase(const QString &kitId, const QString &robotId)
	: CommonRobotModel(kitId, robotId)
{
	QList<DeviceInfo> const inputPortConnections = {
			touchSensorInfo()
			, lightSensorInfo()
			, rangeSensorInfo()
			, colorRedSensorInfo()
			, colorGreenSensorInfo()
			, colorBlueSensorInfo()
			, colorPassiveSensorInfo()
			, colorFullSensorInfo()
			, colorAmbientSensorInfo()
			, colorReflectedSensorInfo()
			, colorRawSensorInfo()
			, gyroscopeSensorInfo()
			, compassSensorInfo()
			, ACIRSeekerSensorInfo()
			, DCIRSeekerSensorInfo()
			, NXTColorSensorV2ColorInfo()
			, NXTColorSensorV2RGBInfo()
			, NXTColorSensorV2PassiveInfo()
			, NXTColorSensorV2RawInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });
	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });

	addAllowedConnection(PortInfo("Up", input, {}, "buttonUp"), { buttonInfo() });
	addAllowedConnection(PortInfo("Enter", input, {}, "buttonEnter"), { buttonInfo() });
	addAllowedConnection(PortInfo("Down", input, {}, "buttonDown"), { buttonInfo() });
	addAllowedConnection(PortInfo("Right", input, {}, "buttonRight"), { buttonInfo() });
	addAllowedConnection(PortInfo("Left", input, {}, "buttonLeft"), { buttonInfo() });
	addAllowedConnection(PortInfo("Back", input, {}, "buttonBack"), { buttonInfo() });

	addAllowedConnection(PortInfo("A", output, { QString::fromUtf8("А") }), { motorInfo() });
	addAllowedConnection(PortInfo("B", output, { QString::fromUtf8("В") }), { motorInfo() });
	addAllowedConnection(PortInfo("C", output, { QString::fromUtf8("С") }), { motorInfo() });
	addAllowedConnection(PortInfo("D", output), { motorInfo() });

	addAllowedConnection(PortInfo("A", input, { QString::fromUtf8("А") }, "encoderA"), { encoderInfo() });
	addAllowedConnection(PortInfo("B", input, { QString::fromUtf8("В") }, "encoderB"), { encoderInfo() });
	addAllowedConnection(PortInfo("C", input, { QString::fromUtf8("С") }, "encoderC"), { encoderInfo() });
	addAllowedConnection(PortInfo("D", input, {}, "encoderD"), { encoderInfo() });

	addAllowedConnection(PortInfo("1", input, {}, "sensor1"), inputPortConnections);
	addAllowedConnection(PortInfo("2", input, {}, "sensor2"), inputPortConnections);
	addAllowedConnection(PortInfo("3", input, {}, "sensor3"), inputPortConnections);
	addAllowedConnection(PortInfo("4", input, {}, "sensor4"), inputPortConnections);
}

QList<DeviceInfo> SpikeRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::ColorSensorFull>()
			, DeviceInfo::create<robotParts::ColorSensorRed>()
			, DeviceInfo::create<robotParts::ColorSensorBlue>()
			, DeviceInfo::create<robotParts::ColorSensorGreen>()
			, DeviceInfo::create<robotParts::ColorSensorPassive>()
			, DeviceInfo::create<robotParts::ColorSensorAmbient>()
			, DeviceInfo::create<robotParts::ColorSensorReflected>()
			, DeviceInfo::create<robotParts::ColorSensorRaw>()
			, DeviceInfo::create<robotParts::GyroscopeSensor>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeCompass>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeACIRSeeker>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeDCIRSeeker>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Color>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2RGB>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Passive>()
			, DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Raw>()
	};
}

DeviceInfo SpikeRobotModelBase::motorInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeMotor>();
}

DeviceInfo SpikeRobotModelBase::encoderInfo() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}

DeviceInfo SpikeRobotModelBase::displayInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeDisplay>();
}

DeviceInfo SpikeRobotModelBase::ledInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeLed>();
}

DeviceInfo SpikeRobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeSpeaker>();
}

DeviceInfo SpikeRobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
}

DeviceInfo SpikeRobotModelBase::touchSensorInfo() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}

DeviceInfo SpikeRobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}

DeviceInfo SpikeRobotModelBase::rangeSensorInfo() const
{
	return DeviceInfo::create<robotParts::RangeSensor>();
}

DeviceInfo SpikeRobotModelBase::colorFullSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorFull>();
}

DeviceInfo SpikeRobotModelBase::colorRedSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorRed>();
}

DeviceInfo SpikeRobotModelBase::colorGreenSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorGreen>();
}

DeviceInfo SpikeRobotModelBase::colorBlueSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorBlue>();
}

DeviceInfo SpikeRobotModelBase::colorPassiveSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorPassive>();
}

DeviceInfo SpikeRobotModelBase::colorAmbientSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorAmbient>();
}

DeviceInfo SpikeRobotModelBase::colorReflectedSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorReflected>();
}

DeviceInfo SpikeRobotModelBase::colorRawSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorRaw>();
}

DeviceInfo SpikeRobotModelBase::gyroscopeSensorInfo() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}

DeviceInfo SpikeRobotModelBase::compassSensorInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeCompass>();
}

DeviceInfo SpikeRobotModelBase::ACIRSeekerSensorInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeACIRSeeker>();
}

DeviceInfo SpikeRobotModelBase::DCIRSeekerSensorInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeDCIRSeeker>();
}

DeviceInfo SpikeRobotModelBase::NXTColorSensorV2ColorInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Color>();
}

DeviceInfo SpikeRobotModelBase::NXTColorSensorV2RGBInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2RGB>();
}

DeviceInfo SpikeRobotModelBase::NXTColorSensorV2PassiveInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Passive>();
}

DeviceInfo SpikeRobotModelBase::NXTColorSensorV2RawInfo() const
{
	return DeviceInfo::create<spike::robotModel::parts::SpikeNXTColorSensorV2Raw>();
}
