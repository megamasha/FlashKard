#include "flashkardtrayicon.h"
#include <QtGui>
#include <QSettings>
#include "flashcardwindow.h"
#include "cardpack.h"
#include "mainwindow.h"
#include "popupwindow.h" //included for debugging, can be removed
#include <QApplication>

flashKardTrayIcon::flashKardTrayIcon(QObject *parent) :
    QSystemTrayIcon(parent),
    popupModeActive(false)
{
    connect(&popupTimer,SIGNAL(timeout()),this,SLOT(popupFlashcard()));
    connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(clicked(QSystemTrayIcon::ActivationReason)));
}

void flashKardTrayIcon::setPopupMode(bool setMode)
{
    if (setMode == popupModeActive)
        return;

    if (setMode == true) //enabling
    {
        QSettings settings;

        int timerInterval = settings.value("Testing/popupInterval",10).toInt() * 60000;

        show();
        popupTimer.start(timerInterval);
        QApplication::setQuitOnLastWindowClosed(false);
        popupModeActive = true;
    }
    else //disabling
    {
        hide();
        popupTimer.stop();
        QApplication::setQuitOnLastWindowClosed(true);
        popupModeActive = false;
    }
}

void flashKardTrayIcon::popupFlashcard()
{
    if (!qobject_cast<MainWindow *>(parent())->currentlyTesting() && //guard against stacking multiple open cards if user leaves computer
        !qobject_cast<MainWindow *>(parent())->isVisible() && //only pop up if main window is not showing
        !mainPack.isEmpty()) //sanity check
    {
        flashCardWindow tester;
        tester.show();
        tester.showNormal();
        tester.exec();
    }
}

void flashKardTrayIcon::clicked(QSystemTrayIcon::ActivationReason)
{
    qobject_cast<QWidget *>(parent())->show();
}
