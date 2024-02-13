#include "leg_stats_model.h"

CLegStatsModel::CLegStatsModel(CStatsWindow::SLegStatsData iLegStatsData, QObject * iParent)
  : QAbstractTableModel(iParent)
{
  mLegStatsData =
  {
    {"3-Dart Leg Average", iLegStatsData.Avg3Dart},
    {"1-Dart Leg Average", iLegStatsData.Avg1Dart},
    {"Average Darts Per Leg", iLegStatsData.AvgLegDartCount},
    {"Best Won Leg", iLegStatsData.BestWonLegDartCount},
    {"Worst Won Leg", iLegStatsData.WorstWonLegDartCount},
    {"Current Leg", iLegStatsData.DartCountOfCurrentLeg}
  };
}

int CLegStatsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mLegStatsData.size();
}

int CLegStatsModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mLegStatsData.size() || iIndex.column() >= 2)
  {
    return QVariant();
  }

  const SDataRow & row = mLegStatsData.at(iIndex.row());

  switch (iRole)
  {
  case Qt::DisplayRole:
    if (iIndex.column() == 0) return row.RowDescriptor;
    else
    {
      if (iIndex.row() < 3) return QString::number(row.Value.toDouble(), 'f', 2);
      else                  return QVariant::fromValue(row.Value.toInt());
    }
  default:
    return QVariant();
  }
}

void CLegStatsModel::update(CStatsWindow::SLegStatsData iLegStatsData)
{
  mLegStatsData =
  {
    {"3-Dart Leg Average", iLegStatsData.Avg3Dart},
    {"1-Dart Leg Average", iLegStatsData.Avg1Dart},
    {"Average Darts Per Leg", iLegStatsData.AvgLegDartCount},
    {"Best Won Leg", iLegStatsData.BestWonLegDartCount},
    {"Worst Won Leg", iLegStatsData.WorstWonLegDartCount},
    {"Current Leg", iLegStatsData.DartCountOfCurrentLeg}
  };
  emit dataChanged(createIndex(0,0), createIndex(rowCount() - 1, columnCount() - 1));
}

