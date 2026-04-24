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

#include "spikeKit/blocks/spikeBlocksFactory.h"

#include <kitBase/blocksBase/common/enginesStopBlock.h>
#include <kitBase/blocksBase/common/clearEncoderBlock.h>

#include <kitBase/blocksBase/common/waitForColorBlock.h>
#include <kitBase/blocksBase/common/waitForColorIntensityBlock.h>
#include <kitBase/blocksBase/common/waitForEncoderBlock.h>
#include <kitBase/blocksBase/common/waitForLightSensorBlock.h>
#include <kitBase/blocksBase/common/waitForSonarDistanceBlock.h>
#include <kitBase/blocksBase/common/waitForSoundSensorBlock.h>
#include <kitBase/blocksBase/common/waitForTouchSensorBlock.h>
#include <kitBase/blocksBase/common/waitForGyroscopeSensorBlock.h>
#include <kitBase/blocksBase/common/waitForButtonBlock.h>
#include <kitBase/blocksBase/common/calibrateGyroscopeBlock.h>

#include <kitBase/robotModel/robotParts/rangeSensor.h>

#include "details/beepBlock.h"
#include "details/playToneBlock.h"
#include "details/ledBlock.h"
#include "details/spikeReadRGBBlock.h"

#include "details/spikeEnginesForwardBlock.h"
#include "details/spikeEnginesBackwardBlock.h"

#include "details/drawPixelBlock.h"
#include "details/drawLineBlock.h"
#include "details/drawRectBlock.h"
#include "details/drawCircleBlock.h"

using namespace spike::blocks;
using namespace details;
using namespace kitBase::blocksBase::common;

SpikeBlocksFactory::SpikeBlocksFactory(const QStringList &interpretedModels)
	: mInterpretedModels(interpretedModels)
{
}

qReal::interpretation::Block *SpikeBlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "SpikeBeep")) {
		return new BeepBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikePlayTone")) {
		return new PlayToneBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeEnginesForward")) {
		return new details::SpikeEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeEnginesBackward")) {
		return new details::SpikeEnginesBackwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeEnginesStop")) {
		return new EnginesStopBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeClearEncoder")) {
		return new ClearEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeLed")) {
		return new LedBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeReadRGB")) {
		return new details::SpikeReadRGBBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "SpikeWaitForTouchSensor")) {
		return new WaitForTouchSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForSonarDistance")) {
		return new WaitForSonarDistanceBlock(mRobotModelManager->model()
				, kitBase::robotModel::DeviceInfo::create<
						kitBase::robotModel::robotParts::RangeSensor>());
	} else if (elementMetatypeIs(element, "SpikeWaitForColor")) {
		return new WaitForColorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForColorIntensity")) {
		return new WaitForColorIntensityBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForLight")) {
		return new WaitForLightSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForSound")) {
		return new WaitForSoundSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeWaitForGyroscope")) {
		return new WaitForGyroscopeSensorBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeCalibrateGyroscope")) {
		return new CalibrateGyroscopeBlock(mRobotModelManager->model());

	} else if (elementMetatypeIs(element, "SpikeWaitForButton")) {
		return new WaitForButtonBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeDrawPixel")) {
		return new DrawPixelBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeDrawLine")) {
		return new DrawLineBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeDrawCircle")) {
		return new DrawCircleBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "SpikeDrawRect")) {
		return new DrawRectBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList SpikeBlocksFactory::providedBlocks() const
{
	return {
		id("SpikeBeep")
		, id("SpikePlayTone")
		, id("SpikeEnginesForward")
		, id("SpikeEnginesBackward")
		, id("SpikeEnginesStop")
		, id("SpikeClearEncoder")
		, id("SpikeCalibrateGyroscope")
		, id("SpikeLed")
		, id("SpikeSendMail")
		, id("SpikeStartCompassCalibration")
		, id("SpikeStopCompassCalibration")
		, id("SpikeReadRGB")

		, id("SpikeWaitForTouchSensor")
		, id("SpikeWaitForSonarDistance")
		, id("SpikeWaitForLight")
		, id("SpikeWaitForColor")
		, id("SpikeWaitForColorIntensity")
		, id("SpikeWaitForEncoder")
		, id("SpikeWaitForButton")
		, id("SpikeWaitForSound")
		, id("SpikeWaitForGyroscope")
		, id("SpikeWaitForReceivingMail")

		, id("SpikeCalibrateWhiteLL")
		, id("SpikeCalibrateBlackLL")
		, id("SpikeCalibratePIDLL")
		, id("SpikeSleepLL")
		, id("SpikeWakeUpLL")
		, id("SpikeReadAvrLL")
		, id("SpikeReadAllLL")
		, id("SpikeReadSteeringLL")

		, id("SpikeDrawPixel")
		, id("SpikeDrawLine")
		, id("SpikeDrawCircle")
		, id("SpikeDrawRect")
	};
}

qReal::IdList SpikeBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("SpikeWaitForSound")
				<< id("SpikeSendMail")
				<< id("SpikeWaitForReceivingMail")
				<< id("SpikeStartCompassCalibration")
				<< id("SpikeStopCompassCalibration")
				<< id("SpikeCalibrateWhiteLL")
				<< id("SpikeCalibrateBlackLL")
				<< id("SpikeCalibratePIDLL")
				<< id("SpikeSleepLL")
				<< id("SpikeWakeUpLL")
				<< id("SpikeReadAvrLL")
				<< id("SpikeReadAllLL")
				<< id("SpikeReadSteeringLL")
				<< id("SpikeWaitForReceivingMail")
				;
	} else {
		if (!mInterpretedModels.contains(mRobotModelManager->model().robotId())) {
			result << id("Join") << id("SendMessageThreads") << id("ReceiveMessageThreads") << id("KillThread");
		}
		if (!mRobotModelManager->model().name().contains("Gen")) {
			result
					<< id("SpikeCalibrateGyroscope")
					;
		}
	}

	return result;
}
