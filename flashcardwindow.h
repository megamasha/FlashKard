#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QDialog>
#include <QTimer>
#include "flashcard.h"

namespace Ui {
    class flashCardWindow;
}

class flashCardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit flashCardWindow(QWidget *parent = 0);
    ~flashCardWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_infoButton_clicked();

    void on_hintButton_clicked();

    void on_answerOKButton_clicked();

    void progressTimer();

private:
    Ui::flashCardWindow *ui;
    flashCard * currentCard;
    bool answered; //are we expecting an answer, or just waiting around?
    bool usedHintThisTime;
    void anotherCard();

    QTimer timer;
    int currentTimerProgress;
};

extern flashCardWindow tester;

#endif // FLASHCARDWINDOW_H
