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

TARGET = robots-spike-python-generator

TEMPLATE = lib

include(../../../../../global.pri)

QT += widgets

CONFIG += plugin trikqscintilla2

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/spike/spikeGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/spikeKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrrepo qrgui-text-editor qrtext \
		robots-generator-base robots-spike-generator-base robots-spike-kit robots-kit-base robots-utils)

HEADERS += \
	$$PWD/spikePythonGeneratorPlugin.h \
	$$PWD/spikePythonMasterGenerator.h \
	$$PWD/spikePythonGeneratorCustomizer.h \
	$$PWD/spikePythonGeneratorFactory.h \
	$$PWD/lua/spikeLuaProcessor.h \
	$$PWD/lua/spikeLuaPrinter.h \
	$$PWD/simpleGenerators/prependedCodeGenerator.h \

SOURCES += \
	$$PWD/spikePythonGeneratorPlugin.cpp \
	$$PWD/spikePythonMasterGenerator.cpp \
	$$PWD/spikePythonGeneratorCustomizer.cpp \
	$$PWD/spikePythonGeneratorFactory.cpp \
	$$PWD/lua/spikeLuaProcessor.cpp \
	$$PWD/lua/spikeLuaPrinter.cpp \
	$$PWD/simpleGenerators/prependedCodeGenerator.cpp \

RESOURCES += \
	$$PWD/spikePythonGenerator.qrc \
	$$PWD/templates.qrc \
