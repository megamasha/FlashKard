#include <QString>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Load Flashcard Database"),
                                            "~/Documents", tr("Files (*.*)"));

}
