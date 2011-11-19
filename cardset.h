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

private:

    flashCard * head;
    flashCard * tail;
    unsigned int cardsInSetCounter;
};

#endif // CARDSET_H
