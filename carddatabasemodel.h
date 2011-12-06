#ifndef CARDDATABASEMODEL_H
#define CARDDATABASEMODEL_H

#include <QAbstractTableModel>

class cardDatabaseModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit cardDatabaseModel(QObject *parent = 0);
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

signals:

public slots:

};

#endif // CARDDATABASEMODEL_H
