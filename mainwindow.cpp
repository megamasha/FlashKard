#include <QString>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svfileops.h"
#include "flashcardbox.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
                                            "~/Documents", tr("Files (*.*)"));
    QByteArray poop = filename.toAscii();
    char * importfilename = (char *)malloc(256);
    importfilename = poop.data();

    importdatabase(importfilename);

}

void MainWindow::on_testButton_clicked()
{
   flashCardBox tester;
    tester.exec();
}

