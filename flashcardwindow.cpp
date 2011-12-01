#include "flashcardwindow.h"
#include "ui_flashcardwindow.h"
#include "cardpack.h"
#include "popupwindow.h"
#include "resultswindow.h"
#include <QCloseEvent>

flashCardWindow::flashCardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flashCardWindow)
{
    ui->setupUi(this);
    anotherCard();
}

flashCardWindow::~flashCardWindow()
{
    delete ui;
}

void flashCardWindow::closeEvent(QCloseEvent * event)
{
    if (answered)
        event->accept();
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
}

void flashCardWindow::on_infoButton_clicked()
{
    if (currentCard->getInfo().isEmpty())
        popup.info(this,tr("Sorry, there is no additional information for this flashcard\nPerhaps you should add some..."));
    else popup.info(this, currentCard->getInfo());
}

void flashCardWindow::on_hintButton_clicked()
{
    if (currentCard->getHint().isEmpty())
        popup.info(this,tr("Sorry, there is no hint for this flashcard\nPerhaps you should add one..."));
    else
    {
        if (popup.getYesOrNo(this, tr("Are you sure you want to view the hint?\n\nYou will not improve your score if you use a hint.")))
            popup.info(this, currentCard->getHint());
    }
}

void flashCardWindow::on_answerOKButton_clicked()
{

    resultsWindow results(this);
    results.processResults(*currentCard,ui->answerBox->text());
    if (results.exec())
        anotherCard();
    else accept();
}
