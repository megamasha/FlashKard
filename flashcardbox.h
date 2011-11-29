#ifndef FLASHCARDBOX_H
#define FLASHCARDBOX_H

#include <QDialog>

namespace Ui {
    class flashCardBox;
}

class flashCardBox : public QDialog
{
    Q_OBJECT

public:
    explicit flashCardBox(QWidget *parent = 0);
    ~flashCardBox();

private:
    Ui::flashCardBox *ui;
};

#endif // FLASHCARDBOX_H
