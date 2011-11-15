#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>
#include <QObject> // required for Qt Signals and Slots
using namespace std;

class flashcard : public QObject
{
    Q_OBJECT // required for Qt Signals and Slots

    public:
        bool isCorrect (string);
        // bool isAlmostCorrect (string); //FISH! TODO

        string getQuestion();
        bool setQuestion(string);
        string getAnswer();
        bool setAnswer(string);
        string getInfo();
        bool setInfo(string);
        string getHint();
        bool setHint(string);

        bool wasCorrectLastTime();

    private:
        string question;
        string answer;
        string info;
        string hint;
        bool lastCorrect;
        int currentStreak;
        int levelUp;
        int knownlevel;
};

#endif // FLASHCARD_H
