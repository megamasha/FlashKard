#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"
#include "flashcard.h" //for defines
#include "cardpack.h" //for defines

preferencesWindow::preferencesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferencesWindow)
{
    ui->setupUi(this);

    //testing section
    settings.beginGroup("Testing");
    ui->caseSensitiveAnswersBox->setChecked(
                settings.value("caseSensitiveAnswers",true).toBool());
    ui->timedAnswersBox->setChecked(
                settings.value("timedAnswers",false).toBool());
    ui->popupModeBox->setChecked(
                settings.value("popupMode",false).toBool());
    on_popupModeBox_toggled(ui->popupModeBox->isChecked());
    if (ui->popupModeBox->isChecked())
        ui->popupIntervalSpinBox->setValue(
                settings.value("popupInterval",DEFAULT_POPUP_INTERVAL).toInt());
    else
        ui->popupIntervalSpinBox->setValue(DEFAULT_POPUP_INTERVAL);
    settings.endGroup();

    //scoring section
    settings.beginGroup("Scoring");
    ui->bonusForCorrectAnswerSpinBox->setValue(
                settings.value("bonusForCorrectAnswer",BONUS_FOR_CORRECT_ANSWER).toInt());
    ui->bonusPerCorrectStreakSpinBox->setValue(
                settings.value("bonusPerCorrectStreak",BONUS_PER_CORRECT_STREAK).toInt());
    ui->maxBonusStreakLengthSpinBox->setValue(
                settings.value("maxBonusStreakLength",MAX_BONUS_STREAK_LENGTH).toInt());
    ui->bonusPerKnownLevelSpinBox->setValue(
                settings.value("bonusPerKnownLevel",BONUS_PER_KNOWN_LEVEL).toInt());
    settings.endGroup();

    //learning section
    settings.beginGroup("Learning");
    ui->n2lToNormSpinBox->setValue(
                settings.value("n2lToNorm",N2LTONORM).toInt());
    ui->normToN2lSpinBox->setValue(
                settings.value("normToN2l",NORMTON2L).toInt());
    ui->normToKnownSpinBox->setValue(
                settings.value("normToKnown",NORMTOKNOWN).toInt());
    ui->knownToNormSpinBox->setValue(
                settings.value("knownToNorm",KNOWNTONORM).toInt());
    ui->knownToOldSpinBox->setValue(
                settings.value("knownToOld",KNOWNTOOLD).toInt());
    ui->oldToNormSpinBox->setValue(
                settings.value("oldToNorm",OLDTONORM).toInt());
    settings.endGroup();
}

preferencesWindow::~preferencesWindow()
{
    delete ui;
}

void preferencesWindow::on_buttons_accepted()
{
    //testing section
    settings.beginGroup("Testing");
    settings.setValue("caseSensitiveAnswers",ui->caseSensitiveAnswersBox->isChecked());
    settings.setValue("timedAnswers",ui->timedAnswersBox->isChecked());
    settings.setValue("popupMode",ui->popupModeBox->isChecked());
    if (ui->popupModeBox->isChecked())
        settings.setValue("popupInterval",ui->popupIntervalSpinBox->value());
    settings.endGroup();

    //scoring section
    settings.beginGroup("Scoring");
    settings.setValue("bonusForCorrectAnswer",ui->bonusForCorrectAnswerSpinBox->value());
    settings.setValue("bonusPerCorrectStreak",ui->bonusPerCorrectStreakSpinBox->value());
    settings.setValue("maxBonusStreakLength",ui->maxBonusStreakLengthSpinBox->value());
    settings.setValue("bonusPerKnownLevel",ui->bonusPerKnownLevelSpinBox->value());
    settings.endGroup();

    //learning section
    settings.beginGroup("Learning");
    settings.setValue("n2lToNorm",ui->n2lToNormSpinBox->value());
    settings.setValue("normToN2l",ui->normToN2lSpinBox->value());
    settings.setValue("normToKnown",ui->normToKnownSpinBox->value());
    settings.setValue("knownToNorm",ui->knownToNormSpinBox->value());
    settings.setValue("knownToOld",ui->knownToOldSpinBox->value());
    settings.setValue("oldToNorm",ui->oldToNormSpinBox->value());
    settings.endGroup();

    accept();
}

void preferencesWindow::on_popupModeBox_toggled(bool checked)
{
    ui->popupModeFrame->setEnabled(checked);
}

void preferencesWindow::on_resetButton_clicked()
{
    //testing section
    ui->caseSensitiveAnswersBox->setChecked(true);
    ui->timedAnswersBox->setChecked(false);
    ui->popupModeBox->setChecked(false);
    on_popupModeBox_toggled(false);
    ui->popupIntervalSpinBox->setValue(DEFAULT_POPUP_INTERVAL);

    //scoring section
    ui->bonusForCorrectAnswerSpinBox->setValue(BONUS_FOR_CORRECT_ANSWER);
    ui->bonusPerCorrectStreakSpinBox->setValue(BONUS_PER_CORRECT_STREAK);
    ui->maxBonusStreakLengthSpinBox->setValue(MAX_BONUS_STREAK_LENGTH);
    ui->bonusPerKnownLevelSpinBox->setValue(BONUS_PER_KNOWN_LEVEL);

    //learning section
    ui->n2lToNormSpinBox->setValue(N2LTONORM);
    ui->normToN2lSpinBox->setValue(NORMTON2L);
    ui->normToKnownSpinBox->setValue(NORMTOKNOWN);
    ui->knownToNormSpinBox->setValue(KNOWNTONORM);
    ui->knownToOldSpinBox->setValue(KNOWNTOOLD);
    ui->oldToNormSpinBox->setValue(OLDTONORM);
}
