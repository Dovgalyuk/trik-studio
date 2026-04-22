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

#include "spikeAdditionalPreferences.h"
#include "ui_spikeAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <utils/widgets/comPortPicker.h>

using namespace spike;
using namespace qReal;

SpikeAdditionalPreferences::SpikeAdditionalPreferences(QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::SpikeAdditionalPreferences)
{
	mUi->setupUi(this);
	mUi->robotImagePicker->configure("spikeRobot2DImage", tr("2D robot image:"));
	connect(mUi->manualComPortCheckbox, &QCheckBox::toggled
			, this, &SpikeAdditionalPreferences::manualComPortCheckboxChecked);
	connect(mUi->commonFolderChecbox, &QCheckBox::toggled
			, this, [this](bool state){ mUi->commonFolderNameLineEdit->setEnabled(state); });
}

SpikeAdditionalPreferences::~SpikeAdditionalPreferences()
{
	delete mUi;
}

void SpikeAdditionalPreferences::save()
{
	SettingsManager::setValue("SpikeBluetoothPortName", selectedPortName());
	SettingsManager::setValue("SpikeManualComPortCheckboxChecked", mUi->manualComPortCheckbox->isChecked());
	SettingsManager::setValue("SpikeCommonFolderChecboxChecked", mUi->commonFolderChecbox->isChecked());
	SettingsManager::setValue("SpikeCommonFolderName", mUi->commonFolderNameLineEdit->text());
	mUi->robotImagePicker->save();
	Q_EMIT settingsChanged();
}

void SpikeAdditionalPreferences::restoreSettings()
{
	ui::ComPortPicker::populate(*mUi->comPortComboBox, "SpikeBluetoothPortName");
	mUi->robotImagePicker->restore();

	if (mUi->comPortComboBox->count() == 0) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->manualComPortCheckbox->hide();
		mUi->noComPortsFoundLabel->show();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(SettingsManager::value("SpikeBluetoothPortName").toString());
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->manualComPortCheckbox->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();
		mUi->manualComPortCheckbox->setChecked(false);
		mUi->manualComPortCheckbox->setChecked(SettingsManager::value("SpikeManualComPortCheckboxChecked").toBool());
	}

	mUi->commonFolderChecbox->setChecked(SettingsManager::value("SpikeCommonFolderChecboxChecked", false).toBool());
	mUi->commonFolderNameLineEdit->setEnabled(mUi->commonFolderChecbox->isChecked());
	mUi->commonFolderNameLineEdit->setText(SettingsManager::value("SpikeCommonFolderName", "ts").toString());
}

void SpikeAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	mUi->bluetoothSettingsGroupBox->setVisible(robotModel->name().toLower().contains("bluetooth"));
}

void SpikeAdditionalPreferences::manualComPortCheckboxChecked(bool state)
{
	const QString defaultPortName = SettingsManager::value("SpikeBluetoothPortName").toString();

	if (state) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();
	}
}

QString SpikeAdditionalPreferences::selectedPortName() const
{
	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}
