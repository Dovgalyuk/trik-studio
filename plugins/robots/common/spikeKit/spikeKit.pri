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

QT += widgets serialport

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
                robots-utils robots-kit-base robots-2d-model)

includes(plugins/robots/common/spikeKit \
		plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/utils \
		qrtext \
                plugins/robots/thirdparty/hidapi/hidapi \
)


# libusb headers contain dirty code
gcc:QMAKE_CXXFLAGS += -fpermissive

#HIDAPI windows/hid.c contains bad code (2019, June)
win32: QMAKE_CXXFLAGS += -Wno-error=cast-qual
win32: LIBS += -lsetupapi

macx: LIBS += -framework CoreFoundation -framework AppKit

linux:!autolupdate {
    CONFIG *= link_pkgconfig
    PKGCONFIG += libudev #for hidapi
}

HEADERS += \
	$$PWD/include/spikeKit/robotModel/spikeRobotModelBase.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeDisplay.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeSpeaker.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeMotor.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeLed.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeCompass.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeDCIRSeeker.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeACIRSeeker.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeNXTColorSensorV2Color.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeNXTColorSensorV2RGB.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeNXTColorSensorV2Passive.h \
	$$PWD/include/spikeKit/robotModel/parts/spikeNXTColorSensorV2Raw.h \
	$$PWD/include/spikeKit/blocks/spikeBlocksFactory.h \
	$$PWD/include/spikeKit/communication/commandConstants.h \
	$$PWD/include/spikeKit/communication/spikeDirectCommand.h \
	$$PWD/include/spikeKit/communication/spikeRobotCommunicationThread.h \
	$$PWD/include/spikeKit/communication/bluetoothRobotCommunicationThread.h \
	$$PWD/include/spikeKit/communication/usbRobotCommunicationThread.h \
	$$PWD/src/blocks/details/speakerBlock.h \
	$$PWD/src/blocks/details/beepBlock.h \
	$$PWD/src/blocks/details/playToneBlock.h \
	$$PWD/src/blocks/details/drawCircleBlock.h \
	$$PWD/src/blocks/details/drawLineBlock.h \
	$$PWD/src/blocks/details/drawPixelBlock.h \
	$$PWD/src/blocks/details/drawRectBlock.h \
	$$PWD/src/blocks/details/spikeEnginesBackwardBlock.h \
	$$PWD/src/blocks/details/spikeEnginesForwardBlock.h \
	$$PWD/src/blocks/details/ledBlock.h \
	$$PWD/src/blocks/details/spikeReadRGBBlock.h \

SOURCES += \
	$$PWD/src/robotModel/spikeRobotModelBase.cpp \
	$$PWD/src/robotModel/parts/spikeDisplay.cpp \
	$$PWD/src/robotModel/parts/spikeSpeaker.cpp \
	$$PWD/src/robotModel/parts/spikeMotor.cpp \
	$$PWD/src/robotModel/parts/spikeLed.cpp \
	$$PWD/src/robotModel/parts/spikeCompass.cpp \
	$$PWD/src/robotModel/parts/spikeDCIRSeeker.cpp \
	$$PWD/src/robotModel/parts/spikeACIRSeeker.cpp \
	$$PWD/src/robotModel/parts/spikeNXTColorSensorV2Color.cpp \
	$$PWD/src/robotModel/parts/spikeNXTColorSensorV2RGB.cpp \
	$$PWD/src/robotModel/parts/spikeNXTColorSensorV2Passive.cpp \
	$$PWD/src/robotModel/parts/spikeNXTColorSensorV2Raw.cpp \
	$$PWD/src/blocks/spikeBlocksFactory.cpp \
	$$PWD/src/blocks/details/speakerBlock.cpp \
	$$PWD/src/blocks/details/beepBlock.cpp \
	$$PWD/src/blocks/details/playToneBlock.cpp \
	$$PWD/src/blocks/details/drawCircleBlock.cpp \
	$$PWD/src/blocks/details/drawLineBlock.cpp \
	$$PWD/src/blocks/details/drawPixelBlock.cpp \
	$$PWD/src/blocks/details/drawRectBlock.cpp \
	$$PWD/src/blocks/details/spikeEnginesBackwardBlock.cpp \
	$$PWD/src/blocks/details/spikeEnginesForwardBlock.cpp \
	$$PWD/src/blocks/details/ledBlock.cpp \
	$$PWD/src/blocks/details/spikeReadRGBBlock.cpp \
	$$PWD/src/communication/spikeDirectCommand.cpp \
	$$PWD/src/communication/spikeRobotCommunicationThread.cpp \
	$$PWD/src/communication/bluetoothRobotCommunicationThread.cpp \
	$$PWD/src/communication/hidapi.c \
	$$PWD/src/communication/usbRobotCommunicationThread.cpp \
