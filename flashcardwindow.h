#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QDialog>
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

private slots:
    void on_infoButton_clicked();

    void on_hintButton_clicked();

    void on_answerOKButton_clicked();

private:
    Ui::flashCardWindow *ui;
    flashCard * currentCard;
    bool usedHintThisTime;
    void anotherCard();
};

extern flashCardWindow tester;

#endif // FLASHCARDWINDOW_H
