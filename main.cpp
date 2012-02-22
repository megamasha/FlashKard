#include <QtGui/QApplication>
#include "mainwindow.h"
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication flashKard(argc, argv);

    srand((unsigned)time(NULL));

    MainWindow mainWindow;
    mainWindow.show();

    return flashKard.exec();
}
