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

bool popupWindow::dealWithChanges(MainWindow * parent)
{
    QMessageBox box(parent);
    box.setIcon(QMessageBox::Question);
    box.setText("You have unsaved changes or Progress.");
    box.setInformativeText("Save changes before quitting?");
    box.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Save);
    box.setEscapeButton(QMessageBox::Cancel);

    int buttonClicked = box.exec();
    switch (buttonClicked)
    {
    case QMessageBox::Cancel:
        return false;
        break;
    case QMessageBox::Discard:
        return true;
        break;
    case QMessageBox::Save:
        return parent->on_saveButton_clicked();
        break;
    default:
        return false;
    }
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
