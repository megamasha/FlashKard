#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QObject> // required for Qt Signals and Slots

#define BONUS_FOR_CORRECT_ANSWER 3
#define BONUS_PER_CORRECT_STREAK 1
#define MAX_BONUS_STREAK_LENGTH 5
#define BONUS_PER_KNOWN_LEVEL 1

enum knownLevel_t {level_min = 0, level_n2l = 0, level_norm = 1, level_known = 2, level_old = 3, level_max = 3};

class cardSet;
class cardPack;

class flashCard : public QObject // required for Qt Signals and Slots
{
    Q_OBJECT // required for Qt Signals and Slots

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

        //functions to do with answer correctness
        bool isCorrect (QString & yourAnswer);
        // bool isAlmostCorrect (QString & yourAnswer); //FISH! TODO
        bool wasCorrectLastTime();
        int getCurrentStreak();
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

        //priority adjustment functions
        bool canBePromoted();
        bool canBeDemoted();
        bool promoteCard();
        bool demoteCard();

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
        cardSet * parentSet;
        cardPack * parentPack;

    //friends:

        /*cardSet required for adjusting flashCard.next
        list functionality could be moved into a list class, removing
        the need for friendship, removing the .next member, and allowing
        a clashcard to be in more than one list, but making list calls
        more complex*/

        friend class cardSet;

        /*cardPack required for promoting and demoting cards*/
        //possibly not any more

        friend class cardPack;

        /*editorWindow is a friend because it is the designated class for
          allowing the user to change the card*/

        friend class editorWindow;
};

#endif // FLASHCARD_H
