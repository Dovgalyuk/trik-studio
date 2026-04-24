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

#include "spikeGeneratorFactory.h"

#include <generatorBase/converters/regexpMultiConverter.h>
#include <generatorBase/simpleGenerators/waitForButtonGenerator.h>
#include <generatorBase/lua/luaProcessor.h>

#include "simpleGenerators/detectGenerator.h"
#include "simpleGenerators/detectorToVariableGenerator.h"
#include "simpleGenerators/drawLineGenerator.h"
#include "simpleGenerators/drawPixelGenerator.h"
#include "simpleGenerators/drawRectGenerator.h"
#include "simpleGenerators/drawEllipseGenerator.h"
#include "simpleGenerators/drawArcGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/stopCameraGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/waitForMessageGenerator.h"
#include "simpleGenerators/sadSmileGenerator.h"
#include "simpleGenerators/printTextGenerator.h"
#include "simpleGenerators/sayGenerator.h"
#include "simpleGenerators/sendMessageGenerator.h"
#include "simpleGenerators/setPainterWidthGenerator.h"
#include "simpleGenerators/setPainterColorGenerator.h"
#include "simpleGenerators/setBackgroundGenerator.h"
#include "simpleGenerators/smileGenerator.h"
#include "simpleGenerators/systemGenerator.h"
#include "simpleGenerators/spikeEnginesGenerator.h"
#include "simpleGenerators/waitForInfraredSensorGenerator.h"
#include "simpleGenerators/waitForMotionGenerator.h"
#include "simpleGenerators/waitPadPressGenerator.h"
#include "simpleGenerators/fileWriteGenerator.h"
#include "simpleGenerators/removeFileGenerator.h"
#include "simpleGenerators/markerDownGenerator.h"
#include "simpleGenerators/markerUpGenerator.h"
#include "simpleGenerators/readLidarGenerator.h"
#include "parts/spikeDeviceVariables.h"
#include "simpleGenerators/spikeJoinNetworkGenerator.h"

using namespace spike;
using namespace spike::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

SpikeGeneratorFactory::SpikeGeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, lua::LuaProcessor &luaProcessor
		, const QStringList &pathsToTemplates)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mPathsToTemplates(pathsToTemplates)
{
	mDeviceVariables.reset(new spike::parts::SpikeDeviceVariables());
}

SpikeGeneratorFactory::~SpikeGeneratorFactory()
{
}

void SpikeGeneratorFactory::initDeviceVariables()
{
	mDeviceVariables.reset(new spike::parts::SpikeDeviceVariables());
}

AbstractSimpleGenerator *SpikeGeneratorFactory::simpleGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();
	if (elementType.contains("EnginesForward")
			|| elementType.contains("EnginesBackward")
			|| elementType.contains("AngularServo"))
	{
		return new SpikeEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType == "SpikePlayTone") {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawLine") {
		return new DrawLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawPixel") {
		return new DrawPixelGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawRect") {
		return new DrawRectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawEllipse") {
		return new DrawEllipseGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDrawArc") {
		return new DrawArcGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSetPainterWidth") {
		return new SetPainterWidthGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSetPainterColor") {
		return new SetPainterColorGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSmile") {
		return new SmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSadSmile") {
		return new SadSmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikePrintText") {
		return new PrintTextGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSay") {
		return new SayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSendMessage") {
		return new SendMessageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitForMessage") {
		return new WaitForMessageGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeJoinNetwork") {
		return new SpikeJoinNetworkGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSetBackground") {
		return new SetBackgroundGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeSystem") {
		return new SystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeLed") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDetect") {
		return new DetectGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeInitCamera") {
		return new InitCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeStopCamera") {
		return new StopCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeDetectorToVariable") {
		return new DetectorToVariableGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitForMotion") {
		return new WaitForMotionGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitForIRDistance") {
		return new WaitForInfraredSensorGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWaitPadPress") {
		return new WaitPadPressGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeWriteToFile") {
		return new FileWriteGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SpikeRemoveFile") {
		return new RemoveFileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "MarkerDown") {
		return new MarkerDownGenerator(mRepo, customizer, id, this);
	} else if (elementType == "MarkerUp") {
		return new MarkerUpGenerator(mRepo, customizer, id, this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QStringList SpikeGeneratorFactory::pathsToTemplates() const
{
	return mPathsToTemplates; //{":/" + mGeneratorName + "/templates"};
}
