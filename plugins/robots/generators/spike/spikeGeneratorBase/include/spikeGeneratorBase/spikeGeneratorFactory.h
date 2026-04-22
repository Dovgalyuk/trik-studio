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

#include <generatorBase/generatorFactoryBase.h>

#include "parts/spikeMailboxes.h"
#include "spikeGeneratorBase/spikeGeneratorBaseDeclSpec.h"

namespace spike {

class ROBOTS_SPIKE_GENERATOR_BASE_EXPORT SpikeGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
	Q_OBJECT
public:
	SpikeGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	~SpikeGeneratorFactory() override;

	/// Returns an entity processing everything about Mailboxes.
	parts::Mailboxes &mailboxes();

	generatorBase::simple::AbstractSimpleGenerator *ifGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer, bool elseIsEmpty, bool needInverting) override;

	generatorBase::simple::AbstractSimpleGenerator *forLoopGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	/// Converts an output port with given id into generator-dependent port representation.
	generatorBase::simple::Binding::ConverterInterface *outputPortNameConverter() const;

	/// Converts TRIK Studio LED color enum values to corresponding names in SPIKE runtime.
	generatorBase::simple::Binding::ConverterInterface *ledColorConverter() const;

	generatorBase::simple::Binding::ConverterInterface *colorConverter() const;

	QStringList pathsToTemplates() const override;

	void reportError(const QString &errorMessage, const qReal::Id &id);

private:
	const QString mGeneratorName;
	parts::Mailboxes mMailboxes;
};

}

