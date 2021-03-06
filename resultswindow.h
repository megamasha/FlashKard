#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include "flashcard.h"
#include "cardpack.h" //for default arg 2 of constructor
#include "flashcardwindow.h" //parent

namespace Ui {
    class resultsWindow;
}

class resultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit resultsWindow(flashCardWindow *parent = 0,
                           flashCard * card = mainPack.getFirstCard(),
                           QString givenAnswer = QString());

    ~resultsWindow();

    void processResults(QString givenAnswer);

private slots:
    void on_stopButton_clicked();

    void on_continueButton_clicked();

    void on_editButton_clicked();

private:

    void generateResultsText(QString & givenAnswer);

    Ui::resultsWindow *ui;
    flashCard * currentCard;
    QString resultsText;
};

#endif // RESULTSWINDOW_H
