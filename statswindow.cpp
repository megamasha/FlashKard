#include "statswindow.h"
#include "ui_statswindow.h"
#include "cardpack.h"

StatsWindow::StatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsWindow)
{
    ui->setupUi(this);
    generateStats();
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::generateStats()
{
    Q_ASSERT(!mainPack.isEmpty());

    int infos = 0;
    int hints = 0;
    int untested = 0;
    int rights = 0;
    int wrongs = 0;
    int bestRun = 0;
    flashCard * bestRunCard = NULL;
    int worstRun = 0;
    flashCard * worstRunCard = NULL;

    // cycle through cards, assemble stats
    flashCard * currentCard = mainPack.getFirstCard();
    do
    {
        if (!currentCard->getInfo().isEmpty()) infos++;
        if (!currentCard->getHint().isEmpty()) hints++;
        if (currentCard->getCurrentStreak() == 0) untested++;
        else if (currentCard->wasCorrectLastTime()) rights++;
        else wrongs++;
        if (currentCard->wasCorrectLastTime() && (currentCard->getCurrentStreak() > bestRun) )
        {
            bestRun = currentCard->getCurrentStreak();
            bestRunCard = currentCard;
        }
        else if ( !currentCard->wasCorrectLastTime() && currentCard->getCurrentStreak() > worstRun)
        {
            worstRun = currentCard->getCurrentStreak();
            worstRunCard = currentCard;
        }
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    // generate text from stats
    QString generatedText;

    generatedText =
                tr("Your overall score is ") +
                QString::number(double(mainPack.packScore()),'f',2) +
                tr("%.") +

                tr("\n\nThere are presently ") +
                QString::number(mainPack.cardsInPack()) +
                tr(" entries loaded.\n\n");
    if (untested > 0)
    {
        generatedText +=
                QString::number(untested) +
                tr(" of these you've never been tested on.\n");
    }
    else
    {
        generatedText +=
                tr("You've been tested on all of them at least once.\n");
    }

    generatedText +=
                QString::number(rights) +
                tr(" of these you got RIGHT the last time they came up.\n") +
                QString::number(wrongs) +
                tr(" of these you got WRONG the last time they came up.\n\n") +

                QString::number(infos) +
                tr(" loaded entries have additional info.\n") +
                QString::number(hints) +
                tr(" loaded entries have an associated hint.\n\n");

    if (bestRun != 0)
    {
        generatedText +=
                tr("Your longest run of consecutive right answers is currently in response to the question:\n'") +
                bestRunCard->getQuestion() +
                tr("',\nwhich you got right the last ") +
                QString::number(bestRun) +
                tr(" times.\n\n");
    }
    if (worstRun != 0)
    {
        generatedText +=
                tr("Your longest run of consecutive wrong answers is currently in response to the question:\n'") +
                worstRunCard->getQuestion() +
                tr("',\nwhich you got wrong the last ") +
                QString::number(worstRun) +
                tr(" times.\n\n");
    }

    // apply text to window
    ui->statsText->setText(generatedText);
}
