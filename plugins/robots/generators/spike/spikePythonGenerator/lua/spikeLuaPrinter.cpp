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

#include "spikeLuaPrinter.h"

#include <qrkernel/logging.h>
#include <qrkernel/definitions.h>

#include <qrtext/lua/ast/number.h>
#include <qrtext/lua/ast/unaryMinus.h>
#include <qrtext/lua/ast/not.h>
#include <qrtext/lua/ast/bitwiseNegation.h>
#include <qrtext/lua/ast/length.h>
#include <qrtext/lua/ast/logicalAnd.h>
#include <qrtext/lua/ast/logicalOr.h>
#include <qrtext/lua/ast/addition.h>
#include <qrtext/lua/ast/subtraction.h>
#include <qrtext/lua/ast/multiplication.h>
#include <qrtext/lua/ast/division.h>
#include <qrtext/lua/ast/integerDivision.h>
#include <qrtext/lua/ast/modulo.h>
#include <qrtext/lua/ast/exponentiation.h>
#include <qrtext/lua/ast/bitwiseAnd.h>
#include <qrtext/lua/ast/bitwiseOr.h>
#include <qrtext/lua/ast/bitwiseXor.h>
#include <qrtext/lua/ast/bitwiseLeftShift.h>
#include <qrtext/lua/ast/bitwiseRightShift.h>
#include <qrtext/lua/ast/concatenation.h>
#include <qrtext/lua/ast/equality.h>
#include <qrtext/lua/ast/lessThan.h>
#include <qrtext/lua/ast/lessOrEqual.h>
#include <qrtext/lua/ast/inequality.h>
#include <qrtext/lua/ast/greaterThan.h>
#include <qrtext/lua/ast/greaterOrEqual.h>
#include <qrtext/lua/ast/integerNumber.h>
#include <qrtext/lua/ast/floatNumber.h>
#include <qrtext/lua/ast/fieldInitialization.h>
#include <qrtext/lua/ast/tableConstructor.h>
#include <qrtext/lua/ast/string.h>
#include <qrtext/lua/ast/true.h>
#include <qrtext/lua/ast/false.h>
#include <qrtext/lua/ast/nil.h>
#include <qrtext/lua/ast/identifier.h>
#include <qrtext/lua/ast/functionCall.h>
#include <qrtext/lua/ast/methodCall.h>
#include <qrtext/lua/ast/assignment.h>
#include <qrtext/lua/ast/block.h>
#include <qrtext/lua/ast/indexingExpression.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>
#include <qrtext/lua/types/boolean.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/table.h>

using namespace spike::python::lua;

const QMap<SpikePythonType, QString> registerNames = {  // clazy:exclude=non-pod-global-static
	{ SpikePythonType::data8, "_bool_temp_result_" }
	, { SpikePythonType::data16, "_small_int_temp_result_" }
	, { SpikePythonType::data32, "_int_temp_result_" }
	, { SpikePythonType::dataF, "_float_temp_result_" }
	, { SpikePythonType::dataS, "_string_temp_result_" }
	, { SpikePythonType::array8, "_array8_temp_result_" }
	, { SpikePythonType::array16, "_array16_temp_result_" }
	, { SpikePythonType::array32, "_array32_temp_result_" }
	, { SpikePythonType::arrayF, "_arrayF_temp_result_" }
};

const QMap<SpikePythonType, QString> typeNames = {  // clazy:exclude=non-pod-global-static
	{ SpikePythonType::data8, "8" }
	, { SpikePythonType::data16, "16" }
	, { SpikePythonType::data32, "32" }
	, { SpikePythonType::dataF, "F" }
	, { SpikePythonType::dataS, "S" }
};

SpikeLuaPrinter::SpikeLuaPrinter(const QStringList &pathsToTemplates
		, const qrtext::LanguageToolboxInterface &textLanguage
		, generatorBase::parts::Variables &variables)
	: generatorBase::TemplateParametrizedEntity(addSuffix(pathsToTemplates))
	, mTextLanguage(textLanguage)
	, mVariables(variables)
	, mReservedFunctionsConverter(pathsToTemplates)
{
	mArrayDeclarationCount[SpikePythonType::array8] = 0;
	mArrayDeclarationCount[SpikePythonType::array16] = 0;
	mArrayDeclarationCount[SpikePythonType::array32] = 0;
	mArrayDeclarationCount[SpikePythonType::arrayF] = 0;
}

