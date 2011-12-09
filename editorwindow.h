#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QDialog>
#include "flashcard.h"
#include "cardpack.h" //for default value of currentCard

namespace Ui {
    class editorWindow;
}

class editorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit editorWindow(QWidget *parent = 0,
                          flashCard * card = mainPack.getFirstCard());
    ~editorWindow();

protected:
    void closeEvent(QCloseEvent *event) {/*FISH! TODO*/}

private slots:
    void on_markN2lButton_clicked();

    void on_markKnownButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::editorWindow *ui;

    QString createStreakText();
    QString createLevelText();

    flashCard * currentCard;
};

#endif // EDITORWINDOW_H
