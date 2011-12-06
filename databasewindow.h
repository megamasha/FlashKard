#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QDialog>

namespace Ui {
    class databaseWindow;
}

class databaseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit databaseWindow(QWidget *parent = 0);
    ~databaseWindow();

private:
    Ui::databaseWindow *ui;
};

#endif // DATABASEWINDOW_H
