#ifndef CARDPACK_H
#define CARDPACK_H

#include <QObject>
#include "flashcard.h"
#include "cardset.h"

class cardPack : public QObject
{
    Q_OBJECT
    public:

        explicit cardPack(QObject *parent = 0);

        bool addCard(flashCard & newCard, knownLevel_t set);

        flashCard * getRandom();
        flashCard * getNext(const flashCard *);

    signals:

    public slots:

    private:

        cardSet n2l;
        cardSet norm;
        cardSet known;
        cardSet old;
        unsigned int cardsInPack;
};

extern cardPack mainPack;

#endif // CARDPACK_H
