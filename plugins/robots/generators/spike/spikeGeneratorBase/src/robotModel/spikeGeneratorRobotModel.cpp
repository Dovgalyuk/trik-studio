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

#include "spikeGeneratorBase/robotModel/spikeGeneratorRobotModel.h"

using namespace spike::robotModel;

SpikeGeneratorRobotModel::SpikeGeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName
		, int priority)
	: SpikeRobotModel(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mPriority(priority)
{
}

QString SpikeGeneratorRobotModel::name() const
{
	return mName;
}

QString SpikeGeneratorRobotModel::friendlyName() const
{
	return mFriendlyName;
}

bool SpikeGeneratorRobotModel::needsConnection() const
{
	return false;
}

bool SpikeGeneratorRobotModel::interpretedModel() const
{
	return false;
}

int SpikeGeneratorRobotModel::priority() const
{
	return mPriority;
}

void SpikeGeneratorRobotModel::addDevice(const kitBase::robotModel::PortInfo &port
		, kitBase::robotModel::robotParts::Device *device)
{
	mPreConfiguredDevices[port] = device;
	configureDevice(port, device->deviceInfo());
	applyConfiguration();
}

kitBase::robotModel::robotParts::Device *SpikeGeneratorRobotModel::createDevice(
		const kitBase::robotModel::PortInfo &port
		, const kitBase::robotModel::DeviceInfo &deviceInfo)
{
	if (mPreConfiguredDevices.contains(port) && mPreConfiguredDevices[port]->deviceInfo().isA(deviceInfo)) {
		return mPreConfiguredDevices[port];
	}

	return SpikeRobotModel::createDevice(port, deviceInfo);
}
