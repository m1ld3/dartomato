#include "leg_stats_cricket_model.h"

CLegStatsCricketModel::CLegStatsCricketModel(CStatsWindowCricket::SLegStatsData iLegStatsData, QObject * iParent)
  : QAbstractTableModel(iParent)
{
  mLegStatsData =
  {
    {"Leg Hits Per Round:", iLegStatsData.HitsPerRound},
    {"Average Darts Per Leg:", iLegStatsData.AvgLegDartCount},
    {"Best Won Leg:", iLegStatsData.BestWonLegDartCount},
    {"Worst Won Leg:", iLegStatsData.WorstWonLegDartCount},
    {"Current Leg:", iLegStatsData.DartCountOfCurrentLeg}
  };
}

int CLegStatsCricketModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mLegStatsData.size();
}

int CLegStatsCricketModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegStatsCricketModel::data(const QModelIndex & iIndex, int iRole) const
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

void CLegStatsCricketModel::update(CStatsWindowCricket::SLegStatsData iLegStatsData)
{
  mLegStatsData[0].Value = iLegStatsData.HitsPerRound;
  mLegStatsData[1].Value = iLegStatsData.AvgLegDartCount;
  mLegStatsData[2].Value = iLegStatsData.BestWonLegDartCount;
  mLegStatsData[3].Value = iLegStatsData.WorstWonLegDartCount;
  mLegStatsData[4].Value = iLegStatsData.DartCountOfCurrentLeg;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
}

