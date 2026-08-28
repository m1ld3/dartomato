#include "leg_stats_x01_model.h"

CLegStatsX01Model::CLegStatsX01Model(const CStatsWindowX01::SLegStatsData& iLegStatsData, QObject * iParent)
  : QAbstractTableModel(iParent)
{
  mLegStatsData =
  {
    {.RowDescriptor = "3-Dart Leg Average:", .Value = iLegStatsData.Avg3Dart},
    {.RowDescriptor = "1-Dart Leg Average:", .Value = iLegStatsData.Avg1Dart},
    {.RowDescriptor = "First-9-Average:", .Value = iLegStatsData.First9Avg},
    {.RowDescriptor = "Average Darts Per Leg:", .Value = iLegStatsData.AvgLegDartCount},
    {.RowDescriptor = "Best Won Leg:", .Value = iLegStatsData.BestWonLegDartCount},
    {.RowDescriptor = "Worst Won Leg:", .Value = iLegStatsData.WorstWonLegDartCount},
    {.RowDescriptor = "Current Leg:", .Value = iLegStatsData.DartCountOfCurrentLeg}
  };
}

int CLegStatsX01Model::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return static_cast<int>(mLegStatsData.size());
}

int CLegStatsX01Model::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegStatsX01Model::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount({}) || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  const auto & [row, Value] = mLegStatsData.at(iIndex.row());

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0) return row;
    if (iIndex.row() < 4) return QString::number(Value.toDouble(), 'f', 2);
    return QVariant::fromValue(Value.toInt());
  }
  return {};
}

void CLegStatsX01Model::update(const CStatsWindowX01::SLegStatsData& iLegStatsData)
{
  mLegStatsData[0].Value = iLegStatsData.Avg3Dart;
  mLegStatsData[1].Value = iLegStatsData.Avg1Dart;
  mLegStatsData[2].Value = iLegStatsData.First9Avg;
  mLegStatsData[3].Value = iLegStatsData.AvgLegDartCount;
  mLegStatsData[4].Value = iLegStatsData.BestWonLegDartCount;
  mLegStatsData[5].Value = iLegStatsData.WorstWonLegDartCount;
  mLegStatsData[6].Value = iLegStatsData.DartCountOfCurrentLeg;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount({}) - 1, columnCount({}) - 1));
}

