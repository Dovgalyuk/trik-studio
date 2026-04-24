/* Copyright 2016 CyberTech Labs Ltd.
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

#include "spikePythonGeneratorPlugin.h"

#include <spikeKit/blocks/spikeBlocksFactory.h>
#include <spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h>

using namespace spike;
using namespace spike::python;
using namespace kitBase::robotModel;

SpikePythonGeneratorPlugin::SpikePythonGeneratorPlugin()
	: SpikePythonGeneratorPluginBase()
	// , mModel(static_cast<robotModel::SpikeGeneratorRobotModel *>(robotModels().first()))
{
}

void SpikePythonGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	SpikePythonGeneratorPluginBase::init(configurator);
	// addShellDevice(*mModel);
}
