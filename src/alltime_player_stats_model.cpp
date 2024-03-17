#include "alltime_player_stats_model.h"

CAlltimePlayerStatsModel::CAlltimePlayerStatsModel(const QVector<CGameDataHandler::SGameData> & iGameData, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameData(iGameData)
  , mSelectedPlayers(iSelectedPlayers)
  , mIsCricket(iIsCricket)
{

}

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

}

void CAlltimePlayerStatsModel::prepare_data()
{

}
