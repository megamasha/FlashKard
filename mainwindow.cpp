#include <QString>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svfileops.h"
#include "cardpack.h"
#include "flashcardwindow.h"
#include "popupwindow.h"
#include "databasewindow.h"
#include "statswindow.h"
#include <QCloseEvent>

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

void MainWindow::closeEvent(QCloseEvent * event)
{
    //if no changes have been made, the program can be safely closed
    if ( !mainPack.hasUnsavedChanges() )
        event->accept();

    //otherwise ask whether user wants to save changes
    else if (popup.importantQuestion(this,tr("Save changes before quitting?")))
    {
        on_saveButton_clicked();
        event->accept();
    }
    // if user doesn't want to save changes, just quit
    else
        event->accept();
}

void MainWindow::on_exitButton_clicked()
{
    //if non-empty pack has unsaved changes that eh user wants to save...
    if ( mainPack.hasUnsavedChanges() &&
         ! mainPack.isEmpty() &&
         popup.importantQuestion(this,tr("Save changes before quitting?")) )
    {
        //...then save
        on_saveButton_clicked();
    }

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

    mainPack.setChanged();
    flashCardWindow tester;
    tester.exec();
}

void MainWindow::on_databaseButton_clicked()
{
    mainPack.setChanged();
    databaseWindow database;
    database.exec();
    enableAndDisableButtons();
}

void MainWindow::on_saveButton_clicked()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this, tr("Save Flashcard Database"),
                                            "~/Documents", tr("Files (*.~sv)"));
    mainPack.exportdatabase(filename.toAscii().data());
    mainPack.SetUnchanged();
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
void MainWindow::on_statsButton_clicked()
{
    StatsWindow statsWindow;
    statsWindow.exec();
}
