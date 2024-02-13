#include "global_score_stats_model.h"

CGlobalScoreStatsModel::CGlobalScoreStatsModel(static_cast<int>(CStatsWindow::EScoreCountsIdx::SCORE_COUNT_MAX)> iScores, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mScores(iScores)
{}

int CGlobalScoreStatsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

int CGlobalScoreStatsModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 4;
}

QVariant CGlobalScoreStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mScores.size() / 2 || iIndex.column() >= 4)
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
