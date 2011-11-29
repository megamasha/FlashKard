#include "flashcardbox.h"
#include "ui_flashcardbox.h"

flashCardBox::flashCardBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flashCardBox)
{
    ui->setupUi(this);
}

flashCardBox::~flashCardBox()
{
    delete ui;
}
