#ifndef FLASHCARD_H
#define FLASHCARD_H

#define BONUS_FOR_CORRECT_ANSWER 3
#define BONUS_PER_CORRECT_STREAK 1
#define MAX_BONUS_STREAK_LENGTH 5
#define BONUS_PER_KNOWN_LEVEL 1

#define DEFAULT_ALMOST_CORRECT_THRESHOLD 85

#define DEFAULT_TIME_LIMIT 30000 //in milliseconds
#define TIME_LIMIT_MARGIN 3000 //how much slower (in milliseconds) the card may be answered than last time

#include <QString>

enum knownLevel_t {level_min = 0, level_n2l = 0, level_norm = 1, level_known = 2, level_old = 3, level_max = 3};

class cardSet;
class cardPack;

class flashCard
{
    public:

        flashCard();
        flashCard (
            QString qu,
            QString an,
            QString in,
            QString hi,
            knownLevel_t kl,
            bool lc,
            int cs,
            int lu);
        flashCard & operator=(const flashCard & source);

        //functions to do with answer correctness
        bool isCorrect (QString & yourAnswer);
        bool isAlmostCorrect (QString & yourAnswer);
        bool wasCorrectLastTime();
        int getCurrentStreak();
        int getLevelUp();
        bool markAsCorrect();
        bool markAsIncorrect();

        //other testing-related functions
        int score();
        static int maxPossibleScore();
        void setAnswerTime(int time);
        int getTimeLimit();

        //string access functions
        QString getQuestion();
        bool setQuestion(QString newQuestion);
        QString getAnswer();
        bool setAnswer(QString newAnswer);
        bool hasInfo();
        QString getInfo();
        bool setInfo(QString newInfo);
        bool hasHint();
        QString getHint();
        bool setHint(QString newHint);

        //string adjustment functions
        bool containsCharacter(char charToFind);
        void replaceCharacter(char charToReplace, char replaceWith);

        //priority adjustment functions
        bool canBePromoted();
        bool canBeDemoted();
        bool promoteCard();
        bool demoteCard();

        knownLevel_t getKnownLevel();
        bool resetKnownLevel();
        bool isHighPriority();
        bool setHighPriority();
        bool isLowPriority();
        bool setLowPriority();

        bool isDuplicateOf(flashCard * otherCard);

        //static members
        static int n2lToNorm;
        static int normToN2l;
        static int normToKnown;
        static int knownToNorm;
        static int knownToOld;
        static int oldToNorm;

        static int bonusForCorrectAnswer;
        static int bonusPerCorrectStreak;
        static int maxBonusStreakLength;
        static int bonusPerKnownLevel;


    private:

        QString question;
        QString answer;
        QString info;
        QString hint;

        knownLevel_t knownLevel;
        bool lastCorrect;
        int currentStreak;
        int levelUp;

        int answerTime;

        flashCard * next;
        cardPack * parentPack;
        cardSet * parentSet;

        unsigned int levenshteinDistance (const QString & yourAnswer); //used for isAlmostCorrect()

    //friends:

        /*cardSet required for adjusting flashCard.next
        list functionality could be moved into a list class, removing
        the need for friendship, removing the .next member, and allowing
        a clashcard to be in more than one list, but making list calls
        more complex*/
        friend class cardSet;

        /* cardPack (was at one point) required for promoting and demoting cards
         * and replacing characters*/
        friend class cardPack;

        /*editorWindow is a friend because it is the designated class for
          allowing the user to change the card*/
        friend class editorWindow;

        /*FLMHandler constructs flashCards from a (hopefully) well-formed
          xml file*/
        friend class FMLHandler;

        /*the static members of flashcard can be adjusted from the preferences window
         *they are then applied from the main window*/
        friend class MainWindow;
};

#endif // FLASHCARD_H
