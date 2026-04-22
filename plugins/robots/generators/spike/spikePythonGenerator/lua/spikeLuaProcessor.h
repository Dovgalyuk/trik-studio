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

#include <generatorBase/lua/luaProcessor.h>

namespace generatorBase {
namespace parts {
class Variables;
}
}

namespace spike {
namespace python {
class SpikePythonGeneratorFactory;
namespace lua {
class SpikeLuaPrinter;

/// A toolbox for reengineering lua code. Prints lua code into the SPIKE bytecode.
class SpikeLuaProcessor : public generatorBase::lua::LuaProcessor
{
	Q_OBJECT

public:
	SpikeLuaProcessor(qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguage
			, const utils::ParserErrorReporter &parserErrorReporter
			, QObject *parent = nullptr);

	~SpikeLuaProcessor() override;

	/// Configures this processor with variables instance. Variables cannot be passed to constructor because it will
	/// cause cyclic dependencies between SpikeLuaProcessor and spikeGenratorBase::SpikeGeneratorCustomizer dueing the
	/// initialization.
	void configure(generatorBase::parts::Variables *variables, SpikePythonGeneratorFactory *factory);

	/// Converts the given Lua code into the SPIKE bytecode and substitues all
	/// reserved variables and functions code.
	/// Takes ownership on @arg reservedVariablesConverter.
	QString translate(const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

	/// Converts the given Lua code into the SPIKE bytecode, substitues all
	/// reserved variables and functions code and casts the result to string.
	/// Takes ownership on @arg reservedVariablesConverter.
	QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

	/// Returns code that initializes all known constants.
	QString constantsValuation() const;

	/// Returns code that initializes all known arrays.
	QString arraysInitialization() const;
private:
	void preparePrinter();

	generatorBase::parts::Variables *mVariables;
	SpikePythonGeneratorFactory *mFactory;
	QScopedPointer<SpikeLuaPrinter> mPrinter;
};

}
}
}
