/* Copyright 2017 QReal Research Group
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

#include "spikeGeneratorBase/spikeBlocksValidator.h"

using namespace spike;

SpikeBlocksValidator::SpikeBlocksValidator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::ErrorReporterInterface &errorReporter)
	: RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
{
}

bool SpikeBlocksValidator::validate(const qReal::Id &startNode)
{
	mNoErrors = true;
	startSearch(startNode);
	return mNoErrors;
}

void SpikeBlocksValidator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitFinal(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void SpikeBlocksValidator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitLoop(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitPreconditionalLoop(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void SpikeBlocksValidator::visitFork(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void SpikeBlocksValidator::visitJoin(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void SpikeBlocksValidator::visitGeneral(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	const QSet<QString> blocksWithPossiblyEmptyProperties{
		"CommentBlock"
	};

	if (!blocksWithPossiblyEmptyProperties.contains(id.element())) {
		checkStringPropertiesNotEmpty(id);
	}
}

void SpikeBlocksValidator::checkStringPropertiesNotEmpty(const qReal::Id &id)
{
	// Properties that we don't want to check for some reason. For example, on some old saves function has property
	// "Init", which is long removed from metamodel but still shows up in saves.
	const QSet<QString> badProperties{
		"Init"
	};

	const auto properties = mRepo.properties(id);
	for (auto const &property : properties.keys()) {
		if (badProperties.contains(property)) {
			continue;
		}

		if (properties[property].type() == QVariant::String &&  properties[property].toString().isEmpty()) {
			error(QObject::tr("Property should not be empty."), id);
		}
	}
}

void SpikeBlocksValidator::error(const QString &message, const qReal::Id &id)
{
	mErrorReporter.addError(message, id);
	mNoErrors = false;
}
