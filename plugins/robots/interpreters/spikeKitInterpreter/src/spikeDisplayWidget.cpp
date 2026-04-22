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

#include "spikeDisplayWidget.h"

#include "ui_spikeDisplayWidget.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>

using namespace spike;

SpikeDisplayWidget::SpikeDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
	, mUi(new Ui::SpikeDisplayWidget)
	, mBackground(":/spike/interpreter/images/spike-brick.png", "PNG")
{
	mUi->setupUi(this);
}

SpikeDisplayWidget::~SpikeDisplayWidget()
{
	delete mUi;
}

bool SpikeDisplayWidget::buttonIsDown(const QString &buttonPort) const
{
	if (buttonPort == "Left") {
		return mUi->leftButton->isDown();
	} else if (buttonPort == "Right") {
		return mUi->rightButton->isDown();
	} else if (buttonPort == "Up") {
		return mUi->upButton->isDown();
	} else if (buttonPort == "Down") {
		return mUi->downButton->isDown();
	} else if (buttonPort == "Enter") {
		return mUi->enterButton->isDown();
	} else if (buttonPort == "Back") {
		return mUi->escapeButton->isDown();
	}

	QLOG_WARN() << "Button on port" << buttonPort << "is not supported by 2d model";

	return false;
}

void SpikeDisplayWidget::setPainter(qReal::ui::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
	mUi->display->setPainter(painter);
}

void SpikeDisplayWidget::repaintDisplay()
{
	mUi->display->repaint();
}

int SpikeDisplayWidget::displayWidth() const
{
	return mUi->display->width();
}

int SpikeDisplayWidget::displayHeight() const
{
	return mUi->display->height();
}

void SpikeDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
