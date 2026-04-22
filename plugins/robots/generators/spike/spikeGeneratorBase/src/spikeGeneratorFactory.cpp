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

#include "spikeGeneratorBase/spikeGeneratorFactory.h"

#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawCircleGenerator.h"
#include "simpleGenerators/enginesGenerator.h"
#include "simpleGenerators/enginesStopGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/nullificationEncoderGenerator.h"
#include "simpleGenerators/sendMailGenerator.h"
#include "simpleGenerators/receiveMailGenerator.h"
#include "simpleGenerators/calibrateGyroscopeGenerator.h"
#include "simpleGenerators/startCompassCalibrationGenerator.h"
#include "simpleGenerators/stopCompassCalibrationGenerator.h"
#include "simpleGenerators/readRGBGenerator.h"
#include "simpleGenerators/waitForColorBlockGenerator.h"
#include "simpleGenerators/subprogramCallGenerator.h"

#include "simpleGenerators/lineLeader/calibrateBlackGenerator.h"
#include "simpleGenerators/lineLeader/calibratePIDGenerator.h"
#include "simpleGenerators/lineLeader/calibrateWhiteGenerator.h"
#include "simpleGenerators/lineLeader/readAverageGenerator.h"
#include "simpleGenerators/lineLeader/readSensorArrayGenerator.h"
#include "simpleGenerators/lineLeader/readSteeringGenerator.h"
#include "simpleGenerators/lineLeader/sleepGenerator.h"
#include "simpleGenerators/lineLeader/wakeUpGenerator.h"


#include "converters/outputPortNameConverter.h"
#include "converters/ledColorConverter.h"
#include "converters/colorConverter.h"

using namespace spike;
using namespace spike::simple;
using namespace spike::converters;
using namespace generatorBase::simple;

SpikeGeneratorFactory::SpikeGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName) // NOLINT(modernize-pass-by-value)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
	, mMailboxes({":/" + mGeneratorName + "/templates"})
{
}

SpikeGeneratorFactory::~SpikeGeneratorFactory() = default;

parts::Mailboxes &SpikeGeneratorFactory::mailboxes()
{
	return mMailboxes;
}

AbstractSimpleGenerator *SpikeGeneratorFactory::ifGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting)
{
	return GeneratorFactoryBase::ifGenerator(id, customizer, elseIsEmpty, needInverting);
}

AbstractSimpleGenerator *SpikeGeneratorFactory::forLoopGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	return GeneratorFactoryBase::forLoopGenerator(id, customizer);
}

generatorBase::simple::AbstractSimpleGenerator *SpikeGeneratorFactory::simpleGenerator(const qReal::Id &id
		, generatorBase::GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType == "SpikeDrawLine") {
		return new DrawLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawPixel") {
		return new DrawPixelGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawRect") {
		return new DrawRectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawCircle") {
		return new DrawCircleGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("EnginesForward") || elementType.contains("EnginesBackward")) {
		return new EnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "SpikeEnginesStop") {
		return new EnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeLed") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeClearEncoder") {
		return new NullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSendMail") {
		return new SendMailGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitForReceivingMail") {
		return new ReceiveMailGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeCalibrateGyroscope") {
		return new CalibrateGyroscopeGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeStartCompassCalibration") {
		return new StartCompassCalibrationGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeStopCompassCalibration") {
		return new StopCompassCalibrationGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeReadRGB") {
		return new ReadRGBGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitForColor") {
		return new WaitForColorBlockGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Subprogram") {
		return new SubprogramCallGenerator(mRepo, customizer, id, this);
	}
	else if (elementType == "SpikeCalibrateWhiteLL") {
		return new lineLeader::CalibrateWhiteGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeCalibrateBlackLL") {
		return new lineLeader::CalibrateBlackGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeCalibratePIDLL") {
		return new lineLeader::CalibratePIDGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSleepLL") {
		return new lineLeader::SleepGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWakeUpLL") {
		return new lineLeader::WakeUpGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeReadAvrLL") {
		return new lineLeader::ReadAverageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeReadAllLL") {
		return new lineLeader::ReadSensorArrayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeReadSteeringLL") {
		return new lineLeader::ReadSteeringGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

Binding::ConverterInterface *SpikeGeneratorFactory::outputPortNameConverter() const
{
	return new OutputPortNameConverter(pathsToTemplates()
			, mRobotModelManager.model().availablePorts());
}

Binding::ConverterInterface *SpikeGeneratorFactory::ledColorConverter() const
{
	return new LedColorConverter(pathsToTemplates());
}

Binding::ConverterInterface *SpikeGeneratorFactory::colorConverter() const
{
	return new ColorConverter(pathsToTemplates());
}

QStringList SpikeGeneratorFactory::pathsToTemplates() const
{
	return { ":/" + mGeneratorName + "/templates" };
}

void SpikeGeneratorFactory::reportError(const QString &errorMessage, const qReal::Id &id)
{
	mErrorReporter.addError(errorMessage, id);
}
