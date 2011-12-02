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
    //mark the card accordingly
    if (currentCard.isCorrect(givenAnswer))
        currentCard.markAsCorrect();
    else
        currentCard.markAsIncorrect();

    //(re)create results text for user
    if (currentCard.isCorrect(givenAnswer))
        resultsText = tr("Yay! You're right!");
    else
        resultsText =
                tr("Sorry, the correct answer was:\n\n") +
                currentCard.getAnswer();

    if ( currentCard.isCorrect(givenAnswer) &&
         currentCard.wasCorrectLastTime() &&
         currentCard.getCurrentStreak() > 1 )
        resultsText +=
                tr("\n\nYou got this one right the last ") +
                QString::number(currentCard.getCurrentStreak()) +
                tr(" times in a row.");
    else if ( currentCard.isCorrect(givenAnswer) == false &&
              currentCard.wasCorrectLastTime() == false &&
              currentCard.getCurrentStreak() > 2 )
        resultsText +=
                tr("\n\nYou got this one wrong the last ") +
                QString::number(currentCard.getCurrentStreak()) +
                tr(" times in a row.");

    //put this string in the window
    ui->resultsText->setText(resultsText);

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
