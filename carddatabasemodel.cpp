#include "carddatabasemodel.h"
#include "cardpack.h"
#include "flashcard.h"
#include <QSize>

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

QVariant cardDatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant returnValue = QVariant();

    //range check
    if (section >= columnCount())
        return returnValue;

    if (role == Qt::SizeHintRole)
    {
        returnValue = QSize(20,20);
    }

    if (role == Qt::DisplayRole)
    {
        //this only does top (horizontal) header
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: returnValue = QString("Question");
                break;
            case 1: returnValue = QString("Answer");
                break;
            case 2: returnValue = QString("Info");
                break;
            case 3: returnValue = QString("Hint");
                break;
            default: returnValue = section;
            }
        }
    }

    return returnValue;
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

bool cardDatabaseModel::insertRows(int, int count, const QModelIndex &parent)
{
    bool success = true;

    beginInsertRows(parent,mainPack.cardsInPack(),mainPack.cardsInPack()+count-1);
    for (int i = 0;i<count;i++)
    {
        flashCard * card = new flashCard;
        success = success &
                mainPack.addCard(*card,level_norm);
    }
    endInsertRows();
    return success;
}

bool cardDatabaseModel::removeRows (int row,int count,const QModelIndex & parent)
{
    beginRemoveRows (parent,row,row+count-1);
    for (int i = 0;i<count;i++)
    {
        flashCard * cardToBeRemoved = mainPack.getCardByIndex(row+1);
        mainPack.removeCard(cardToBeRemoved);
    }
    endRemoveRows();
    return true;
}
