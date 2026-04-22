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

#include "spikeLuaProcessor.h"

#include <qrkernel/logging.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "spikePythonGeneratorFactory.h"
#include "spikeLuaPrinter.h"

using namespace spike::python::lua;
using namespace qReal;

SpikeLuaProcessor::SpikeLuaProcessor(qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, QObject *parent)
	: generatorBase::lua::LuaProcessor(errorReporter, textLanguage, parserErrorReporter, parent)
	, mVariables(nullptr)
	, mFactory(nullptr)
	, mPrinter(nullptr)
{
}

SpikeLuaProcessor::~SpikeLuaProcessor()
{
}

void SpikeLuaProcessor::configure(generatorBase::parts::Variables *variables, SpikePythonGeneratorFactory *factory)
{
	mVariables = variables;
	mFactory = factory;
	mPrinter.reset(new SpikeLuaPrinter(pathsToRoot(), mTextLanguage, *mVariables));
}

QString SpikeLuaProcessor::translate(const QString &data
		, const Id &id
		, const QString &propertyName
		, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	if (!mVariables) {
		qWarning() << "Variables instance is null in SpikeLuaProcessor. Something went wrong.";
		QLOG_ERROR() << "Variables instance is null in SpikeLuaProcessor. Something went wrong.";
		return QString();
	}

	mTextLanguage.setNeedGeneralization(false);
	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	mPrinter->configure(reservedVariablesConverter);
	const QString result = mPrinter->print(tree, id);
	mFactory->addAdditionalCode(id, mPrinter->additionalCode(tree));
	return result;
}


QString SpikeLuaProcessor::castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
		, const QString &data
		, const Id &id
		, const QString &propertyName
		, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	if (!mVariables) {
		qWarning() << "Variables instance is null in SpikeLuaProcessor. Something went wrong.";
		QLOG_ERROR() << "Variables instance is null in SpikeLuaProcessor. Something went wrong.";
		return QString();
	}

	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	mPrinter->configure(reservedVariablesConverter);
	const QString result = mPrinter->castTo(type, tree, id);
	mFactory->addAdditionalCode(id, mPrinter->additionalCode(tree));
	return result;
}

QString SpikeLuaProcessor::constantsValuation() const
{
	return mPrinter->constantsEvaluation();
}

QString SpikeLuaProcessor::arraysInitialization() const
{
	return mPrinter->arraysEvaluation();
}
