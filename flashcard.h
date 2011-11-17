#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QObject> // required for Qt Signals and Slots

class flashCard : public QObject
{
    Q_OBJECT // required for Qt Signals and Slots

    public:
        bool isCorrect (QString);
        // bool isAlmostCorrect (QString); //FISH! TODO

        QString getQuestion();
        bool setQuestion(QString);
        QString getAnswer();
        bool setAnswer(QString);
        QString getInfo();
        bool setInfo(QString);
        QString getHint();
        bool setHint(QString);

        bool wasCorrectLastTime();

    private:
        QString question;
        QString answer;
        QString info;
        QString hint;
        bool lastCorrect;
        int currentStreak;
        int levelUp;
        int knownLevel;
};

#endif // FLASHCARD_H
