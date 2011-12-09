#include "databasewindow.h"
#include "ui_databasewindow.h"
#include "carddatabasemodel.h"
#include "editorwindow.h"

databaseWindow::databaseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::databaseWindow)
{
    ui->setupUi(this);

    cardDatabaseModel * model = new cardDatabaseModel;
    ui->databaseTreeView->setModel(model);
}

databaseWindow::~databaseWindow()
{
    delete ui;
}

void databaseWindow::on_editButton_clicked()
{

}

void databaseWindow::on_addButton_clicked()
{

}

void databaseWindow::on_deleteButton_clicked()
{

}

void databaseWindow::on_closeButton_clicked()
{

}

flashCard * databaseWindow::indexToCard(const QModelIndex & index)
{
    // use model index to retreive flashcard index,
    //and use that index to retrieve card
    int cardIndex =
            ui->databaseTreeView->model()->data(index,Qt::EditRole).toInt();
    return mainPack.getCardByIndex(cardIndex);
}

void databaseWindow::on_databaseTreeView_doubleClicked(const QModelIndex &index)
{
    //convert index back into flashcard pointer:
    flashCard * card = indexToCard(index);

    editorWindow editor(this,card);
    editor.exec();
}
