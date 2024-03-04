#include "global_score_stats_x01_model.h"

CGlobalScoreStatsX01Model::CGlobalScoreStatsX01Model(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> iScores, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mScores(iScores)
{}

int CGlobalScoreStatsX01Model::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 10;
}

int CGlobalScoreStatsX01Model::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 4;
}

QVariant CGlobalScoreStatsX01Model::data(const QModelIndex & iIndex, int iRole) const
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
      return QString::number(mScores.at(iIndex.row()));
    }
    else if (iIndex.column() == 2)
    {
      return mCol2Labels.at(iIndex.row());
    }
    else
    {
      return QString::number(mScores.at(iIndex.row() + 10));
    }
  }
  else
  {
    return QVariant();
  }
}
