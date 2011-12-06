#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QDialog>

namespace Ui {
    class editorWindow;
}

class editorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit editorWindow(QWidget *parent = 0);
    ~editorWindow();

private:
    Ui::editorWindow *ui;
};

#endif // EDITORWINDOW_H
