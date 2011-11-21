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

bool cardPack::canBePromoted(flashCard & candidateCard)
{
    if (candidateCard.wasCorrectLastTime() == false) return false;
    if (candidateCard.knownLevel == level_old) return false;

    int requiredLevelUp = INT_MAX;

    if (candidateCard.knownLevel == level_n2l)
        requiredLevelUp = N2LTONORM;
    else if (candidateCard.knownLevel == level_norm)
        requiredLevelUp = NORMTOKNOWN;
    else if (candidateCard.knownLevel == level_known)
        requiredLevelUp = KNOWNTOOLD;

    if (candidateCard.levelUp >= requiredLevelUp)
        return true;
    else
        return false;
}

bool cardPack::canBeDemoted(flashCard & candidateCard)
{
    if (candidateCard.wasCorrectLastTime()) return false;
    if (candidateCard.knownLevel == level_n2l) return false;

    int requiredLevelUp = INT_MAX;

    if (candidateCard.knownLevel == level_norm)
        requiredLevelUp = NORMTON2L;
    else if (candidateCard.knownLevel == level_known)
        requiredLevelUp = KNOWNTONORM;
    else if (candidateCard.knownLevel == level_old)
        requiredLevelUp = OLDTONORM;

    if (candidateCard.levelUp >= requiredLevelUp)
        return true;
    else
        return false;
}

bool cardPack::promoteCard(flashCard & promotedCard)
{
    if (promotedCard.knownLevel == level_old) return false;


    if (promotedCard.knownLevel == level_n2l)
    {
        n2l.removeCard(promotedCard);
        norm.addCard(promotedCard);
    }
    else if (promotedCard.knownLevel == level_norm)
    {
        norm.removeCard(promotedCard);
        known.addCard(promotedCard);
    }
    else if (promotedCard.knownLevel == level_n2l)
    {
        known.removeCard(promotedCard);
        old.addCard(promotedCard);
    }

    return true; // FISH! TODO return success value
}

bool cardPack::demoteCard(flashCard & demotedCard)
{
    if (demotedCard.knownLevel == level_n2l) return false;


    if (demotedCard.knownLevel == level_norm)
    {
        norm.removeCard(demotedCard);
        n2l.addCard(demotedCard);
    }
    else if (demotedCard.knownLevel == level_known)
    {
        known.removeCard(demotedCard);
        norm.addCard(demotedCard);
    }
    else if (demotedCard.knownLevel == level_old)
    {
        old.removeCard(demotedCard);
        norm.addCard(demotedCard);
    }

        return true; // FISH! TODO return success value
}
