#include "databasewindow.h"
#include "ui_databasewindow.h"
#include "carddatabasemodel.h"

databaseWindow::databaseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::databaseWindow)
{
    ui->setupUi(this);
    QAbstractTableModel * model = new cardDatabaseModel;
    ui->databaseTableView->setModel(model);
    /* This code is superfluous and can probably be removed
    int numRows = model->rowCount(QModelIndex());
    int numColumns = model->columnCount(QModelIndex());
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numColumns; column++)
        {
            QModelIndex index = model->index(row, column, parentIndex);
            QString text = model->data(index, Qt::DisplayRole).toString();
            ui->databaseTableView->
        }//columns
    }//rows
    */
}

databaseWindow::~databaseWindow()
{
    delete ui;
}
