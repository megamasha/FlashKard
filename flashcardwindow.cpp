#include "flashcardwindow.h"
#include "ui_flashcardwindow.h"

flashCardWindow::flashCardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flashCardWindow)
{
    ui->setupUi(this);
    ui->questionText->setText(QApplication::translate("flashcardBox", "Question", 0, QApplication::UnicodeUTF8));
}

flashCardWindow::~flashCardWindow()
{
    delete ui;
}