SpikeLuaPrinter::~SpikeLuaPrinter() = default;

void SpikeLuaPrinter::configure(const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	mReservedVariablesConverter = reservedVariablesConverter;
}

QStringList SpikeLuaPrinter::addSuffix(const QStringList &list)
{
	QStringList result;
	result.reserve(list.size());
	for (const QString &path : list) {
		result << path + "/luaPrinting";
	}

	return result;
}

SpikePythonType SpikeLuaPrinter::toSpikeType(const QSharedPointer<qrtext::core::types::TypeExpression> &type)
{
	if (type->is<qrtext::lua::types::Boolean>()) {
		return SpikePythonType::data8;
	}

	if (type->is<qrtext::lua::types::Integer>()) {
		return SpikePythonType::data32;
	}

	if (type->is<qrtext::lua::types::Float>()) {
		return SpikePythonType::dataF;
	}

	if (type->is<qrtext::lua::types::String>()) {
		return SpikePythonType::dataS;
	}

	if (type->is<qrtext::lua::types::Table>()) {
		const QSharedPointer<qrtext::core::types::TypeExpression> &tableType
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
			= static_cast<qrtext::lua::types::Table *>(type.data())->elementType();

		if (tableType->is<qrtext::lua::types::Boolean>()) {
			return SpikePythonType::array8;
		}

		if (tableType->is<qrtext::lua::types::Integer>()) {
			return SpikePythonType::array32;
		}

		if (tableType->is<qrtext::lua::types::Float>()) {
			return SpikePythonType::arrayF;
		}
	}

	qWarning() << "SpikeLuaPrinter::typeOf: Unsupported type" << qUtf8Printable(type->toString());
	return SpikePythonType::other;
}

SpikePythonType SpikeLuaPrinter::typeOf(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return toSpikeType(mTextLanguage.type(node));
}

bool SpikeLuaPrinter::isArray(SpikePythonType type) const
{
	return type == SpikePythonType::array8
			|| type == SpikePythonType::array16
			|| type == SpikePythonType::array32
			|| type == SpikePythonType::arrayF;
}

SpikePythonType SpikeLuaPrinter::elementType(SpikePythonType arrayType) const
{
	switch (arrayType) {
	case SpikePythonType::array8:
		return SpikePythonType::data8;
	case SpikePythonType::array16:
		return SpikePythonType::data16;
	case SpikePythonType::array32:
		return SpikePythonType::data32;
	case SpikePythonType::arrayF:
		return SpikePythonType::dataF;
	default:
		return SpikePythonType::other;
	}
}

QString SpikeLuaPrinter::newRegister(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return newRegister(typeOf(node));
}

QString SpikeLuaPrinter::newRegister(SpikePythonType type)
{
	if (type == SpikePythonType::other) {
		return {};
	}

	QString result {};
	if (isArray(type)) {
		++mArrayDeclarationCount[type];
		result = registerNames[type] + QString::number(mArrayDeclarationCount[type]);
	} else {
		result = registerNames[type] + QString::number(++mRegistersCount[mId][type]);
	}

	const QString declarationTemplate = (type == SpikePythonType::dataS)
			? "DATA%1 %2 255"
			: (isArray(type) ? QString("HANDLE %2").arg(result): "DATA%1 %2");
	mVariables.appendManualDeclaration(declarationTemplate.arg(typeNames[type], result));
	return result;
}

QString SpikeLuaPrinter::print(const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id)
{
	mId = id;
	mAdditionalCode.clear();
	return printWithoutPop(node) ? popResult(node) : QString();
}

QString SpikeLuaPrinter::castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
		, const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id)
{
	mId = id;
	mAdditionalCode.clear();
	return printWithoutPop(node) ? castTo(toSpikeType(type), node) : QString();
}

QStringList SpikeLuaPrinter::additionalCode(const QSharedPointer<qrtext::core::ast::Node> &node) const
{
	return mAdditionalCode[node.data()];
}

