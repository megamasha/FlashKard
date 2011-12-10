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

class cardPack : public QObject
{
    Q_OBJECT
    public:

        explicit cardPack();

        bool addCard(flashCard & newCard, knownLevel_t set);
        bool removeCard(flashCard * cardToDelete = NULL);

        flashCard * getRandomCard();
        flashCard * getFirstCard();
        flashCard * getNextCard(const flashCard *);
        flashCard * getCardByIndex(int index);

        bool isEmpty();
        int cardsInPack();

        //FISH! this is just a bit of a kludge
        void exportdatabase(char * outputfilename);

    signals:

    public slots:

    private:

        cardSet knownLevelSets[level_max + 1]; //one set of cards for each 'knownLevel'
        unsigned int cardsInPackCounter;
        bool n2lFlag;

    //friends:

        friend class flashCard;
};

extern cardPack mainPack;

#endif // CARDPACK_H
