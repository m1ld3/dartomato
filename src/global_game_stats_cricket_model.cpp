#include "global_game_stats_cricket_model.h"

CGlobalGameStatsCricketModel::CGlobalGameStatsCricketModel(const CStatsWindowCricket::SGlobalGameStatsData & iData, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mData(iData)
{}

int CGlobalGameStatsCricketModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 1;
}

int CGlobalGameStatsCricketModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGlobalGameStatsCricketModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return "Hits Per Round:";
    }
    else if (iIndex.column() == 1)
    {
      return QString::number(mData.HitsPerRound, 'f', 2);
    }
    else if (iIndex.column() == 2)
    {
      return "Total Hits:";
    }
    else if (iIndex.column() == 3)
    {
      return QString::number(mData.TotalHits);
    }
    else if (iIndex.column() == 4)
    {
      return "Legs Won:";
    }
    else
    {
      return QString::number(mData.LegsWon) + " of " + QString::number(mData.NumLegs);
    }
  }
  else
  {
    return QVariant();
  }
}
