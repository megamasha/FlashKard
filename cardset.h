#ifndef CARDSET_H
#define CARDSET_H

#include "flashcard.h"

class cardSet
{

    public:

        cardSet();
        bool addCard(flashCard & newCard);
        bool removeCard(flashCard & oldCard);
        unsigned int cardsInSet();
        bool isEmpty();

    private:

        flashCard * head;
        flashCard * tail;
        unsigned int cardsInSetCounter;

    friend class cardPack;
};

#endif // CARDSET_H
