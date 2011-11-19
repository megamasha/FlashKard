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
