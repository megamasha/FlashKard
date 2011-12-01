#include "resultswindow.h"
#include "ui_resultswindow.h"

resultsWindow::resultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultsWindow)
{
    ui->setupUi(this);
}

resultsWindow::~resultsWindow()
{
    delete ui;
}

bool resultsWindow::processResults(flashCard & currentCard, QString givenAnswer)
{
    //(re)create results text for user
    if (currentCard.isCorrect(givenAnswer))
        resultsText = tr("Yay! You're right!");
    else
        resultsText =
                tr("Sorry, the correct answer was:\n\n") +
                currentCard.getAnswer();

/* FISH! FIXME This seems to lead to SegFaults. Check the code.

    if ( currentCard.wasCorrectLastTime() &&
         currentCard.getCurrentStreak() > 1 )
        resultsText +=
                tr("\n\nYou got this one right the last ") +
                currentCard.getCurrentStreak() +
                tr(" times in a row.");
    else if ( currentCard.wasCorrectLastTime() == false &&
         currentCard.getCurrentStreak() > 2 )
        resultsText +=
                tr("\n\nYou got this one wrong the last ") +
                currentCard.getCurrentStreak() +
                tr(" times in a row.");

    if (currentCard.canBePromoted())
        resultsText += tr("\n\nPromoting this card to the next level and increasing your score.\nFISH!\nProd Rob to make this text more descriptive!");
    if (currentCard.canBeDemoted())
        resultsText += tr("\n\nDemoting this card to the next level and decreasing your score.\nFISH!\nProd Rob to make this text more descriptive!");

    //put this string in the window
    ui->resultsText->setText(resultsText);

    //adjust the card accordingly
    if (currentCard.isCorrect(givenAnswer))
        currentCard.markAsCorrect();
    else
        currentCard.markAsIncorrect();

*/

    return true; //FISH! TODO replace with success value
}

void resultsWindow::on_stopButton_clicked()
{
    reject();
}

void resultsWindow::on_continueButton_clicked()
{
    accept();
}