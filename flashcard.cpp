#include "flashcard.h"
#include "cardpack.h"
#include "cardset.h"
#include <QString>

flashCard::flashCard()
{
    question = tr("Question");
    answer = tr("Answer");
    knownLevel = level_norm;
    lastCorrect = false;
    currentStreak = levelUp = 0;

    next = NULL;
    parentPack = NULL;
    parentSet = NULL;
}

flashCard::flashCard (
    QString qu,
    QString an,
    QString in,
    QString hi,
    knownLevel_t kl,
    bool lc,
    int cs,
    int lu)
{
    question = qu;
    answer = an;
    info = in;
    hint = hi;

    knownLevel = kl;
    lastCorrect = lc;
    currentStreak = cs;
    levelUp = lu;

    next = NULL;
    parentPack = NULL;
    parentSet = NULL;

}


//functions to do with answer correctness
bool flashCard::isCorrect (QString & yourAnswer)
{
    if (yourAnswer == answer)
        return true;
    else
        return false;
}

// bool isAlmostCorrect (QString & yourAnswer); //FISH! TODO

bool flashCard::wasCorrectLastTime()
{
    return lastCorrect;
}

int flashCard::getCurrentStreak()
{
    return currentStreak;
}

bool flashCard::markAsCorrect()
{
    if (lastCorrect)
    {
        currentStreak++;
        levelUp++;
    }
    else // if this is the first time in a row you've got it right
    {
        lastCorrect = true;
        currentStreak = 1;
        levelUp = 1;
    }

    if (canBePromoted())
        promoteCard();

    return true;
}

bool flashCard::markAsIncorrect()
{
    if (lastCorrect) // if you got it right last time
    {
        lastCorrect = false;
        currentStreak = 1;
        levelUp = 1;
    }
    else // if this is not the first time in a row you've got it wrong
    {
        currentStreak++;
        levelUp++;
    }

    if (canBeDemoted())
        demoteCard();

    return true;
}

int flashCard::score()
{
    int returnValue = 0;
    returnValue += BONUS_PER_KNOWN_LEVEL * (knownLevel - 1);
    if (wasCorrectLastTime())
    {
        returnValue += BONUS_FOR_CORRECT_ANSWER;
        //enforce max streak length
        int bonusStreak =
                currentStreak >= MAX_BONUS_STREAK_LENGTH ?
                    MAX_BONUS_STREAK_LENGTH : currentStreak;
        returnValue += (BONUS_PER_CORRECT_STREAK * bonusStreak);
    }
    else
        returnValue += (BONUS_FOR_CORRECT_ANSWER - currentStreak);

    // ensure return value isn't negative
    return (returnValue > 0 ? returnValue : 0);
}


//string access functions
bool flashCard::setQuestion(QString newQuestion)
{
    question = newQuestion;
    if (question == newQuestion)
        return true;
    else
        return false;
}

QString flashCard::getQuestion()
{
    return question;
}

bool flashCard::setAnswer(QString newAnswer)
{
    answer = newAnswer;
    if (answer == newAnswer)
        return true;
    else
        return false;
}

QString flashCard::getAnswer()
{
    return answer;
}

bool flashCard::setInfo(QString newInfo)
{
    info = newInfo;
    if (info == newInfo)
        return true;
    else
        return false;
}

QString flashCard::getInfo()
{
    return info;
}

bool flashCard::setHint(QString newHint)
{
    hint = newHint;
    if (hint == newHint)
        return true;
    else
        return false;
}

QString flashCard::getHint()
{
    return hint;
}


//priority adjustment functions
bool flashCard::canBePromoted()
{
    if (wasCorrectLastTime() == false) return false;
    if (knownLevel == level_max) return false;

    int requiredLevelUp = INT_MAX;

    if (knownLevel == level_n2l)
        requiredLevelUp = N2LTONORM;
    else if (knownLevel == level_norm)
        requiredLevelUp = NORMTOKNOWN;
    else if (knownLevel == level_known)
        requiredLevelUp = KNOWNTOOLD;

    if (levelUp >= requiredLevelUp)
        return true;
    else
        return false;
}

bool flashCard::canBeDemoted()
{
    if (wasCorrectLastTime()) return false;
    if (knownLevel == level_min) return false;

    int requiredLevelUp = INT_MAX;

    if (knownLevel == level_norm)
        requiredLevelUp = NORMTON2L;
    else if (knownLevel == level_known)
        requiredLevelUp = KNOWNTONORM;
    else if (knownLevel == level_old)
        requiredLevelUp = OLDTONORM;

    if (levelUp >= requiredLevelUp)
        return true;
    else
        return false;
}

bool flashCard::promoteCard() //FISH! TODO display (somewhere) messages about promotion
{
    if (knownLevel == level_max)
        return false;

    parentPack->knownLevelSets[knownLevel].removeCard(*this);

    knownLevel = (knownLevel_t) (knownLevel + 1);
    levelUp = 1;

    parentPack->knownLevelSets[knownLevel].addCard(*this);

    return true; // FISH! TODO return success value
}

bool flashCard::demoteCard() //FISH! TODO display (somewhere) messages about demotion
{
    if (knownLevel == level_min)
        return false;

    parentPack->knownLevelSets[knownLevel].removeCard(*this);

    if (knownLevel == level_old) // special case
        knownLevel = level_norm;
    else // all others
        knownLevel = (knownLevel_t) (knownLevel - 1);

    levelUp = 1;

    parentPack->knownLevelSets[knownLevel].addCard(*this);

    return true; // FISH! TODO return success value
}


bool flashCard::resetKnownLevel()
{
    bool success = true;

    if (knownLevel != level_norm)
    {
        success = success &
            mainPack.knownLevelSets[knownLevel].removeCard(*this);
        knownLevel = level_norm;
        levelUp = 1;
        success = success &
            mainPack.knownLevelSets[knownLevel].addCard(*this);
    }
    else
        success = false;

    return success;
}

bool flashCard::setHighPriority()
{
    bool success = true;

    if (knownLevel != level_n2l)
    {
        success = success &
                mainPack.knownLevelSets[knownLevel].removeCard(*this);
        knownLevel = level_n2l;
        levelUp = 1;
        success = success &
                mainPack.knownLevelSets[knownLevel].addCard(*this);
    }
    else
        success = false;

    return success;
}

bool flashCard::isHighPriority()
{
    if (knownLevel == level_n2l)
        return true;
    else
        return false;
}

bool flashCard::setLowPriority()
{
    bool success = true;

    if (knownLevel < level_known)
    {
        success = success &
                mainPack.knownLevelSets[knownLevel].removeCard(*this);
        knownLevel = level_known;
        levelUp = 1;
        success = success &
                mainPack.knownLevelSets[knownLevel].addCard(*this);
    }
    else
        success = false;

    return success;
}

bool flashCard::isLowPriority()
{
    if (knownLevel >= level_known)
        return true;
    else
        return false;
}