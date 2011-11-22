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

bool cardPack::canBePromoted(flashCard & candidateCard)
{
    if (candidateCard.wasCorrectLastTime() == false) return false;
    if (candidateCard.knownLevel == level_max) return false;

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
    if (candidateCard.knownLevel == level_min) return false;

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
    if (promotedCard.knownLevel == level_max)
        return false;

    knownLevelSets[promotedCard.knownLevel].removeCard(promotedCard);
    promotedCard.knownLevel = (knownLevel_t) (promotedCard.knownLevel + 1);
    knownLevelSets[promotedCard.knownLevel].addCard(promotedCard);

    return true; // FISH! TODO return success value
}

bool cardPack::demoteCard(flashCard & demotedCard)
{
    if (demotedCard.knownLevel == level_min)
        return false;

    if (demotedCard.knownLevel == level_old) // special case
    {
        knownLevelSets[level_old].removeCard(demotedCard);
        demotedCard.knownLevel = level_norm;
        knownLevelSets[demotedCard.knownLevel].addCard(demotedCard);
    }

    else // all others
    {
        knownLevelSets[demotedCard.knownLevel].removeCard(demotedCard);
        demotedCard.knownLevel = (knownLevel_t) (demotedCard.knownLevel - 1);
        knownLevelSets[demotedCard.knownLevel].addCard(demotedCard);
    }

    return true; // FISH! TODO return success value
}
