#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QObject> // required for Qt Signals and Slots

enum knownLevel_t {level_min = 0, level_n2l = 0, level_norm = 1, level_known = 2, level_old = 3, level_max = 3};

class flashCard : public QObject // required for Qt Signals and Slots
{
    Q_OBJECT // required for Qt Signals and Slots

    public:

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
        bool markAsCorrect();
        bool markAsIncorrect();

        //string access functions
        QString getQuestion();
        bool setQuestion(QString & newQuestion);
        QString getAnswer();
        bool setAnswer(QString & newAnswer);
        QString getInfo();
        bool setInfo(QString & newInfo);
        QString getHint();
        bool setHint(QString & newHint);

        //priority adjustment functions
        bool resetKnownLevel();
        bool setAsHighPriorityToLearn();
        bool setAsLowPriorityToLearn();

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

    //friends:

        /*cardSet required for adjusting flashCard.next
        list functionality could be moved into a list class, removing
        the need for friendship, removing the .next member, and allowing
        a clashcard to be in more than one list, but making list calls
        more complex*/

        friend class cardSet;

        /*cardPack required for promoting and demoting cards*/

        friend class cardPack;
};

#endif // FLASHCARD_H
