/* Copyright 2015 CyberTech Labs Ltd.
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

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>

namespace spike {
namespace communication {

/// Interface for SPIKE communicators. Must be able to upload, start and stop robots to SPIKE brick.
class SpikeRobotCommunicationThread : public utils::robotCommunication::RobotCommunicationThreadInterface
{
	Q_OBJECT

public:
	virtual ~SpikeRobotCommunicationThread();

public Q_SLOTS:
	/// Uploads file on the local machine to a remote device via Bluetooth.
	/// @returns path to uploaded file on SPIKE brick if it was uploaded successfully or empty string otherwise.
	virtual QString uploadFile(const QString &sourceFile, const QString &targetDir);

	/// Starts program execution on SPIKE brick. Does not upload the program itself.
	virtual bool runProgram(const QString &pathOnRobot = QString());

	/// Stops currently executing program in SPIKE brick.
	virtual void stopProgram();

protected:
	/// Must be reimplemented in each thread just to send the given buffer.
	/// Returns false on error
	virtual bool send1(const QByteArray &buffer) const = 0;

	/// Must be reimplemented in each thread just to recieve the buffer of the given size.
	virtual QByteArray receive(int size) const = 0;

	/// Forth running counter.
	quint16 mMessageCounter {0};

	/// Maximum filename length to upload to SPIKE
	static const int SPIKE_MAX_ALLOWED_FILENAME_LENGTH = 27;
};

template<typename T> char charOf(T x)
{
	return static_cast<char>(static_cast<uchar>(x));
}

}
}
