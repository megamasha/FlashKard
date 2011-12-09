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

private:
    Ui::editorWindow *ui;
    flashCard * currentCard;
};

#endif // EDITORWINDOW_H
