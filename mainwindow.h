#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_exitButton_clicked();

    void on_loadButton_clicked();

    void on_testButton_clicked();

    void on_databaseButton_clicked();

    void on_saveButton_clicked();

    void on_statsButton_clicked();

private:

    Ui::MainWindow *ui;

    void loadFML();
    void saveFML();
    void loadFDB();
    void saveFDB();

    void enableAndDisableButtons();

    QString currentlyLoadedFilename;
};

extern MainWindow mainWindow;

#endif // MAINWINDOW_H
