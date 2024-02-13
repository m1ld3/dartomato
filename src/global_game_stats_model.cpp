#include "global_game_stats_model.h"

CGlobalGameStatsModel::CGlobalGameStatsModel(CStatsWindow::SGlobalGameStatsData &iData, QObject *iParent)
    : QAbstractTableModel(iParent)
    , mData(iData)
{}

int CGlobalGameStatsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

int CGlobalGameStatsModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 4;
}
