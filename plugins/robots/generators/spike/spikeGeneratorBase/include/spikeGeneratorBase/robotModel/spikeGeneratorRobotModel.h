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

#include <spikeKit/robotModel/spikeRobotModelBase.h>
#include <QSharedPointer>
#include "spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h"

namespace spike {

namespace communication {
class SpikeRobotCommunicationThread;
}

namespace robotModel {

class ROBOTS_SPIKE_GENERATOR_BASE_EXPORT SpikeGeneratorRobotModel : public SpikeRobotModelBase
{
	Q_OBJECT

public:
	/// Does not take ownership over \a communicator.
	/// @param priority A priority of this model over other among their kit.
	SpikeGeneratorRobotModel(const QString &kitId, const QString &robotId
			, const QString &name, const QString &friendlyName, int priority
			, const QSharedPointer<communication::SpikeRobotCommunicationThread> &communicator);
	~SpikeGeneratorRobotModel();

	QString name() const override;
	QString friendlyName() const override;

	bool needsConnection() const override;
	bool interpretedModel() const override;

	int priority() const override;

	/// Returns a pointer to communication thread object of this robot.
	QSharedPointer<communication::SpikeRobotCommunicationThread> communicator();

private:
	const QString mName;
	const QString mFriendlyName;
	const int mPriority;
	const QSharedPointer<communication::SpikeRobotCommunicationThread> mCommunicator;
};

}
}
