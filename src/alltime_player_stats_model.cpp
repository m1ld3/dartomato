#include "alltime_player_stats_model.h"

CAlltimePlayerStatsModel::CAlltimePlayerStatsModel(CGameDataHandler & iGameDataHandler, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mIsCricket(iIsCricket)
  , mGameDataHandler(iGameDataHandler)
  , mGameStats(mGameDataHandler.get_stats_data())
{
  if (mIsCricket) update_selected_players_cricket(iSelectedPlayers);
  else update_selected_players_x01(iSelectedPlayers);
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
      return QVariant();
    }
  }
  else
  {
    return QVariant();
  }
}

void CAlltimePlayerStatsModel::update_selected_players_x01(const QVector<QString> iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
  QVector<SPlayerStats> selectedStats = {};

  for (const auto & data : mGameStats)
  {
    if (!mSelectedPlayers.contains(data.PlayerName)) continue;
    SPlayerStats stats;

    double avg3 = 0.0;
    double avg1 = 0.0;
    double first9Avg = 0.0;
    double bestLegAvg = 0.0;
    uint32_t bestWonLegDartCount = UINT32_MAX;
    uint32_t worstWonLegDartCount = 0;
    uint32_t checkoutHits = 0;
    uint32_t checkoutAttempts = 0;
    uint32_t highestCheckout = 0;
    uint32_t totalDarts = 0;

    for (const auto & gameData : data.GameDataX01)
    {
      if (gameData.Finished)
      {
        stats.GamesPlayed++;
        if (gameData.RemainingPointsOfAllLegs.back().back() == 0) stats.GamesWon++;
      }
      stats.LegsWon += gameData.TotalLegsWon;
      stats.LegsPlayed += gameData.AllScoresOfAllLegs.size();
      stats.ThrownDarts += gameData.TotalDarts;
      avg3 += gameData.Avg3Dart;
      avg1 += gameData.Avg1Dart;
      first9Avg += gameData.First9Avg;
      QVector<QVector<uint32_t>> totalScores = gameData.AllScoresOfAllLegs;
      QVector<QVector<QVector<QString>>> totalDarts = gameData.ThrownDartsOfAllLegs;
      QVector<QVector<uint32_t>> remainingPointsOfAllLegs = gameData.RemainingPointsOfAllLegs;
      if (gameData.ScoresOfCurrentLeg.size()) totalScores.append(gameData.ScoresOfCurrentLeg);
      if (gameData.ThrownDartsOfCurrentLeg.size()) totalDarts.append(gameData.ThrownDartsOfCurrentLeg);

      for (auto idx = 0; idx < totalScores.size(); idx++)
      {
        uint32_t numberOfDarts = (totalDarts.at(idx).size() - 1) * 3 + totalDarts.at(idx).back().size();
        double avg = std::accumulate(totalScores.at(idx).begin(), totalScores.at(idx).end(), 0.0) / numberOfDarts;
        if (avg > bestLegAvg) bestLegAvg = avg;
        if (remainingPointsOfAllLegs.at(idx).back() == 0)
        {
          if (numberOfDarts < bestWonLegDartCount) bestWonLegDartCount = numberOfDarts;
          if (numberOfDarts > worstWonLegDartCount) worstWonLegDartCount = numberOfDarts;
          if (totalScores.at(idx).back() > highestCheckout) highestCheckout = totalScores.at(idx).back();
        }
      }
      checkoutAttempts += gameData.CheckoutAttempts;
      checkoutHits += gameData.CheckoutHits;
    }

    stats.PlayerName = data.PlayerName;
    stats.WinRate = stats.GamesPlayed > 0 ? static_cast<double>(stats.GamesWon) / stats.GamesPlayed * 100 : 0.0;
    stats.LegWinRate = stats.LegsPlayed > 0 ? static_cast<double>(stats.LegsWon) / stats.LegsPlayed * 100 : 0.0;
    stats.Avg3Dart = avg3 / data.GameDataX01.size();
    stats.Avg1Dart = avg1 / data.GameDataX01.size();
    stats.First9Avg = first9Avg / data.GameDataX01.size();
    stats.BestLegAvg = 3 * bestLegAvg;
    stats.BestWonLegDartCount = bestWonLegDartCount;
    stats.WorstWonLegDartCount = worstWonLegDartCount;
    stats.CheckoutAttempts = checkoutAttempts;
    stats.CheckoutHits = checkoutHits;
    stats.CheckoutRate = checkoutAttempts > 0 ? static_cast<double>(checkoutHits) / checkoutAttempts * 100 : 0.0;
    stats.HighestCheckout = highestCheckout;
    stats.DartsPerLegAvg = stats.LegsPlayed > 0 ? static_cast<double>(stats.ThrownDarts) / stats.LegsPlayed : 0.0;
    selectedStats.append(stats);
  }
}

void CAlltimePlayerStatsModel::update_selected_players_cricket(const QVector<QString> iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
  QVector<SPlayerStats> selectedStats = {};

  for (const auto & data : mGameStats)
  {
    if (!mSelectedPlayers.contains(data.PlayerName)) continue;
    SPlayerStats stats;

    double bestLegAvg = 0.0;
    uint32_t bestWonLegDartCount = UINT32_MAX;
    uint32_t worstWonLegDartCount = 0;
    uint32_t totalDarts = 0;

    for (const auto & gameData : data.GameDataCricket)
    {
      if (gameData.Finished)
      {
        stats.GamesPlayed++;
        if (gameData.GameWon) stats.GamesWon++;
      }
      stats.LegsWon += gameData.TotalLegsWon;
      stats.LegsPlayed += gameData.ScoringHistory.size();
      stats.ThrownDarts += gameData.TotalDarts;
      QVector<QVector<QVector<QString>>> totalDarts = gameData.ScoringHistory;
      QVector<QVector<bool>> wonLegs = gameData.LegWonHistory;
      if (gameData.LegWonVec.size()) wonLegs.append(gameData.LegWonVec);
      if (gameData.ScoresOfCurrentLeg.size()) totalDarts.append(gameData.ScoresOfCurrentLeg);

      for (auto idx = 0; idx < totalDarts.size(); idx++)
      {
        uint32_t numberOfDarts = (totalDarts.at(idx).size() - 1) * 3 + totalDarts.at(idx).back().size();
        if (wonLegs.at(idx).back())
        {
          if (numberOfDarts < bestWonLegDartCount) bestWonLegDartCount = numberOfDarts;
          if (numberOfDarts > worstWonLegDartCount) worstWonLegDartCount = numberOfDarts;
        }
      }
    }

    stats.PlayerName = data.PlayerName;
    stats.WinRate = stats.GamesPlayed > 0 ? static_cast<double>(stats.GamesWon) / stats.GamesPlayed * 100 : 0.0;
    stats.LegWinRate = stats.LegsPlayed > 0 ? static_cast<double>(stats.LegsWon) / stats.LegsPlayed * 100 : 0.0;
    stats.BestLegAvg = 3 * bestLegAvg;
    stats.BestWonLegDartCount = bestWonLegDartCount;
    stats.WorstWonLegDartCount = worstWonLegDartCount;
    stats.DartsPerLegAvg = stats.LegsPlayed > 0 ? static_cast<double>(stats.ThrownDarts) / stats.LegsPlayed : 0.0;
    selectedStats.append(stats);
  }
}
