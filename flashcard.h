#ifndef FLASHCARD_H
#define FLASHCARD_H

#define BONUS_FOR_CORRECT_ANSWER 3
#define BONUS_PER_CORRECT_STREAK 1
#define MAX_BONUS_STREAK_LENGTH 5
#define BONUS_PER_KNOWN_LEVEL 1

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
        // bool isAlmostCorrect (QString & yourAnswer); //FISH! TODO
        bool wasCorrectLastTime();
        int getCurrentStreak();
        int getLevelUp();
        bool markAsCorrect();
        bool markAsIncorrect();

        int score();
        static const int maxPossibleScore = (
                 BONUS_FOR_CORRECT_ANSWER +
                (BONUS_PER_CORRECT_STREAK * MAX_BONUS_STREAK_LENGTH) +
                 BONUS_PER_KNOWN_LEVEL * (level_max - 1));

        //string access functions
        QString getQuestion();
        bool setQuestion(QString newQuestion);
        QString getAnswer();
        bool setAnswer(QString newAnswer);
        QString getInfo();
        bool setInfo(QString newInfo);
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

    private:

        QString question;
        QString answer;
        QString info;
        QString hint;

        knownLevel_t knownLevel;
        bool lastCorrect;
        int currentStreak;
        int levelUp;

        flashCard * next;
        cardPack * parentPack;
        cardSet * parentSet;

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
};

#endif // FLASHCARD_H
