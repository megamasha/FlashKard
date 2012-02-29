#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include <QSettings>

#define DEFAULT_POPUP_INTERVAL 10

namespace Ui {
    class preferencesWindow;
}

class preferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit preferencesWindow(QWidget *parent = 0);
    ~preferencesWindow();

private slots:
    void on_buttons_accepted();

    void on_popupModeBox_toggled(bool checked);

    void on_resetButton_clicked();

private:
    Ui::preferencesWindow *ui;
    QSettings settings;
};

#endif // PREFERENCESWINDOW_H
