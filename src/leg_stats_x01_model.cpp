#include "leg_stats_x01_model.h"

CLegStatsX01Model::CLegStatsX01Model(CStatsWindowX01::SLegStatsData iLegStatsData, QObject * iParent)
  : QAbstractTableModel(iParent)
{
  mLegStatsData =
  {
    {"3-Dart Leg Average:", iLegStatsData.Avg3Dart},
    {"1-Dart Leg Average:", iLegStatsData.Avg1Dart},
    {"Average Darts Per Leg:", iLegStatsData.AvgLegDartCount},
    {"Best Won Leg:", iLegStatsData.BestWonLegDartCount},
    {"Worst Won Leg:", iLegStatsData.WorstWonLegDartCount},
    {"Current Leg:", iLegStatsData.DartCountOfCurrentLeg}
  };
}

int CLegStatsX01Model::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mLegStatsData.size();
}

int CLegStatsX01Model::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegStatsX01Model::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
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

void CLegStatsX01Model::update(CStatsWindowX01::SLegStatsData iLegStatsData)
{
  mLegStatsData[0].Value = iLegStatsData.Avg3Dart;
  mLegStatsData[1].Value = iLegStatsData.Avg1Dart;
  mLegStatsData[2].Value = iLegStatsData.AvgLegDartCount;
  mLegStatsData[3].Value = iLegStatsData.BestWonLegDartCount;
  mLegStatsData[4].Value = iLegStatsData.WorstWonLegDartCount;
  mLegStatsData[5].Value = iLegStatsData.DartCountOfCurrentLeg;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

