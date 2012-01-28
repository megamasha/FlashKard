#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication flashKard(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return flashKard.exec();
}
