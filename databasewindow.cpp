#include "databasewindow.h"
#include "ui_databasewindow.h"
#include "carddatabasemodel.h"
#include "carddatabaseproxymodel.h"
#include "editorwindow.h"
#include "popupwindow.h"
#include "cardpack.h"

databaseWindow::databaseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::databaseWindow)
{
    ui->setupUi(this);

    model = new cardDatabaseModel;
    proxyModel = new cardDatabaseProxyModel;
    proxyModel->setSourceModel(model);

    ui->databaseTreeView->setModel(proxyModel);

    connect(model,SIGNAL(cardAdded(flashCard*)),this,SLOT(itemJustAdded(flashCard*)));
    connect(ui->searchText,SIGNAL(textChanged(QString)),proxyModel,SLOT(setFilterFixedString(QString)));

    enableAndDisableButtons();
}

databaseWindow::~databaseWindow()
{
    delete proxyModel;
    delete model;
    delete ui;
}

void databaseWindow::on_editButton_clicked()
{
    if (ui->databaseTreeView->selectionModel()->hasSelection())
    {
        QModelIndex index = ui->databaseTreeView->selectionModel()->currentIndex();
        on_databaseTreeView_doubleClicked(index);
    }
}

void databaseWindow::on_addButton_clicked()
{
    ui->databaseTreeView->model()->insertRow(
                ui->databaseTreeView->model()->rowCount()
                );
    enableAndDisableButtons();
}

void databaseWindow::on_deleteButton_clicked()
{
    if (popup.importantQuestion(this,tr("Are you sure you want to permanently delete this flashcard from the database?")))
    {
        QModelIndex index = ui->databaseTreeView->selectionModel()->currentIndex();
        ui->databaseTreeView->model()->removeRow(index.row());
        enableAndDisableButtons();
    }
}

void databaseWindow::on_closeButton_clicked()
{
    accept();
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

void databaseWindow::itemJustAdded(flashCard *addedCard)
{
    editorWindow editor(this,addedCard);
    editor.exec();
}

void databaseWindow::enableAndDisableButtons()
{
    if (mainPack.isEmpty())
    {
        ui->deleteButton->setDisabled(true);
        ui->editButton->setDisabled(true);
    }
    else
    {
        ui->deleteButton->setEnabled(true);
        ui->editButton->setEnabled(true);
    }
}
