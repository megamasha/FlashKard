#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include "flashcard.h"

namespace Ui {
    class resultsWindow;
}

class resultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit resultsWindow(QWidget *parent = 0);
    ~resultsWindow();

    bool processResults(flashCard & currentCard, QString givenAnswer);

private slots:
    void on_stopButton_clicked();

    void on_continueButton_clicked();

private:
    Ui::resultsWindow *ui;
    QString resultsText;
};

#endif // RESULTSWINDOW_H
