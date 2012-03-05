#include "flashcardwindow.h"
#include "ui_flashcardwindow.h"
#include "cardpack.h"
#include "popupwindow.h"
#include "resultswindow.h"
#include <QCloseEvent>
#include <QSettings>

flashCardWindow::flashCardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flashCardWindow),
    currentTimerProgress(0)
{
    ui->setupUi(this);

    QSettings settings;
    if (settings.value("Testing/timedAnswers",false).toBool() == true)
    {
        resize( size().width(), size().height() + ui->timerProgressBar->size().height());
        ui->timerProgressBar->setVisible(true);
    }
    else
        ui->timerProgressBar->setVisible(false);

    connect(&timer,SIGNAL(timeout()),this,SLOT(progressTimer()));

    anotherCard();
}

flashCardWindow::~flashCardWindow()
{
    delete ui;
}

void flashCardWindow::closeEvent(QCloseEvent * event)
{
    //if the card has already been answer, the window can be safely closed
    if (answered)
        event->accept();

    //otherwise ask whether user wants to forfeit the question
    else if (popup.importantQuestion(this,tr("Are you sure you want to close the tester?\n\nThis will count as an incorrect answer!")))
    {
        currentCard->markAsIncorrect();
        event->accept();
    }
    else
        event->ignore();
}

void flashCardWindow::anotherCard()
{
    currentCard = mainPack.getRandomCard();
    if (currentCard == NULL)
    {
        popup.error(this,tr("No vocab loaded!"));
        delete this;
    }
    ui->answerBox->clear();
    answered = usedHintThisTime = false;
    ui->questionText->setText(currentCard->getQuestion());

    if (currentCard->hasInfo())
        ui->infoButton->setEnabled(true);
    else
        ui->infoButton->setDisabled(true);

    if (currentCard->hasHint())
        ui->hintButton->setEnabled(true);
    else
        ui->hintButton->setDisabled(true);

    QSettings settings;
    if (settings.value("Testing/timedAnswers",false).toBool() == true)
    {
        ui->timerProgressBar->setMaximum( (currentCard->getTimeLimit()) / TIMER_FRAME_LENGTH );
        ui->timerProgressBar->reset();
        currentTimerProgress = 0;
        timer.start(TIMER_FRAME_LENGTH);
    }
}

void flashCardWindow::on_infoButton_clicked()
{
    if (!currentCard->hasInfo())
        popup.info(this,tr("Sorry, there is no additional information for this flashcard\nPerhaps you should add some..."));
    else popup.info(this, currentCard->getInfo());
}

void flashCardWindow::on_hintButton_clicked()
{
    if (!currentCard->hasHint())
        popup.info(this,tr("Sorry, there is no hint for this flashcard\nPerhaps you should add one..."));
    else
    {
        if (popup.getYesOrNo(this, tr("Are you sure you want to view the hint?\n\nYou will not improve your score if you use a hint.")))
            popup.info(this, currentCard->getHint());
    }
}

void flashCardWindow::on_answerOKButton_clicked()
{
    timer.stop();
    currentCard->setAnswerTime(ui->timerProgressBar->value() * TIMER_FRAME_LENGTH);

    resultsWindow results(this,currentCard,ui->answerBox->text());
    if (results.exec())
        anotherCard();
    else accept();
}

void flashCardWindow::progressTimer()
{
    ui->timerProgressBar->setValue(++currentTimerProgress);

    if (ui->timerProgressBar->value() == ui->timerProgressBar->maximum())
    {
        on_answerOKButton_clicked();
    }
}
