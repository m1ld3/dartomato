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

QVariant CGlobalGameStatsCricketModel::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount({}) || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return "Hits Per Round:";
    }
    if (iIndex.column() == 1)
    {
      return QString::number(mData.HitsPerRound, 'f', 2);
    }
    if (iIndex.column() == 2)
    {
      return "Total Hits:";
    }
    if (iIndex.column() == 3)
    {
      return QString::number(mData.TotalHits);
    }
    if (iIndex.column() == 4)
    {
      return "Legs Won:";
    }
    return QString::number(mData.LegsWon) + " of " + QString::number(mData.NumLegs);
  }
  return {};
}
