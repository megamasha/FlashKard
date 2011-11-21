#include "flashcard.h"
#include "cardpack.h"
#include <QString>

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

    qDebug("Created flashCard.");
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

    if (mainPack.canBePromoted(*this))
        mainPack.promoteCard(*this);

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

    if (mainPack.canBeDemoted(*this))
        mainPack.demoteCard(*this);

    return true;
}


//string access functions
bool flashCard::setQuestion(QString & newQuestion)
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

bool flashCard::setAnswer(QString & newAnswer)
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

bool flashCard::setInfo(QString & newInfo)
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

bool flashCard::setHint(QString & newHint)
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
bool flashCard::resetKnownLevel()
{
    if (knownLevel != level_norm)
        {};//FISH! TODO
    return false;
}
