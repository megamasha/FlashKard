#include "cardpack.h"

cardPack mainPack;

cardPack::cardPack(QObject *parent) :
    QObject(parent)
{

}

bool cardPack::addCard(flashCard &newCard, knownLevel_t set)
{
    bool success = false;
    switch (set)
    {
        case level_n2l:
                success = n2l.addCard(newCard);
                break;
        case level_norm:
                success = n2l.addCard(newCard);
                break;
        case level_known:
                success = n2l.addCard(newCard);
                break;
        case level_old:
                success = n2l.addCard(newCard);
                break;
        default:
                qDebug("unknown set when adding card");
    }

    if (success)
        qDebug("Added file to set %i",set);

    return success;
}
