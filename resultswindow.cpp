#include "resultswindow.h"
#include "ui_resultswindow.h"
#include "editorwindow.h" //so it can spawn one

resultsWindow::resultsWindow(QWidget *parent,flashCard * card, QString givenAnswer) :
    QDialog(parent),
    ui(new Ui::resultsWindow),
    currentCard(card)
{
    ui->setupUi(this);
    processResults(givenAnswer);
}

resultsWindow::~resultsWindow()
{
    delete ui;
}

bool resultsWindow::processResults(QString givenAnswer)
{
    //mark the card accordingly
    if (currentCard->isCorrect(givenAnswer))
        currentCard->markAsCorrect();
    else
        currentCard->markAsIncorrect();

    generateResultsText(givenAnswer);

    return true; //FISH! TODO replace with success value
}

void resultsWindow::generateResultsText(QString & givenAnswer)
{
    //(re)create results text for user
    if (currentCard->isCorrect(givenAnswer))
        resultsText = tr("Yay! You're right!");
    else
        resultsText =
                tr("Sorry, the correct answer was:\n\n") +
                currentCard->getAnswer();

    if ( currentCard->isCorrect(givenAnswer) &&
         currentCard->wasCorrectLastTime() &&
         currentCard->getCurrentStreak() > 1 )
        resultsText +=
                tr("\n\nYou got this one right the last ") +
                QString::number(currentCard->getCurrentStreak()) +
                tr(" times in a row.");
    else if ( currentCard->isCorrect(givenAnswer) == false &&
              currentCard->wasCorrectLastTime() == false &&
              currentCard->getCurrentStreak() > 2 )
        resultsText +=
                tr("\n\nYou got this one wrong the last ") +
                QString::number(currentCard->getCurrentStreak()) +
                tr(" times in a row.");

    resultsText +=
            tr("\n\nYour score for this card is ") +
            QString::number(currentCard->score()) +
            tr(" out of ") +
            QString::number(flashCard::maxPossibleScore) +
            tr(".");

    resultsText +=
            tr("\nYour overall score is ") +
            QString::number(double(mainPack.packScore()),'f',2) +
            tr("%.");

    //put this string in the window
    ui->resultsText->setText(resultsText);
}

void resultsWindow::on_stopButton_clicked()
{
    reject();
}

void resultsWindow::on_continueButton_clicked()
{
    accept();
}

void resultsWindow::on_editButton_clicked()
{
    editorWindow editor(this, currentCard);
    editor.exec();
}
