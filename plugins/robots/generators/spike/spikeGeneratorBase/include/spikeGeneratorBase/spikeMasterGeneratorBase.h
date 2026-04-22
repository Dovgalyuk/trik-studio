/* Copyright 2007-2017 CyberTech Labs Ltd.
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

#include <generatorBase/masterGeneratorBase.h>

#include "spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h"

namespace spike {

/// Master generator base implementation for every generator from Spike kit
class ROBOTS_SPIKE_GENERATOR_BASE_EXPORT SpikeMasterGeneratorBase : public generatorBase::MasterGeneratorBase
{
	Q_OBJECT
public:
	SpikeMasterGeneratorBase(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	void beforeGeneration() override;
	void processGeneratedCode(QString &code) override;

	const QString mGeneratorName;
};

}
