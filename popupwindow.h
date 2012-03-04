#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QObject>
#include "mainwindow.h"

class popupWindow : public QObject
{
    Q_OBJECT
public:
    explicit popupWindow(QObject *parent = 0);

    bool getYesOrNo(QWidget * parent, QString message);
    bool importantQuestion(QWidget * parent, QString message);
    bool dealWithChanges(MainWindow * parent);
    void error(QWidget * parent, QString message);
    void info(QWidget * parent, QString message);

signals:

public slots:

};

extern popupWindow popup;

#endif // POPUPWINDOW_H
