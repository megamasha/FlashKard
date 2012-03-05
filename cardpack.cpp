#include "cardpack.h"
#include <stdlib.h>
#include "popupwindow.h"
#include "flashcardwindow.h"
#include <QFile>
#include <QObject>
#include "svfileops.h" //for MAXTEXTLENGTH

cardPack mainPack;

cardPack::cardPack() :
    cardsInPackCounter(0),
    n2lFlag (false),
    changed (false)
{
    ;
}

cardPack::cardPack(cardPack & source) :
    cardsInPackCounter(0),
    n2lFlag (source.n2lFlag),
    changed (false)
{
    flashCard * sourceCard = source.getFirstCard();
    if (sourceCard == NULL) return;

    do
    {
        flashCard * newCard = new flashCard;
        *newCard = *sourceCard;
        addCard(*newCard,newCard->getKnownLevel());
    }
    while ((sourceCard = source.getNextCard(sourceCard)) != NULL);
}

cardPack::~cardPack()
{
    flashCard * currentCard = getFirstCard();
    if (currentCard == NULL)
        return;
    do
    {
        removeCard(currentCard);
    }
    while ((currentCard = getFirstCard()) != NULL);
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

void cardPack::empty()
{
    flashCard * currentCard = getFirstCard();
    if (currentCard == NULL)
        return;
    do
    {
        removeCard(currentCard);
    }
    while ((currentCard = getFirstCard()) != NULL);
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

void cardPack::setChanged()
{
    changed = true;
}

void cardPack::setUnchanged()
{
    changed = false;
}

bool cardPack::hasUnsavedChanges()
{
    return changed;
}

bool cardPack::containsCharacter(char charToFind)
{
    if (isEmpty()) return false;

    //loop through all cards
    flashCard * currentCard = getFirstCard();
    do
    {
        if (currentCard->containsCharacter(charToFind)) return true;
    }
    while ((currentCard = getNextCard(currentCard)) != NULL);

    return false;
}

void cardPack::replaceCharacter(char charToReplace, char replaceWith)
{
    if (isEmpty()) return;

    //loop through all cards
    flashCard * currentCard = getFirstCard();
    do
    {
        currentCard->replaceCharacter(charToReplace,replaceWith);
    }
    while ((currentCard = getNextCard(currentCard)) != NULL);
}

void cardPack::removeDuplicates()
{
    // *** WARNING!! *** Here be demons! Specifically, gotos. ***
    // They are used to make the logic of this function easier to follow.
    // Otherwise, there'd be a truck-load of loops and massive indentations (bad enough as it is!),
    // And recursion is difficult to implement while trying to keep track of how many entries were removed.

    int cardsRemoved = 0;

    flashCard * cardA;
    flashCard * cardB;

STARTOVER:

    //sanity check
    if (cardsInPackCounter <= 1)
        goto FINISHED;

    //iterate over all combinations of two cards in the pack
    for (cardA = getFirstCard() ; cardA != getCardByIndex(cardsInPackCounter) ; cardA = getNextCard(cardA) )
    {
        for (cardB = getNextCard(cardA); cardB != NULL ; cardB = getNextCard(cardB) )
        {
            if (cardB->isDuplicateOf(cardA))
            {
                mergeCards(cardA,cardB);
                cardsRemoved++;
                goto STARTOVER;
            }
        }
    }

FINISHED:

    popup.info(0,QObject::tr("Cards removed: %1.").arg(cardsRemoved));

}

void cardPack::mergeCards(flashCard *cardA, flashCard *cardB)
{
    //determine which card to delete
    flashCard * cardToDelete;

    //delete one if it has a lower score
    if      (cardA->score() > cardB->score())
        cardToDelete = cardB;
    else if (cardB->score() > cardA->score())
        cardToDelete = cardA;

    //if scores are the same, check whether one has a hint while the other does not
    else if (cardA->hasHint() && !cardB->hasHint())
        cardToDelete = cardB;
    else if (cardB->hasHint() && !cardA->hasHint())
        cardToDelete = cardA;

    //or whether one has more info than the other
    else if (cardA->getInfo().length() > cardB->getInfo().length())
        cardToDelete = cardB;
    else if (cardB->getInfo().length() > cardA->getInfo().length())
        cardToDelete = cardA;

    //and if they're otherwise identical, just delete card A
    else
        cardToDelete = cardA;


    //move favourable changes to the card to be kept.
    flashCard * cardToKeep;
    if (cardToDelete == cardA)
        cardToKeep = cardB;
    else
        cardToKeep = cardA;

    if (cardToDelete->hasHint() && !cardToKeep->hasHint())
        cardToKeep->setHint(cardToDelete->getHint());
    if (cardToDelete->getInfo().length() > cardToKeep->getInfo().length())
        cardToKeep->setInfo(cardToDelete->getInfo());

    //and delete the unneeded duplicate
    removeCard(cardToDelete);
}

void cardPack::exportdatabase (QString fileToExport)
//this is all just a bit of a kludge,
//might be alright in C, but probably bad C++ form
{
    flashCard * currentCard = getFirstCard();
    if (currentCard == NULL) return;

    //check for invalid character '~'
    if (containsCharacter('~'))
    {
        //if pack has invalid characters and user doesn't want to replace them,
        if ( ! popup.importantQuestion(0,QObject::tr("Your flashcards contain the character '~', which cannot be stored in a '~sv' file.\n\nWould you like to export a file in which all instances of '~' are replaced with '-'?")))
        {
            popup.info(0,QObject::tr("File not saved."));
            return;
        }

        //FISH! TODO Make this act on a copy of the pack (requires cardPack copy constructor)

        //otherwise, replace characters and continue
        else
        {
            cardPack exportCopy = mainPack;
            exportCopy.replaceCharacter('~','-');
            exportCopy.exportdatabase(fileToExport);
            return;
        }
    }

    QFile outputfile (fileToExport);
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
        if (getNextCard(currentCard) != NULL)
            outputfile.write("\n");
    }
    while ((currentCard = getNextCard(currentCard)) != NULL);

    outputfile.close();
}

float cardPack::packScore()
{
    if (isEmpty())
        return 0.0;
    if (flashCard::maxPossibleScore() == 0)
        return 0.0;

    long score = 0;
    flashCard * currentCard = getFirstCard();
    do
    {
        score += currentCard->score();
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    return (float(score) / float(cardsInPackCounter) / float(flashCard::maxPossibleScore()) * 100);
}
