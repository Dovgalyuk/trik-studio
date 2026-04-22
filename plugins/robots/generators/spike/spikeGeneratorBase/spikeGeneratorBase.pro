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

QT += widgets xml

CONFIG += trikqscintilla2

includes(plugins/robots/generators/spike/spikeGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/spikeKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrrepo robots-generator-base robots-spike-kit robots-kit-base)

DEFINES += ROBOTS_SPIKE_GENERATOR_BASE_LIBRARY

HEADERS += \
	$$PWD/include/spikeGeneratorBase/spikeGeneratorPluginBase.h \
	$$PWD/include/spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h \
	$$PWD/include/spikeGeneratorBase/spikeMasterGeneratorBase.h \
	$$PWD/include/spikeGeneratorBase/spikeGeneratorFactory.h \
	$$PWD/include/spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h \
	$$PWD/include/spikeGeneratorBase/parts/spikeMailboxes.h \
	$$PWD/src/spikeGeneratorCustomizer.h \
	$$PWD/src/simpleGenerators/drawLineGenerator.h \
	$$PWD/src/simpleGenerators/drawPixelGenerator.h \
	$$PWD/src/simpleGenerators/drawRectGenerator.h \
	$$PWD/src/simpleGenerators/drawCircleGenerator.h \
	$$PWD/src/simpleGenerators/enginesGenerator.h \
	$$PWD/src/simpleGenerators/enginesStopGenerator.h \
	$$PWD/src/simpleGenerators/ledGenerator.h \
	$$PWD/src/simpleGenerators/nullificationEncoderGenerator.h \
	$$PWD/src/converters/outputPortNameConverter.h \
	$$PWD/src/converters/ledColorConverter.h \
	$$PWD/src/simpleGenerators/sendMailGenerator.h \
	$$PWD/src/simpleGenerators/receiveMailGenerator.h \
	$$PWD/src/simpleGenerators/calibrateGyroscopeGenerator.h \
	$$PWD/src/simpleGenerators/startCompassCalibrationGenerator.h \
	$$PWD/src/simpleGenerators/stopCompassCalibrationGenerator.h \
	$$PWD/src/simpleGenerators/readRGBGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/calibrateBlackGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/calibratePIDGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/calibrateWhiteGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/readAverageGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/readSensorArrayGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/readSteeringGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/sleepGenerator.h \
	$$PWD/src/simpleGenerators/lineLeader/wakeUpGenerator.h \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.h \
	$$PWD/src/converters/colorConverter.h \
	$$PWD/src/simpleGenerators/subprogramCallGenerator.h \

SOURCES += \
	$$PWD/src/spikeGeneratorPluginBase.cpp \
	$$PWD/src/spikeMasterGeneratorBase.cpp \
	$$PWD/src/spikeGeneratorCustomizer.cpp \
	$$PWD/src/spikeGeneratorFactory.cpp \
	$$PWD/src/robotModel/spikeGeneratorRobotModel.cpp \
	$$PWD/src/parts/spikeMailboxes.cpp \
	$$PWD/src/simpleGenerators/drawLineGenerator.cpp \
	$$PWD/src/simpleGenerators/drawPixelGenerator.cpp \
	$$PWD/src/simpleGenerators/drawRectGenerator.cpp \
	$$PWD/src/simpleGenerators/drawCircleGenerator.cpp \
	$$PWD/src/simpleGenerators/enginesGenerator.cpp \
	$$PWD/src/simpleGenerators/enginesStopGenerator.cpp \
	$$PWD/src/simpleGenerators/ledGenerator.cpp \
	$$PWD/src/simpleGenerators/nullificationEncoderGenerator.cpp \
	$$PWD/src/converters/outputPortNameConverter.cpp \
	$$PWD/src/converters/ledColorConverter.cpp \
	$$PWD/src/simpleGenerators/sendMailGenerator.cpp \
	$$PWD/src/simpleGenerators/receiveMailGenerator.cpp \
	$$PWD/src/simpleGenerators/calibrateGyroscopeGenerator.cpp \
	$$PWD/src/simpleGenerators/startCompassCalibrationGenerator.cpp \
	$$PWD/src/simpleGenerators/stopCompassCalibrationGenerator.cpp \
	$$PWD/src/simpleGenerators/readRGBGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/calibrateBlackGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/calibratePIDGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/calibrateWhiteGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/readAverageGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/readSensorArrayGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/readSteeringGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/sleepGenerator.cpp \
	$$PWD/src/simpleGenerators/lineLeader/wakeUpGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForColorBlockGenerator.cpp \
	$$PWD/src/converters/colorConverter.cpp \
	$$PWD/src/simpleGenerators/subprogramCallGenerator.cpp \

OTHER_FILES +=
