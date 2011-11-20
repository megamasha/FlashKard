#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QObject> // required for Qt Signals and Slots

enum knownLevel_t {level_n2l = 0, level_norm, level_known, level_old};

class flashCard : public QObject // required for Qt Signals and Slots
{
    Q_OBJECT // required for Qt Signals and Slots

    public:

        flashCard(QString qu, QString an, QString in, QString hi, knownLevel_t kl, int lc, int cs, int lu);
        bool isCorrect (QString);
        // bool isAlmostCorrect (QString); //FISH! TODO

        QString getQuestion();
        bool setQuestion(QString & newQuestion);
        QString getAnswer();
        bool setAnswer(QString & newAnswer);
        QString getInfo();
        bool setInfo(QString & newInfo);
        QString getHint();
        bool setHint(QString & newHint);

        bool wasCorrectLastTime();

        friend class cardSet;

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
};

#endif // FLASHCARD_H
