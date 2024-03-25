#include "alltime_player_stats_model.h"

CAlltimePlayerStatsModel::CAlltimePlayerStatsModel(CGameDataHandler & iGameDataHandler, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mIsCricket(iIsCricket)
  , mGameDataHandler(iGameDataHandler)
  , mGameStats(mGameDataHandler.get_stats_data())
{
  update_selected_players(iSelectedPlayers);
}

int CAlltimePlayerStatsModel::rowCount(const QModelIndex & iParent) const
{
  return mIsCricket ? mStatLabelsCricket.size() : mStatLabelsX01.size();
}

int CAlltimePlayerStatsModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mSelectedPlayers.size() + 1;
}

QVariant CAlltimePlayerStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return mIsCricket? mStatLabelsCricket.at(iIndex.row()) : mStatLabelsX01.at(iIndex.row());
    }
    else
    {

    }
  }
  else
  {
    return QVariant();
  }
}

void CAlltimePlayerStatsModel::update_selected_players(const QVector<QString> iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
  QVector<CGameDataHandler::SStatsData> selectedStats = {};

  for (const auto & data : mGameStats)
  {
    if (!mSelectedPlayers.contains(data.PlayerName)) continue;
    for (const auto & gamedata : data.GameDataX01)
    {
      SPlayerStats stats;
      double avg3 = 0.0;
      double avg1 = 0.0;
      double first9Avg = 0.0;
      double bestLegAvg = 0.0;
      stats.PlayerName = data.PlayerName;
      for (const auto & gameStats : data.GameDataX01)
      {

        if (gameStats.Finished)
        {
          stats.GamesPlayed++;
          if (gameStats.RemainingPoints == 0) stats.GamesWon++;
        }
        stats.LegsWon += gameStats.TotalLegsWon;
        stats.LegsPlayed += gameStats.AllScoresOfAllLegs.size();
        stats.ThrownDarts += gameStats.TotalDarts;
        avg3 += gameStats.Avg3Dart;
        avg1 += gameStats.Avg1Dart;
        first9Avg += gameStats.First9Avg;
        // todo if (gameStats.bestLegAvg > bestLegAvg) bestLegAvg = gameStats.bestLegAvg;
      }
      stats.WinRate = static_cast<double>(stats.GamesWon) / stats.GamesPlayed * 100;
      stats.LegWinRate = static_cast<double>(stats.LegsWon) / stats.LegsPlayed * 100;
      stats.Avg3Dart = avg3 / data.GameDataX01.size();
      stats.Avg1Dart = avg1 / data.GameDataX01.size();
      stats.First9Avg = first9Avg / data.GameDataX01.size();
    }
  }

//  for (const auto & stats : selectedStats)
//  {
//    for (const auto & stat : stats.GameDataX01)
//    {
//      stat.
//    }
//  }
}