QString SpikeLuaPrinter::constantsEvaluation() const
{
	QStringList code;
	for (const QString &constantName : mTextLanguage.specialConstants()) {
		const QString value = mTextLanguage.value<QString>(constantName);
		/// @todo: Rewrite this sh..t
		const SpikePythonType type = value.contains(".") ? SpikePythonType::dataF : SpikePythonType::data32;
		const QString typeName = typeNames[type];
		code << readTemplate("assignment.t")
				.replace("@@TYPE1@@", typeName)
				.replace("@@TYPE2@@", typeName)
				.replace("@@VARIABLE@@", constantName)
				.replace("@@VALUE@@", type == SpikePythonType::dataF ? value + "F" : value);
	}

	return code.join("\n");
}

QString SpikeLuaPrinter::arraysEvaluation()
{
	QStringList code;
	const QString tableConstuctorTemplate = readTemplate("tableConstructor.t");
	QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression> > variables
			= mTextLanguage.variableTypes();
	for (auto it = variables.cbegin(); it != variables.cend(); it++) {
		const auto &arrayVariable = it.key();
		if (variables[arrayVariable].data()->is<qrtext::lua::types::Table>()
				&& !mTextLanguage.specialIdentifiers().contains(arrayVariable)) {
			SpikePythonType spikeType = toSpikeType(
					dynamic_cast<qrtext::lua::types::Table *>(variables[arrayVariable].data())->elementType());
			QString constuctorTemplate = tableConstuctorTemplate;
			code << constuctorTemplate.replace("@@TYPE@@", typeNames[spikeType]).replace("@@RESULT@@", arrayVariable);
		}
	}

	for (auto it = mArrayDeclarationCount.cbegin(); it != mArrayDeclarationCount.cend(); it++) {
		const auto &arrayType = it.key();
		for (int i = 1; i <= mArrayDeclarationCount[arrayType]; ++i) {
			QString constuctorTemplate = tableConstuctorTemplate;
			code << constuctorTemplate
					.replace("@@TYPE@@", typeNames[elementType(arrayType)])
					.replace("@@RESULT@@", registerNames[arrayType] + QString::number(i));
		}
	}

	return code.join("\n");
}

void SpikeLuaPrinter::pushResult(const QSharedPointer<qrtext::lua::ast::Node> &node
		, const QString &generatedCode, const QString &additionalCode)
{
	mGeneratedCode[node.data()] = generatedCode;
	pushChildrensAdditionalCode(node);
	if (!additionalCode.isEmpty()) {
		mAdditionalCode[node.data()] << additionalCode;
	}
}

QString SpikeLuaPrinter::popResult(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return mGeneratedCode.take(node.data());
}

QStringList SpikeLuaPrinter::popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes)
{
	QStringList result;
	for (const QSharedPointer<qrtext::lua::ast::Node> &node : nodes) {
		result << popResult(node);
	}

	return result;
}

void SpikeLuaPrinter::pushChildrensAdditionalCode(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	for (auto &&child : node->children()) {
		mAdditionalCode[node.data()] += mAdditionalCode.take(child.data());
	}
}

bool SpikeLuaPrinter::printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	if (!node) {
		return false;
	}

	node->acceptRecursively(*this, node, qrtext::wrap(nullptr));
	if (mGeneratedCode.count() != 1 || mGeneratedCode.firstKey() != node.data()) {
		QLOG_WARN() << "Lua printer got into the inconsistent state during printing."
				<< mGeneratedCode.count() << "pieces of code:";
		for (auto it = mGeneratedCode.cbegin(); it != mGeneratedCode.cbegin(); it++) {
			QLOG_INFO() << it.value();
		}

		mGeneratedCode.clear();
		return false;
	}

	return true;
}

void SpikeLuaPrinter::processTemplate(const QSharedPointer<qrtext::lua::ast::Node> &node
		, const QString &templateFileName
		, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings
		, const QMap<QString, QString> &staticBindings)
{
	QString computation = readTemplate(templateFileName);
	const bool typedComputation = computation.contains("@@RESULT@@");
	QString result;
	if (typedComputation) {
		result = newRegister(node);
		computation.replace("@@RESULT@@", result);
	}

	for (auto it = bindings.cbegin(); it != bindings.cend(); it++) {
		const auto &toReplace = it.key();
		computation.replace(toReplace, popResult(bindings[toReplace]));
	}

	for (auto it = staticBindings.cbegin(); it != staticBindings.cend(); it++) {
		const auto &toReplace = it.key();
		computation.replace(toReplace, staticBindings[toReplace]);
	}

	pushResult(node, result, computation);
}

