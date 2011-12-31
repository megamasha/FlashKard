#include "cardpack.h"
#include <stdlib.h>
#include <time.h>
#include "popupwindow.h"
#include "flashcardwindow.h"
#include <QFile>
#include "svfileops.h" //for MAXTEXTLENGTH

cardPack mainPack;

cardPack::cardPack()
{
    srand((unsigned)time(NULL));
}

bool cardPack::addCard(flashCard &newCard, knownLevel_t set = level_norm)
{
    bool success = knownLevelSets[set].addCard(newCard);

    if (success)
    {
        qDebug("Added card to set %i",set);
        newCard.parentPack = this;
        cardsInPackCounter++;
    }

    return success;
}

bool cardPack::removeCard(flashCard * cardToDelete)
{
    if (cardToDelete == NULL)
        return false;
    bool success = true;
    success = success &
            cardToDelete->parentSet->removeCard(*cardToDelete);
    if (success)
    {
        cardsInPackCounter--;
        delete cardToDelete;
    }

    return success;
}

flashCard * cardPack::getRandomCard()
{

    if (isEmpty())
        return NULL;

    //select a list at random, using the percentage probabilities in the if statements.
    unsigned int selector = rand() % 100 + 1;

    cardSet * currentSet = NULL;
    if (selector > 100) {qFatal("Problem with random number generator. Fix it!");exit(1);}
    else if (selector == 100) currentSet = &knownLevelSets[level_old];
    else if (selector > 94) currentSet = &knownLevelSets[level_known];
    else if (selector > 32) {n2lFlag=false;currentSet=&knownLevelSets[level_norm];} //use norm list and cancel n2l flag (not cancelled with other lists)
    else if (selector < 33) currentSet = &knownLevelSets[level_n2l];

    //do a little control over random selection

    //if n2l list was used last time as well (flag is set), use entry from the norm list instead
    if (currentSet==&knownLevelSets[level_n2l] && n2lFlag)
    {
        currentSet=&knownLevelSets[level_norm];
        n2lFlag = 0; // FISH! change to false
    }
    if (currentSet==&knownLevelSets[level_n2l]) n2lFlag = 1; //FISH! change to true//is using n2l this time, set flag so it won't be used next time as well
    if (currentSet->isEmpty()) currentSet = &knownLevelSets[level_norm];//if current list is empty, default to normal list
    if (currentSet->isEmpty() && !n2lFlag) currentSet = &knownLevelSets[level_n2l];//if normal list is empty, try n2l list if it wasn't used last time
    if (currentSet->isEmpty() && selector % 10 == 0) currentSet = &knownLevelSets[level_old];//if list is still empty, in 10% of cases try old list
    if (currentSet->isEmpty()) currentSet = &knownLevelSets[level_known];//in the other 90% of cases, or if old is empty, use the known list
    if (currentSet->isEmpty()) currentSet = &knownLevelSets[level_old];//if known list is empty, try the old list
    if (currentSet->isEmpty()) {currentSet = &knownLevelSets[level_n2l];n2lFlag = true;}//if old list is empty, use n2l list EVEN if it was used last time
    if (currentSet->isEmpty()) return NULL; //if list is STILL empty, abort

    //we now have the desired list of words with at least one entry, let's select an entry at random from this list
    selector = (rand() % currentSet->cardsInSet())+1;
    flashCard * returnCard = currentSet->head;
    for (unsigned int i=1;i<selector;i++) //i starts on 1, so this loops 1 less time than selector
    {
        if (returnCard != NULL)
            returnCard = returnCard->next;
        else return NULL;
    }
    return returnCard;
}

flashCard * cardPack::getFirstCard()
{
    if (isEmpty())
        return NULL;

    flashCard * returnCard = NULL;
    for (int i = 0;i <= level_max;i++)
    {
        returnCard = knownLevelSets[i].head;
        if (returnCard) break;
    }
    return returnCard;
}

flashCard * cardPack::getNextCard(const flashCard * currentCard)
{
    //return next card in set, if there is one
    if (currentCard->next)
        return currentCard->next;

    //otherwise set up a set pointer...
    cardSet * currentSet = currentCard->parentSet;
    while (currentSet != &knownLevelSets[level_max])
        //...and if this is not the last set, loop until the last set
    {
        //move onto next set (works because they're declared as an array)
        currentSet++;
        if (currentSet->isEmpty() == false)
            return currentSet->head;
    }
    return NULL;
}

flashCard * cardPack::getCardByIndex(int index)
{
    if (isEmpty())
        return NULL;

    //this is VERY lazy and VERY inefficient (I think)
    //FISH! better implementation etc. TODO
    flashCard * returnCard = getFirstCard();
    while (index != 1)
    {
        returnCard = getNextCard(returnCard);
        index--;
    }
    return returnCard;
}

bool cardPack::isEmpty()
{
    if (cardsInPackCounter == 0)
        return true;
    else
        return false;
}

int cardPack::cardsInPack()
{
    return cardsInPackCounter;
}

void cardPack::exportdatabase (char * outputfilename)
//this is all just a bit of a kludge,
//might be alright in C, but probably bad C++ form
{
    flashCard * currentCard = mainPack.getFirstCard();
    if (currentCard == NULL) return;

    QFile outputfile (outputfilename);
    outputfile.open(QIODevice::WriteOnly);

    int bytesToWrite;
    do
    {
        bytesToWrite = currentCard->getQuestion().length() > MAXTEXTLENGTH ?
                            MAXTEXTLENGTH :
                            currentCard->getQuestion().length();
        outputfile.write(currentCard->getQuestion().toAscii(),bytesToWrite);
        outputfile.write("~");

        bytesToWrite = currentCard->getAnswer().length() > MAXTEXTLENGTH ?
                            MAXTEXTLENGTH :
                            currentCard->getAnswer().length();
        outputfile.write(currentCard->getAnswer().toAscii(),bytesToWrite);
        outputfile.write("~");

        bytesToWrite = currentCard->getInfo().length() > MAXTEXTLENGTH ?
                            MAXTEXTLENGTH :
                            currentCard->getInfo().length();
        outputfile.write(currentCard->getInfo().toAscii(),bytesToWrite);
        outputfile.write("~");

        bytesToWrite = currentCard->getHint().length() > MAXTEXTLENGTH ?
                            MAXTEXTLENGTH :
                            currentCard->getHint().length();
        outputfile.write(currentCard->getHint().toAscii(),bytesToWrite);
        outputfile.write("~");

        char tempstring[10];
        sprintf(tempstring,"%d~%d~%d",
                currentCard->wasCorrectLastTime(),
                currentCard->levelUp,
                currentCard->knownLevel);

        outputfile.write(tempstring);
        if (mainPack.getNextCard(currentCard) != NULL)
            outputfile.write("\n");
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    outputfile.close();
}

float cardPack::packScore()
{
    if (isEmpty())
        return 0.0;
    if (flashCard::maxPossibleScore == 0)
        return 0.0;

    long score = 0;
    flashCard * currentCard = getFirstCard();
    do
    {
        if (currentCard->wasCorrectLastTime())
        {
            score += currentCard->score();
        }
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    return (float(score) / float(cardsInPackCounter) / float(flashCard::maxPossibleScore) * 100);
}
