#include "alltime_player_stats_model.h"

CAlltimePlayerStatsModel::CAlltimePlayerStatsModel(CGameDataHandler & iGameDataHandler, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mSelectedPlayers(iSelectedPlayers)
  , mIsCricket(iIsCricket)
  , mGameDataHandler(iGameDataHandler)
  , mGameStats(mGameDataHandler.get_stats_data())
{}

int CAlltimePlayerStatsModel::rowCount(const QModelIndex & iParent) const
{
  return 0;
}

int CAlltimePlayerStatsModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mSelectedPlayers.size();
}

QVariant CAlltimePlayerStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  return QVariant();
}

void CAlltimePlayerStatsModel::prepare_data()
{

}
