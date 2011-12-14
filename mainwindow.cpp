#include <QString>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svfileops.h"
#include "cardpack.h"
#include "flashcardwindow.h"
#include "popupwindow.h"
#include "databasewindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    enableAndDisableButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    exit(EXIT_SUCCESS);
}

void MainWindow::on_loadButton_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Load Flashcard Database"),
                                            "~/Documents", tr("Files (*.~sv)"));
    QByteArray poop = filename.toAscii();
    char * importfilename = (char *)malloc(256);
    importfilename = poop.data();

    importdatabase(importfilename);

    enableAndDisableButtons();
}

void MainWindow::on_testButton_clicked()
{
    if (mainPack.isEmpty())
        on_loadButton_clicked();

    while (mainPack.isEmpty())
    {
        if(popup.importantQuestion(this,tr("Nothing to test!\n\nLoad flashcards?")))
            on_loadButton_clicked();
        else
            return;
    }

    flashCardWindow tester;
    tester.exec();
}

void MainWindow::on_databaseButton_clicked()
{
    databaseWindow database;
    database.exec();
    enableAndDisableButtons();
}

void MainWindow::on_saveButton_clicked()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this, tr("Save Flashcard Database"),
                                            "~/Documents", tr("Files (*.~sv)"));
    qDebug("Filename: %s",filename.toAscii().data());
    mainPack.isEmpty();
    mainPack.exportdatabase(filename.toAscii().data());

}

void MainWindow::enableAndDisableButtons()
{
    if (mainPack.isEmpty())
    {
        ui->saveButton->setDisabled(true);
        ui->statsButton->setDisabled(true);
    }
    else
    {
        ui->saveButton->setEnabled(true);
        ui->statsButton->setEnabled(true);
    }
}