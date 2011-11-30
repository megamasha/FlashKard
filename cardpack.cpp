#include "cardpack.h"

cardPack mainPack;

cardPack::cardPack(QObject *parent) :
    QObject(parent)
{

}

bool cardPack::addCard(flashCard &newCard, knownLevel_t set)
{
    bool success = knownLevelSets[set].addCard(newCard);

    if (success)
        qDebug("Added card to set %i",set);

    return success;
}
