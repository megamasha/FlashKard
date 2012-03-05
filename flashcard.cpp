#include "flashcard.h"
#include "cardpack.h"
#include "cardset.h"
#include <QString>
#include <QObject>
#include <QSettings>
#include <QVector>

flashCard::flashCard() :
    question (QObject::tr("Question")),
    answer (QObject::tr("Answer")),
    knownLevel (level_norm),
    lastCorrect (false),
    currentStreak (0),
    levelUp (0),

    answerTime (DEFAULT_TIME_LIMIT),

    next (NULL),
    parentPack (NULL),
    parentSet (NULL)
{

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

    :

      question (qu),
      answer (an),
      info (in),
      hint (hi),

      knownLevel (kl),
      lastCorrect (lc),
      currentStreak (cs),
      levelUp (lu),

      answerTime (DEFAULT_TIME_LIMIT),

      next (NULL),
      parentPack (NULL),
      parentSet (NULL)
{

}

flashCard & flashCard::operator =(const flashCard & source)
{
    question = source.question;
    answer = source.answer;
    info = source.info;
    hint = source.hint;

    knownLevel = source.knownLevel;
    lastCorrect = source.lastCorrect;
    currentStreak = source.currentStreak;
    levelUp = source.levelUp;

    next = NULL;
    parentPack = NULL;
    parentSet = NULL;

    return *this;
}

//functions to do with answer correctness
bool flashCard::isCorrect (QString & yourAnswer)
{
    //establish case sensitivity
    QSettings settings;
    Qt::CaseSensitivity cs = settings.value("Testing/caseSensitiveAnswers",true).toBool()
                           ? Qt::CaseSensitive
                           : Qt::CaseInsensitive;

    int comparisonResult = answer.compare(yourAnswer,cs);

    return (comparisonResult == 0);
}

bool flashCard::isAlmostCorrect (QString & yourAnswer)
{
    //get error threshold
    QSettings settings;
    int targetSimilarity = settings.value("Testing/almostCorrectThreshold",DEFAULT_ALMOST_CORRECT_THRESHOLD).toInt();

    int maxLength = qMax(answer.length(),yourAnswer.length());
    int similarLetters = maxLength - levenshteinDistance(yourAnswer);
    long timesHundred = similarLetters * 100;
    int percentageSimilarity = timesHundred / maxLength;

    qDebug("MaxLength: %i, Similarity: %i",maxLength,percentageSimilarity);

    return (percentageSimilarity >= targetSimilarity);
}

bool flashCard::wasCorrectLastTime()
{
    return lastCorrect;
}

int flashCard::getCurrentStreak()
{
    return currentStreak;
}

int flashCard::getLevelUp()
{
    return levelUp;
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

int flashCard::n2lToNorm = N2LTONORM;
int flashCard::normToN2l = NORMTON2L;
int flashCard::normToKnown = NORMTOKNOWN;
int flashCard::knownToNorm = KNOWNTONORM;
int flashCard::knownToOld = KNOWNTOOLD;
int flashCard::oldToNorm = OLDTONORM;

int flashCard::bonusForCorrectAnswer = BONUS_FOR_CORRECT_ANSWER;
int flashCard::bonusPerCorrectStreak = BONUS_PER_CORRECT_STREAK;
int flashCard::maxBonusStreakLength = MAX_BONUS_STREAK_LENGTH;
int flashCard::bonusPerKnownLevel = BONUS_PER_KNOWN_LEVEL;

int flashCard::score()
{
    if (currentStreak == 0) return 0;

    int returnValue = 0;
    returnValue += bonusPerKnownLevel * (knownLevel - 1);
    if (wasCorrectLastTime())
    {
        returnValue += bonusForCorrectAnswer;
        //enforce max streak length
        int bonusStreak =
                currentStreak >= maxBonusStreakLength ?
                    maxBonusStreakLength : currentStreak;
        returnValue += (bonusPerCorrectStreak * bonusStreak);
    }
    else
        returnValue += (bonusForCorrectAnswer - currentStreak);

    // ensure return value isn't negative
    return (returnValue > 0 ? returnValue : 0);
}

int flashCard::maxPossibleScore()
{
    return (bonusForCorrectAnswer +
        (bonusPerCorrectStreak * maxBonusStreakLength) +
         bonusPerKnownLevel * (level_max - 1));
}

void flashCard::setAnswerTime(int time)
{
    answerTime = time;
}

int flashCard::getTimeLimit()
{
    //default for unanswered cards
    if (score() == 0 || answerTime == DEFAULT_TIME_LIMIT)
        return DEFAULT_TIME_LIMIT;
    //steadily decreasing time limits for up to half max score
    else if (score() < (maxPossibleScore() / 2))
        return ( (DEFAULT_TIME_LIMIT * score()) / maxPossibleScore() );
    //over half score, the time limit depends on how quickly they answed last time
    else if (score() != maxPossibleScore())
        return (answerTime + TIME_LIMIT_MARGIN);
    //at max score, each answer must be quicker than the last!!
    else
        return(answerTime);
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

bool flashCard::hasInfo()
{
    return ! info.isEmpty();
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

bool flashCard::hasHint()
{
    return ! hint.isEmpty();
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

bool flashCard::containsCharacter(char charToFind)
{
    return question.contains(charToFind) ||
           answer.contains(charToFind) ||
           info.contains(charToFind) ||
           hint.contains(charToFind);
}

void flashCard::replaceCharacter(char charToReplace, char replaceWith)
{
    question.replace(charToReplace,replaceWith);
    answer.replace(charToReplace,replaceWith);
    info.replace(charToReplace,replaceWith);
    hint.replace(charToReplace,replaceWith);
}

//priority adjustment functions
bool flashCard::canBePromoted()
{
    if (wasCorrectLastTime() == false) return false;
    if (knownLevel == level_max) return false;

    int requiredLevelUp = INT_MAX;

    if (knownLevel == level_n2l)
        requiredLevelUp = n2lToNorm;
    else if (knownLevel == level_norm)
        requiredLevelUp = normToKnown;
    else if (knownLevel == level_known)
        requiredLevelUp = knownToOld;

    if (levelUp >= requiredLevelUp)
        return true;
    else
        return false;
}

bool flashCard::canBeDemoted()
{
    if (wasCorrectLastTime()) return false;
    if (knownLevel == level_min) return false;

    if (knownLevel == level_norm && parentPack->knownLevelSets[level_min].isEmpty())
        return true;//special case - if no card marked need2learn

    int requiredLevelUp = INT_MAX;

    if (knownLevel == level_norm)
        requiredLevelUp = normToN2l;
    else if (knownLevel == level_known)
        requiredLevelUp = knownToNorm;
    else if (knownLevel == level_old)
        requiredLevelUp = oldToNorm;

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


knownLevel_t flashCard::getKnownLevel()
{
    return knownLevel;
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

bool flashCard::isDuplicateOf(flashCard * otherCard)
{
    return (question == otherCard->question);
}

unsigned int flashCard::levenshteinDistance(const QString & yourAnswer)
{
    // I like the levenshtein distance string metric, so I'm using it in this class :-)
    // Implementation provided by wikibooks, 02/03/2012
    // (http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein_distance#C.2B.2B)

    const unsigned int len1 = yourAnswer.length(), len2 = answer.length();
    std::vector<int> col(len2+1), prevCol(len2+1);

    for (unsigned int i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++)
    {
        col[0] = i+1;
        for (unsigned int j = 0; j < len2; j++)
            col[j+1] = std::min( std::min( 1 + col[j], 1 + prevCol[1 + j]),
                            prevCol[j] + (yourAnswer[i]==answer[j] ? 0 : 1) );
        col.swap(prevCol);
    }
    qDebug("Levenshtein distance: %i",prevCol[len2]);
    return prevCol[len2];
}