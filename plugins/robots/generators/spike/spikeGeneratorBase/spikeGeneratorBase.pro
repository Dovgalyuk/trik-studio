# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET = robots-spike-generator-base

TEMPLATE = lib

include(../../../../../global.pri)

QT += widgets

CONFIG += trikqscintilla2

includes(plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/spikeKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrrepo robots-generator-base robots-kit-base robots-spike-kit)

DEFINES += ROBOTS_SPIKE_GENERATOR_BASE_LIBRARY

HEADERS += \
	$$PWD/include/spikeGeneratorBase/spikeBlocksValidator.h \
	$$PWD/include/spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h \
	$$PWD/include/spikeGeneratorBase/spikeGeneratorPluginBase.h \
	$$PWD/include/spikeGeneratorBase/spikeMasterGeneratorBase.h \
	$$PWD/include/spikeGeneratorBase/robotModel/generatorModelExtensionInterface.h \
	$$PWD/include/spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h \
	$$PWD/src/spikeGeneratorCustomizer.h \
	$$PWD/src/spikeGeneratorFactory.h \
	$$PWD/src/parts/spikeDeviceVariables.h \
	$$PWD/src/converters/backgroundColorConverter.h \
	$$PWD/src/converters/ledColorConverter.h \
	$$PWD/src/simpleGenerators/detectGenerator.h \
	$$PWD/src/simpleGenerators/initCameraGenerator.h \
	$$PWD/src/simpleGenerators/ledGenerator.h \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.h \
	$$PWD/src/simpleGenerators/playToneGenerator.h \
	$$PWD/src/simpleGenerators/sadSmileGenerator.h \
	$$PWD/src/simpleGenerators/sayGenerator.h \
	$$PWD/src/simpleGenerators/systemGenerator.h \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.h \
	$$PWD/src/simpleGenerators/smileGenerator.h \
	$$PWD/src/simpleGenerators/spikeEnginesGenerator.h \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.h \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.h \
	$$PWD/src/simpleGenerators/drawLineGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelGenerator.h \
	$$PWD/src/simpleGenerators/drawRectGenerator.h \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.h \
	$$PWD/src/simpleGenerators/drawArcGenerator.h \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.h \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.h \
	$$PWD/src/simpleGenerators/sendMessageGenerator.h \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.h \
	$$PWD/src/simpleGenerators/waitPadPressGenerator.h \
	$$PWD/src/simpleGenerators/fileWriteGenerator.h \
	$$PWD/src/simpleGenerators/removeFileGenerator.h \
	$$PWD/src/simpleGenerators/stopCameraGenerator.h \
	$$PWD/src/simpleGenerators/markerDownGenerator.h \
	$$PWD/src/simpleGenerators/markerUpGenerator.h \
	$$PWD/src/simpleGenerators/printTextGenerator.h \
	$$PWD/src/simpleGenerators/readLidarGenerator.h \
	$$PWD/src/simpleGenerators/spikeJoinNetworkGenerator.h \

SOURCES += \
	$$PWD/src/spikeBlocksValidator.cpp \
	$$PWD/src/spikeGeneratorPluginBase.cpp \
	$$PWD/src/spikeMasterGeneratorBase.cpp \
	$$PWD/src/spikeGeneratorCustomizer.cpp \
	$$PWD/src/spikeGeneratorFactory.cpp \
	$$PWD/src/robotModel/spikeGeneratorRobotModel.cpp \
	$$PWD/src/parts/spikeDeviceVariables.cpp \
	$$PWD/src/converters/backgroundColorConverter.cpp \
	$$PWD/src/converters/ledColorConverter.cpp \
	$$PWD/src/simpleGenerators/detectGenerator.cpp \
	$$PWD/src/simpleGenerators/initCameraGenerator.cpp \
	$$PWD/src/simpleGenerators/ledGenerator.cpp \
	$$PWD/src/simpleGenerators/detectorToVariableGenerator.cpp \
	$$PWD/src/simpleGenerators/playToneGenerator.cpp \
	$$PWD/src/simpleGenerators/sadSmileGenerator.cpp \
	$$PWD/src/simpleGenerators/sayGenerator.cpp \
	$$PWD/src/simpleGenerators/systemGenerator.cpp \
	$$PWD/src/simpleGenerators/setBackgroundGenerator.cpp \
	$$PWD/src/simpleGenerators/smileGenerator.cpp \
	$$PWD/src/simpleGenerators/spikeEnginesGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForInfraredSensorGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMotionGenerator.cpp \
	$$PWD/src/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/src/simpleGenerators/drawEllipseGenerator.cpp \
	$$PWD/src/simpleGenerators/drawArcGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterWidthGenerator.cpp \
	$$PWD/src/simpleGenerators/setPainterColorGenerator.cpp \
	$$PWD/src/simpleGenerators/sendMessageGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForMessageGenerator.cpp \
	$$PWD/src/simpleGenerators/waitPadPressGenerator.cpp \
	$$PWD/src/simpleGenerators/fileWriteGenerator.cpp \
	$$PWD/src/simpleGenerators/removeFileGenerator.cpp \
	$$PWD/src/simpleGenerators/stopCameraGenerator.cpp \
	$$PWD/src/simpleGenerators/markerDownGenerator.cpp \
	$$PWD/src/simpleGenerators/markerUpGenerator.cpp \
	$$PWD/src/simpleGenerators/printTextGenerator.cpp \
	$$PWD/src/simpleGenerators/readLidarGenerator.cpp \
	$$PWD/src/simpleGenerators/spikeJoinNetworkGenerator.cpp \
