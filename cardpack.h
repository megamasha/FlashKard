#ifndef CARDPACK_H
#define CARDPACK_H

#include <QObject>
#include "flashcard.h"

class cardpack : public QObject
{
    Q_OBJECT
    public:
        explicit cardpack(QObject *parent = 0);

        bool importTsv(const QString);
        flashcard * getRandom();
        flashcard * getNext(const flashcard *);

    signals:

    public slots:

};

#endif // CARDPACK_H
