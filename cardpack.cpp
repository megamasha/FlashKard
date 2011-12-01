#include "cardpack.h"
#include <stdlib.h>
#include <time.h>
#include "popupwindow.h"
#include "flashcardwindow.h"

cardPack mainPack;

cardPack::cardPack(QObject *parent) :
    QObject(parent)
{
    srand((unsigned)time(NULL));
}

bool cardPack::addCard(flashCard &newCard, knownLevel_t set)
{
    bool success = knownLevelSets[set].addCard(newCard);

    if (success)
    {
        qDebug("Added card to set %i",set);
        cardsInPackCounter++;
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

bool cardPack::isEmpty()
{
    if (cardsInPackCounter == 0)
        return true;
    else
        return false;
}

unsigned int cardPack::cardsInPack()
{
    return cardsInPackCounter;
}
