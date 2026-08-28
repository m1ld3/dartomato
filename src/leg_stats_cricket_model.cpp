#include "leg_stats_cricket_model.h"

CLegStatsCricketModel::CLegStatsCricketModel(const CStatsWindowCricket::SLegStatsData& iLegStatsData, QObject * iParent)
  : QAbstractTableModel(iParent)
{
  mLegStatsData =
  {
    {.RowDescriptor = "Leg Hits Per Round:", .Value = iLegStatsData.HitsPerRound},
    {.RowDescriptor = "Average Darts Per Leg:", .Value = iLegStatsData.AvgLegDartCount},
    {.RowDescriptor = "Best Won Leg:", .Value = iLegStatsData.BestWonLegDartCount},
    {.RowDescriptor = "Worst Won Leg:", .Value = iLegStatsData.WorstWonLegDartCount},
    {.RowDescriptor = "Current Leg:", .Value = iLegStatsData.DartCountOfCurrentLeg}
  };
}

int CLegStatsCricketModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return static_cast<int>(mLegStatsData.size());
}

int CLegStatsCricketModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegStatsCricketModel::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount({}) || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  const auto & [row, Value] = mLegStatsData.at(iIndex.row());

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0) return row;
    if (iIndex.row() < 3) return QString::number(Value.toDouble(), 'f', 2);
    return QVariant::fromValue(Value.toInt());
  }
  return {};
}

void CLegStatsCricketModel::update(const CStatsWindowCricket::SLegStatsData& iLegStatsData)
{
  mLegStatsData[0].Value = iLegStatsData.HitsPerRound;
  mLegStatsData[1].Value = iLegStatsData.AvgLegDartCount;
  mLegStatsData[2].Value = iLegStatsData.BestWonLegDartCount;
  mLegStatsData[3].Value = iLegStatsData.WorstWonLegDartCount;
  mLegStatsData[4].Value = iLegStatsData.DartCountOfCurrentLeg;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount({}) - 1, columnCount({}) - 1));
}

