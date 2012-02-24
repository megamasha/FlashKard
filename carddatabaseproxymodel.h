#ifndef CARDDATABASEPROXYMODEL_H
#define CARDDATABASEPROXYMODEL_H

#include <QSortFilterProxyModel>

class cardDatabaseProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit cardDatabaseProxyModel(QObject *parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

signals:

public slots:

};

#endif // CARDDATABASEPROXYMODEL_H
