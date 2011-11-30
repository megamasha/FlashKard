#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QDialog>

namespace Ui {
    class flashCardWindow;
}

class flashCardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit flashCardWindow(QWidget *parent = 0);
    ~flashCardWindow();

private:
    Ui::flashCardWindow *ui;
};

#endif // FLASHCARDWINDOW_H
