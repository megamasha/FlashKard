#ifndef CARDPACK_H
#define CARDPACK_H

#include <QObject>
#include "flashcard.h"

class cardPack : public QObject
{
    Q_OBJECT
    public:
        explicit cardPack(QObject *parent = 0);

        bool importTsv(const QString);
        flashCard * getRandom();
        flashCard * getNext(const flashCard *);

    signals:

    public slots:

};

#endif // CARDPACK_H
