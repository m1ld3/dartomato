#include "global_segment_stats_cricket_model.h"

CGlobalSegmentStatsCricketModel::CGlobalSegmentStatsCricketModel(std::array<uint32_t, static_cast<int>(CStatsWindowCricket::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mSegments(iSegments)
{}

int CGlobalSegmentStatsCricketModel::rowCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 7;
}

int CGlobalSegmentStatsCricketModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGlobalSegmentStatsCricketModel::data(const QModelIndex & iIndex, int iRole) const
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
      return QString::number(mSegments.at(iIndex.row() + 7));
    }
    else if (iIndex.column() == 4)
    {
      return mCol4Labels.at(iIndex.row());
    }
    else
    {
      return QString::number(mSegments.at(iIndex.row() + 14));
    }
  }
  else
  {
    return QVariant();
  }
}

