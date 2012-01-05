#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "popupwindow.h"
#include <QCloseEvent>

editorWindow::editorWindow(QWidget *parent, flashCard * card) :
    QDialog(parent),
    ui(new Ui::editorWindow)
{
    ui->setupUi(this);

    currentCard = card;

    ui->questionText->setText(currentCard->getQuestion());
    ui->answerText->setText(currentCard->getAnswer());
    ui->infoText->setText(currentCard->getInfo());
    ui->hintText->setText(currentCard->getHint());
    ui->levelText->setText(createLevelText());
    ui->streakText->setText(createStreakText());
    if (currentCard->isHighPriority())
        ui->markN2lButton->setDisabled(true);
    else if (currentCard->isLowPriority())
        ui->markKnownButton->setDisabled(true);
}

editorWindow::~editorWindow()
{
    delete ui;
}

void editorWindow::closeEvent(QCloseEvent *event)
{
    // has text changed?
    if (ui->questionText->text() != currentCard->getQuestion() ||
        ui->answerText->text()   != currentCard->getAnswer()   ||
        ui->infoText->text()     != currentCard->getInfo()     ||
        ui->hintText->text()     != currentCard->getHint()       )
    {
        // if so, does user want to close without saving it?
        if (popup.importantQuestion(this,tr("Discard changes?")))
            event->accept();
        else
            event->ignore();
    }
    // close window without asking if text hasn't changed
    else
        event->accept();

}

QString editorWindow::createLevelText()
{
    QString returnString;
    switch (currentCard->knownLevel)
    {
        case level_n2l:
            returnString = tr("This card is set as high priority to learn.");
            break;
        case level_norm:
            returnString = tr("This card is normal priority. It is still being learnt.");
            break;
        case level_known:
            returnString = tr("This card is well-known. It is set as low priority to learn.");
            break;
        case level_old:
            returnString = tr("This card is archived. You have answered it correctly many times and it is rarely tested now.");
            break;
        default:
            qDebug("This shouldn't happen!!");
            break;
    } //switch

    //add card score
    returnString += tr("\nThe current card score is ") +
                    QString::number(currentCard->score()) +
                    tr(" out of ") +
                    QString::number(flashCard::maxPossibleScore) +
                    tr(".");

    return returnString;
}

QString editorWindow::createStreakText()
{
    QString returnString;
    if (currentCard->getCurrentStreak() == 0)
        returnString = tr("You've never been tested on this card.");
    else if (currentCard->getCurrentStreak() == 1)
    {
        returnString = tr("You got this card ");
        if (currentCard->wasCorrectLastTime())
            returnString += tr("right");
        else
            returnString += tr("wrong");
        returnString += tr(" last time");
    }
    else
    {
        returnString = tr("You got this card ");
        if (currentCard->wasCorrectLastTime())
            returnString += tr("right");
        else
            returnString += tr("wrong");
        returnString += tr(" the last ");
        returnString += QString::number(currentCard->getCurrentStreak());
        returnString += tr(" times you were tested on it.");
    }

    return returnString;
}
void editorWindow::on_markN2lButton_clicked()
{
    if (currentCard->setHighPriority())
    {
        ui->markN2lButton->setDisabled(true);
        ui->markKnownButton->setDisabled(false);
        ui->levelText->setText(createLevelText());
    }
}

void editorWindow::on_markKnownButton_clicked()
{
    if (currentCard->setLowPriority())
    {
        ui->markKnownButton->setDisabled(true);
        ui->markN2lButton->setDisabled(false);
        ui->levelText->setText(createLevelText());
    }
}
void editorWindow::on_buttonBox_accepted()
{
    currentCard->setQuestion(ui->questionText->text());
    currentCard->setAnswer(ui->answerText->text());
    currentCard->setInfo(ui->infoText->text());
    currentCard->setHint(ui->hintText->text());

    accept();
}

void editorWindow::on_buttonBox_rejected()
{
    if (popup.importantQuestion(this,tr("Discard changes?")))
        reject();
}
