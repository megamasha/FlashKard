#include "editorwindow.h"
#include "ui_editorwindow.h"

editorWindow::editorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editorWindow)
{
    ui->setupUi(this);
}

editorWindow::~editorWindow()
{
    delete ui;
}
