#include "global_segment_stats_x01_model.h"

CGlobalSegmentStatsX01Model::CGlobalSegmentStatsX01Model(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mSegments(iSegments)
{}

int CGlobalSegmentStatsX01Model::rowCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 10;
}

int CGlobalSegmentStatsX01Model::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGlobalSegmentStatsX01Model::data(const QModelIndex &iIndex, int iRole) const
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

