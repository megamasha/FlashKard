#include "carddatabasemodel.h"
#include "cardpack.h"
#include "flashcard.h"

cardDatabaseModel::cardDatabaseModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int cardDatabaseModel::rowCount ( const QModelIndex & parent) const
{
    if (parent != QModelIndex())
        return 0;
    else
        return mainPack.cardsInPack();
}

int cardDatabaseModel::columnCount ( const QModelIndex & parent) const
{
    if (parent != QModelIndex())
        return 0;
    else
        return 4;
}

QVariant cardDatabaseModel::data ( const QModelIndex & index, int role ) const
{
    //first use the row to get the right card
    flashCard * currentCard = NULL;
    QVariant returnValue = QVariant();
    if (index.row() < mainPack.cardsInPack())
        currentCard = mainPack.getCardByIndex(index.row() + 1);
    else
        return returnValue;

    // check they want a string or index int (as, right now, that's all we do)
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return returnValue;

    // now return the index int
    if (role == Qt::EditRole)
        return QVariant(index.row() + 1);

    // or use the column to retreive the right string
    switch (index.column())
    {
        case 0: returnValue = currentCard->getQuestion();
                break;
        case 1: returnValue = currentCard->getAnswer();
                break;
        case 2: returnValue = currentCard->getInfo();
                break;
        case 3: returnValue = currentCard->getHint();
                break;
        default:
                break;
    } //switch

    return returnValue;
}