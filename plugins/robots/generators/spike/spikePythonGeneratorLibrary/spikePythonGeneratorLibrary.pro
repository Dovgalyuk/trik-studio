# Copyright 2016 CyberTech Labs Ltd.
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

TARGET = robots-spike-python-generator-library

TEMPLATE = lib

include(../../../../../global.pri)

CONFIG += trikqscintilla2

QT += widgets network

includes(plugins/robots/generators/spike/spikeGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/spikeKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrrepo robots-generator-base robots-spike-generator-base \
		robots-kit-base robots-spike-kit robots-utils qrgui-text-editor \
)

HEADERS += \
	$$PWD/include/spikePythonGeneratorLibrary/spikePythonGeneratorPluginBase.h \
	$$PWD/src/spikePythonMasterGenerator.h \
	$$PWD/src/spikePythonControlFlowValidator.h \
	$$PWD/src/threadsValidator.h \
	$$PWD/src/emptyShell.h \

SOURCES += \
	$$PWD/src/spikePythonGeneratorPluginBase.cpp \
	$$PWD/src/spikePythonMasterGenerator.cpp \
	$$PWD/src/spikePythonControlFlowValidator.cpp \
	$$PWD/src/threadsValidator.cpp \
	$$PWD/src/emptyShell.cpp \

RESOURCES = \
	$$PWD/spikePythonGeneratorLibrary.qrc \
	$$PWD/templates.qrc \
