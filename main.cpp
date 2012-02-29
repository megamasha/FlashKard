#include <QtGui/QApplication>
#include "mainwindow.h"
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication flashKard(argc, argv);

    //used for saving persistent settings
    flashKard.setOrganizationName("MM");
    flashKard.setApplicationName("FlashKard");

    //for loading random cards
    srand((unsigned)time(NULL));

    MainWindow mainWindow;
    mainWindow.show();

    return flashKard.exec();
}
