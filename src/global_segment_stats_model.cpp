#include "global_segment_stats_model.h"

CGlobalSegmentStatsModel::CGlobalSegmentStatsModel(std::array<uint32_t, static_cast<int>(CStatsWindow::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mSegments(iSegments)
{}

int CGlobalSegmentStatsModel::rowCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 10;
}

int CGlobalSegmentStatsModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGlobalSegmentStatsModel::data(const QModelIndex &iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return mCol0Labels.at(iIndex.row());
    }
    else if (iIndex.column() == 1)
    {
      return QString::number(mSegments.at(iIndex.row()));
    }
    else if (iIndex.column() == 2)
    {
      return mCol2Labels.at(iIndex.row());
    }
    else if (iIndex.column() == 3)
    {
      return QString::number(mSegments.at(iIndex.row() + 10));
    }
    else if (iIndex.column() == 4)
    {
      return (iIndex.row() < 3) ? mCol4Labels.at(iIndex.row()) : QVariant();
    }
    else
    {
      return (iIndex.row() < 3) ? QString::number(mSegments.at(iIndex.row() + 20)) : QVariant();
    }
  }
  else
  {
    return QVariant();
  }
}

