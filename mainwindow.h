#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "flashkardtrayicon.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool currentlyTesting() {return testingFlag;}
    void setTesting(bool setTo) {testingFlag = setTo;}


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_exitButton_clicked();

    void on_loadButton_clicked();

    void loadRecent();

    void on_testButton_clicked();

    void on_databaseButton_clicked();

    void on_saveButton_clicked();

    void on_statsButton_clicked();

    void on_actionSave_FlashKard_Database_As_triggered();

    void on_actionMerge_from_FlashKard_Database_triggered();

    void on_actionClear_Pack_triggered();

    void on_actionRemove_Duplicates_triggered();

    void on_actionConfigure_FlashKard_triggered();

private:

    Ui::MainWindow *ui;

    void loadFile(QString fileName);

    void loadFML();
    void saveFML();
    void loadFDB();
    void saveFDB();

    void enableAndDisableButtons();

    bool testingFlag;

    void updateRecentFiles();

    void applyPreferences();

    QString currentlyLoadedFilename;

    flashKardTrayIcon trayIcon;
};

extern MainWindow mainWindow;

#endif // MAINWINDOW_H
