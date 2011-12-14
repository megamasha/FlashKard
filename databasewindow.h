#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QDialog>
#include "carddatabasemodel.h"
#include "flashcard.h"

namespace Ui {
    class databaseWindow;
}

class databaseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit databaseWindow(QWidget *parent = 0);
    ~databaseWindow();

private slots:

    void on_editButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_closeButton_clicked();

    void on_databaseTreeView_doubleClicked(const QModelIndex &index);

private:

    Ui::databaseWindow *ui;

    flashCard * indexToCard(const QModelIndex & index);

    void enableAndDisableButtons();
};

#endif // DATABASEWINDOW_H
