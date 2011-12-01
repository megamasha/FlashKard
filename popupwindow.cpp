#include "popupwindow.h"

#include <QMessageBox>

popupWindow popup;

popupWindow::popupWindow(QObject *parent) :
    QObject(parent)
{
}

bool popupWindow::getYesOrNo(QWidget * parent, QString message)
{
    QMessageBox::StandardButton userResponse;
    userResponse = QMessageBox::question(parent, tr("Question"),message,
                                QMessageBox::Yes | QMessageBox::No);
    if (userResponse == QMessageBox::Yes)
        return true;
    else
        return false;
}

bool popupWindow::importantQuestion(QWidget * parent, QString message)
{
    QMessageBox::StandardButton userResponse;
    userResponse = QMessageBox::warning(parent, tr("Important"),message,
                                QMessageBox::Yes | QMessageBox::No);
    if (userResponse == QMessageBox::Yes)
        return true;
    else
        return false;
}

void popupWindow::error(QWidget * parent, QString message)
{
    QMessageBox::critical(parent, tr("Error"),message, QMessageBox::Ok);

    return;
}

void popupWindow::info(QWidget * parent, QString message)
{
    QMessageBox::information(parent, tr("Info"),message, QMessageBox::Ok);

    return;
}
