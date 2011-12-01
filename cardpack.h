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

        explicit cardPack(QObject *parent = 0);

        bool addCard(flashCard & newCard, knownLevel_t set);

        flashCard * getRandomCard();
        flashCard * getNextCard(const flashCard *);

        bool isEmpty();
        unsigned int cardsInPack();

    signals:

    public slots:

    private:

        cardSet knownLevelSets[level_max]; //one set of cards for each 'knownLevel'
        unsigned int cardsInPackCounter;
        bool n2lFlag;

    //friends:

        friend class flashCard;
};

extern cardPack mainPack;

#endif // CARDPACK_H