void SpikeLuaPrinter::processUnary(const QSharedPointer<qrtext::core::ast::UnaryOperator> &node
		, const QString &templateFileName)
{
	const SpikePythonType type = typeOf(node);
	QString templateCode = readTemplate(templateFileName);
	const bool hasAdditionalCode = templateCode.contains("@@RESULT@@");
	const QString result = hasAdditionalCode ? newRegister(type) : QString();
	const QString code = templateCode
			.replace("@@TYPE@@", typeNames[type])
			.replace("@@OPERAND@@", popResult(node->operand()))
			.replace("@@RESULT@@", result);
	pushResult(node, hasAdditionalCode ? result : code, hasAdditionalCode ? code : QString());
}

void SpikeLuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, SpikePythonType operandsType, SpikePythonType resultType, const QString &templateFileName)
{
	const QString result = newRegister(resultType);
	pushResult(node, result, readTemplate(templateFileName)
			.replace("@@TYPE@@", typeNames[operandsType])
			.replace("@@RESULT@@", result)
			.replace("@@LEFT@@", castTo(operandsType, node->leftOperand()))
			.replace("@@RIGHT@@", castTo(operandsType, node->rightOperand())));
}

void SpikeLuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, SpikePythonType resultType, const QString &templateFileName)
{
	const SpikePythonType operandsType = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	processBinary(node, operandsType, resultType, templateFileName);
}

void SpikeLuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, const QString &templateFileName)
{
	const SpikePythonType type = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	processBinary(node, type, type, templateFileName);
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Number> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation(), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "unaryMinus.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Not> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "not.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "bitwiseNegation.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Length> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "length.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, SpikePythonType::data8, "logicalAnd.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, SpikePythonType::data8, "logicalOr.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Addition> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "addition.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "subtraction.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "multiplication.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Division> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::dataF, SpikePythonType::dataF, "division.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "integerDivision.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Modulo> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "modulo.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::dataF, SpikePythonType::dataF, "exponentiation.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "bitwiseAnd.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "bitwiseOr.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "bitwiseXor.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "bitwiseLeftShift.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data32, SpikePythonType::data32, "bitwiseRightShift.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString result = newRegister(node);
	pushResult(node, result, readTemplate("concatenation.t")
			.replace("@@RESULT@@", result)
			.replace("@@LEFT@@", toString(node->leftOperand()))
			.replace("@@RIGHT@@", toString(node->rightOperand())));
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Equality> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const SpikePythonType operandsType = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	if (operandsType == SpikePythonType::dataS) {
		processBinary(node, SpikePythonType::data8, "equalityString.t");
	} else {
		processBinary(node, SpikePythonType::data8, "equality.t");
	}
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LessThan> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, "lessThan.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, "lessOrEqual.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Inequality> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const SpikePythonType operandsType = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	if (operandsType == SpikePythonType::dataS) {
		processBinary(node, SpikePythonType::data8, "inequalityString.t");
	} else {
		processBinary(node, SpikePythonType::data8, "inequality.t");
	}
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, "greaterThan.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, SpikePythonType::data8, "greaterOrEqual.t");
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IntegerNumber> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation(), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FloatNumber> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation() + "F", QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FieldInitialization> &node
		, const QSharedPointer<qrtext::core::ast::Node> &parent)
{
	QString value;
	const auto &tableType = mTextLanguage.type(parent);
	QString indexerType = typeNames[SpikePythonType::data32];
	if (const auto *table = dynamic_cast<qrtext::lua::types::Table *>(tableType.data())) {
		const auto spikeTableType = toSpikeType(table->elementType());
		value = castTo(spikeTableType, node->value());
		indexerType = typeNames[spikeTableType];
	} else {
		value = popResult(node->value());
	}

	const QString initializer = readTemplate("writeIndexer.t")
			.replace("@@INDEX@@", node->key() ? popResult(node->key()) : QString::number(++mTableInitializersCount))
			.replace("@@VALUE@@", value)
			.replace("@@SPIKE_TYPE@@", indexerType);
	pushResult(node, initializer, QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::TableConstructor> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	mTableInitializersCount = -1;
	QStringList initializers = popResults(qrtext::as<qrtext::lua::ast::Node>(node->initializers()));
	const QString result = newRegister(node);
	for (int i = 0; i < initializers.count(); ++i) {
		initializers[i].replace("@@TABLE@@", result);
	}

	// all arrays(tables) for SPIKE are created in array initialisation section
	// here we only should process initializers
	pushResult(node, result, initializers.join("\n"));
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::String> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("string.t").replace("@@VALUE@@", node->string()), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::True> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("true.t"), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::False> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("false.t"), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Nil> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("nil.t"), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Identifier> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	QString additionalCode;
	QString result = mReservedVariablesConverter->convert(node->name());
	if (result != node->name()) {
		const QString registerName = newRegister(node);
		additionalCode = result.replace("@@RESULT@@", registerName);
		result = registerName;
	}

	pushResult(node, result, additionalCode);
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FunctionCall> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString expression = popResult(node->function());

	const QStringList unused = {"print"};
	const QStringList int32Functions = {"time", "sgn"};
	const QStringList shouldCastToIntAfterFunctions = {"ceil", "floor"};
	bool shouldCastToIntAfter = false;

	QStringList arguments;
	QString reservedFunctionCall;
	SpikePythonType type = SpikePythonType::dataF;
	QStringList additionalResults;
	QString nodeName;
	if (auto idNode = dynamic_cast<qrtext::lua::ast::Identifier *>(node->function().data())) {
		nodeName = idNode->name();
		if (unused.contains(nodeName)) {
			arguments = popResults(qrtext::as<qrtext::lua::ast::Node>(node->arguments()));
		} else {
			shouldCastToIntAfter = shouldCastToIntAfterFunctions.contains(nodeName);
			if (int32Functions.contains(nodeName)) {
				type = SpikePythonType::data32;
			}

			// All arguments are considered float here due to bytecode spec.
			for (auto &argument : node->arguments()) {
				arguments << castTo(SpikePythonType::dataF, qrtext::as<qrtext::lua::ast::Node>(argument));
			}

			if (mReservedFunctionsConverter.isCosOrSin(nodeName)) {
				for (auto &&argument : arguments) {
					const auto &oldValue = argument;
					const auto &argChanged = newRegister(SpikePythonType::dataF);
					additionalResults << QString("MULF(%1, %2, %3)").arg(oldValue, "57.29577951308232F", argChanged);
					argument = argChanged;
				}
			}

			reservedFunctionCall = addRandomIds(mReservedFunctionsConverter.convert(nodeName, arguments));
		}
	}

	const QString functionResult = newRegister(type);
	QString result = shouldCastToIntAfter ? newRegister(SpikePythonType::data32) : functionResult;

	if (reservedFunctionCall.isEmpty()) {
		pushResult(node, result, readTemplate("functionCall.t")
				.replace("@@FUNCTION@@", expression)
				.replace("@@ARGUMENTS@@", arguments.join(readTemplate("argumentsSeparator.t")))
				.replace("@@RESULT@@", functionResult));
	} else {
		additionalResults << reservedFunctionCall.replace("@@RESULT@@", functionResult);
		pushResult(node, result, additionalResults.join("\n"));
		if (shouldCastToIntAfter) {
			mAdditionalCode[node.data()] << QString("MOVEF_32(%1, %2)").arg(functionResult, result);
		}
	}
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::MethodCall> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString object = popResult(node->object());
	const QString method = popResult(node->methodName());
	const QStringList arguments = popResults(qrtext::as<qrtext::lua::ast::Node>(node->arguments()));
	pushResult(node, readTemplate("methodCall.t")
			.replace("@@OBJECT@@", object)
			.replace("@@METHOD@@", method)
			.replace("@@ARGUMENTS@@", arguments.join(readTemplate("argumentsSeparator.t"))), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Assignment> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	if (dynamic_cast<qrtext::lua::ast::IndexingExpression *>(node->variable().data())) {
		// We are dealing with write-indexer here. Template for result is already ready,
		// we just need to replace @@VALUE@@ there.
		// PopResult returns mGeneratedCode, but in case of SPIKE there is no expression like x[E1] = E2
		// So we need to look at additional code, which contains E1 evaluation for variable, and
		// E2 evaluation for value.
		QString writeTemplate;
		if (!mAdditionalCode[node->variable().data()].isEmpty()) {
			writeTemplate = mAdditionalCode[node->variable().data()].last();
			mAdditionalCode[node->variable().data()].pop_back();
		}

		const auto indexerType = typeNames[typeOf(node->variable())];
		writeTemplate.replace("@@SPIKE_TYPE@@", indexerType);
		QString value = castTo(typeOf(node->variable()), node->value());
		popResults({node->value(), node->variable()});
		pushResult(node, writeTemplate.replace("@@VALUE@@", value), QString());
		return;
	}

	const SpikePythonType type = typeOf(node->variable());
	const bool isString = type == SpikePythonType::dataS;
	const bool isArray = this->isArray(type);
	processTemplate(node, isString ? "assignmentStrings.t" : (isArray ? "assignmentArrays.t" : "assignment.t")
			, { {"@@VARIABLE@@", node->variable()}, {"@@VALUE@@", node->value()} }
			, { {"@@TYPE1@@", typeNames[typeOf(node->variable())]}
			,   {"@@TYPE2@@", typeNames[typeOf(node->value())]} }
	);
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Block> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QStringList expressions = popResults(node->children());
	pushResult(node, expressions.join(readTemplate("statementsSeparator.t")), QString());
}

void SpikeLuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IndexingExpression> &node
		, const QSharedPointer<qrtext::core::ast::Node> &parent)
{
	bool isWriteIndexer = false;
	auto assignment = dynamic_cast<qrtext::lua::ast::Assignment *>(parent.data());
	if (assignment) {
		isWriteIndexer = assignment->variable() == node;
	}

	const QString indexerTemplate = isWriteIndexer ? "writeIndexer.t" : "readIndexer.t";
	processTemplate(node, indexerTemplate, { {"@@TABLE@@", node->table()}, {"@@INDEX@@", node->indexer()} });
}

QString SpikeLuaPrinter::toString(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	const QSharedPointer<qrtext::core::types::TypeExpression> type = mTextLanguage.type(node);
	auto value = popResult(node);
	if (type->is<qrtext::lua::types::String>()) {
		return value;
	}

	QString code;
	if (type->is<qrtext::lua::types::Integer>()) {
		code = readTemplate("intToString.t").replace("@@VALUE@@", value);
	} else if (type->is<qrtext::lua::types::Float>()) {
		code = readTemplate("floatToString.t").replace("@@VALUE@@", value);
	} else if (type->is<qrtext::lua::types::Boolean>()) {
		code = readTemplate("boolToString.t").replace("@@VALUE@@", value);
	} else {
		code = readTemplate("otherToString.t").replace("@@VALUE@@", value);
	}

	auto result = newRegister(SpikePythonType::dataS);
	pushChildrensAdditionalCode(node);
	mAdditionalCode[node.data()] << code.replace("@@RESULT@@", result);
	return result;
}

QString SpikeLuaPrinter::castTo(SpikePythonType targetType, const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	const SpikePythonType actualType = typeOf(node);
	if (targetType == SpikePythonType::dataS) {
		return toString(node);
	}

	auto value = popResult(node);
	if (actualType == targetType) {
		return value;
	}

	if (actualType == SpikePythonType::dataS) {
		return QObject::tr("/* Warning: cast from string to numeric type is not supported */ 0");
	}

	if (isArray(actualType)) {
		return QObject::tr("/* Warning: autocast from array to other type is not supported */ 0");
	}

	if (targetType == SpikePythonType::other || actualType == SpikePythonType::other) {
		return QObject::tr("/* Warning: autocast is supported only for numeric types */ 0");
	}

	auto result = newRegister(targetType);
	pushChildrensAdditionalCode(node);
	mAdditionalCode[node.data()] << QString("MOVE%1_%2(%3, %4)")
			.arg(typeNames[actualType], typeNames[targetType], value, result);
	return result;
}

SpikePythonType SpikeLuaPrinter::moreCommon(SpikePythonType type1, SpikePythonType type2) const
{
	return static_cast<SpikePythonType>(qMax(static_cast<SpikePythonType>(type1), static_cast<SpikePythonType>(type2)));
}
