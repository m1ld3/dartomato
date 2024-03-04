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
  return 4;
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
      return "Hits per Round:";
    }
    else if (iIndex.column() == 1)
    {
      return QString::number(mData.HitsPerRound, 'f', 2);
    }
    else if (iIndex.column() == 2)
    {
      return "Total Hits:";
    }
    else
    {
      return QString::number(mData.TotalHits);
    }
  }
  else
  {
    return QVariant();
  }
}
