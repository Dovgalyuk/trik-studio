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

#pragma once

#include <QtCore/QScopedPointer>

#include <generatorBase/robotsGeneratorPluginBase.h>

#include "spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h"

namespace spike {

namespace robotModel {
class SpikeGeneratorRobotModel;
}

/// A base class for every generator from the SPIKE kit.
class ROBOTS_SPIKE_GENERATOR_BASE_EXPORT SpikeGeneratorPluginBase : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT

public:
	SpikeGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
			, const QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> &blocksFactory
			);

	~SpikeGeneratorPluginBase() override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;

	/// Provides access to robot model to descendant classes.
	kitBase::robotModel::RobotModelInterface &robotModel() const;

private:
	/// Robot model that is used to query information about various robot devices.
	QScopedPointer<kitBase::robotModel::RobotModelInterface> mRobotModel;

	QSharedPointer<kitBase::blocksBase::BlocksFactoryInterface> mBlocksFactory;
};

}
