#ifndef FLASHCARDTRAYICON_H
#define FLASHCARDTRAYICON_H

#include <QSystemTrayIcon>
#include <QTimer>

class flashKardTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit flashKardTrayIcon(QObject *parent = 0);

signals:

public slots:

    void setPopupMode(bool setMode);
    void popupFlashcard();
    void clicked(QSystemTrayIcon::ActivationReason);

private:

    bool popupModeActive;
    QTimer popupTimer;

};

#endif // FLASHCARDTRAYICON_H
