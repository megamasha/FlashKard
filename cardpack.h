#ifndef CARDPACK_H
#define CARDPACK_H

#include <QObject>
#include "flashcard.h"
#include "cardset.h"

#define N2LTONORM 5
#define NORMTON2L 3
#define NORMTOKNOWN 5
#define KNOWNTONORM 2
#define KNOWNTOOLD 3
#define OLDTONORM 1

class cardPack
{
    public:

        explicit cardPack();
        cardPack(cardPack & source);

        ~cardPack();

        bool addCard(flashCard & newCard, knownLevel_t set);
        bool removeCard(flashCard * cardToDelete = NULL);

        flashCard * getRandomCard();
        flashCard * getFirstCard();
        flashCard * getNextCard(const flashCard *);
        flashCard * getCardByIndex(int index);

        bool isEmpty();
        int cardsInPack();
        void setChanged();
        void setUnchanged();
        bool hasUnsavedChanges();

        bool containsCharacter(char charToFind);
        void replaceCharacter(char charToReplace, char replaceWith);
        //FISH! this is just a bit of a kludge
        void exportdatabase(QString fileToExport);

        float packScore();

    private:

        cardSet knownLevelSets[level_max + 1]; //one set of cards for each 'knownLevel'
        unsigned int cardsInPackCounter;
        bool n2lFlag;
        bool changed;

    //friends:

        friend class flashCard;
};

extern cardPack mainPack;

#endif // CARDPACK_H
