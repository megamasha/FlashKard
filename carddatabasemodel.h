#ifndef CARDDATABASEMODEL_H
#define CARDDATABASEMODEL_H

#include <QAbstractTableModel>

class flashCard;

class cardDatabaseModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit cardDatabaseModel(QObject *parent = 0);
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

signals:

    void cardAdded(flashCard * addedCard);

public slots:

};

#endif // CARDDATABASEMODEL_H
