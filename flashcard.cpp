#include "flashcard.h"
#include <string>
#include <stdio.h>
#include <QString>

flashCard::flashCard(QString qu, QString an, QString in, QString hi, knownLevel_t kl, int lc, int cs, int lu)
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

    fprintf(stderr,"Created flashCard.");
};

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
