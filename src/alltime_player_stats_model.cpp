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
  return mSelectedPlayers.size();
}

QVariant CAlltimePlayerStatsModel::headerData(int iSection, Qt::Orientation iOrientation, int iRole) const
{
  if (iRole == Qt::DisplayRole)
  {
    if (iOrientation == Qt::Horizontal)
    {
      if (iSection >= 0 && iSection < mSelectedPlayers.size())
      {
        return mSelectedPlayers.at(iSection);
      }
    }
    else if (iOrientation == Qt::Vertical)
    {
      return mIsCricket? mStatLabelsCricket.at(iSection) : mStatLabelsX01.at(iSection);
    }
  }
  return QVariant();
}

QVariant CAlltimePlayerStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount() || iIndex.column() >= mSelectedStats.size())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    size_t playerIdx = iIndex.column();
    switch (iIndex.row())
    {
    case  0: return QString::number(mSelectedStats.at(playerIdx).GamesPlayed);
    case  1: return QString::number(mSelectedStats.at(playerIdx).GamesWon);
    case  2: return QString::number(mSelectedStats.at(playerIdx).WinRate, 'f', 2);
    case  3: return QString::number(mSelectedStats.at(playerIdx).LegsPlayed);
    case  4: return QString::number(mSelectedStats.at(playerIdx).LegsWon);
    case  5: return QString::number(mSelectedStats.at(playerIdx).LegWinRate, 'f', 2);
    case  6: return QString::number(mSelectedStats.at(playerIdx).ThrownDarts);
    case  7: return QString::number(mSelectedStats.at(playerIdx).BestWonLegDartCount);
    case  8: return QString::number(mSelectedStats.at(playerIdx).WorstWonLegDartCount);
    case  9: return QString::number(mSelectedStats.at(playerIdx).DartsPerLegAvg, 'f', 2);
    case 10:
      if (mIsCricket) return QString::number(mSelectedStats.at(playerIdx).HitsPerRound, 'f', 2);
      else            return QString::number(mSelectedStats.at(playerIdx).Avg3Dart, 'f', 2);
    case 11:
      if (mIsCricket) return QString::number(mSelectedStats.at(playerIdx).TotalHits);
      else            return QString::number(mSelectedStats.at(playerIdx).Avg1Dart, 'f', 2);
    case 12: return QString::number(mSelectedStats.at(playerIdx).First9Avg, 'f', 2);
    case 13: return QString::number(mSelectedStats.at(playerIdx).BestLegAvg, 'f', 2);
    case 14: return QString::number(mSelectedStats.at(playerIdx).CheckoutRate, 'f', 2) + "  (" + QString::number(mSelectedStats.at(playerIdx).CheckoutHits) + " / " + QString::number(mSelectedStats.at(playerIdx).CheckoutAttempts) + ")";
    case 15: return QString::number(mSelectedStats.at(playerIdx).HighestCheckout);
    default: return QVariant();
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
  mSelectedStats = {};

  for (const auto & data : mGameStats)
  {
    if (!mSelectedPlayers.contains(data.PlayerName)) continue;
    if (data.GameDataX01.size() == 0) continue;
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

    if (bestWonLegDartCount == UINT32_MAX) bestWonLegDartCount = 0;

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
    mSelectedStats.append(stats);
  }
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
  emit layoutChanged();
}

void CAlltimePlayerStatsModel::update_selected_players_cricket(const QVector<QString> iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
  mSelectedStats = {};

  for (const auto & data : mGameStats)
  {
    if (!mSelectedPlayers.contains(data.PlayerName)) continue;
    if (data.GameDataCricket.size() == 0) continue;

    SPlayerStats stats;

    uint32_t bestWonLegDartCount = UINT32_MAX;
    uint32_t worstWonLegDartCount = 0;
    uint32_t totalDarts = 0;
    uint32_t totalHits = 0;

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
      stats.TotalHits += gameData.TotalHits;
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

    if (bestWonLegDartCount == UINT32_MAX) bestWonLegDartCount = 0;

    stats.PlayerName = data.PlayerName;
    stats.WinRate = stats.GamesPlayed > 0 ? static_cast<double>(stats.GamesWon) / stats.GamesPlayed * 100 : 0.0;
    stats.LegWinRate = stats.LegsPlayed > 0 ? static_cast<double>(stats.LegsWon) / stats.LegsPlayed * 100 : 0.0;
    stats.BestWonLegDartCount = bestWonLegDartCount;
    stats.WorstWonLegDartCount = worstWonLegDartCount;
    stats.DartsPerLegAvg = stats.LegsPlayed > 0 ? static_cast<double>(stats.ThrownDarts) / stats.LegsPlayed : 0.0;
    stats.HitsPerRound = stats.ThrownDarts > 0 ? 3 * static_cast<double>(stats.TotalHits) / stats.ThrownDarts : 0.0;
    mSelectedStats.append(stats);
  }
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
  emit layoutChanged();
}

void CAlltimePlayerStatsModel::update_selected_game(bool iIsCricket)
{
  mIsCricket = iIsCricket;
  if (mIsCricket) update_selected_players_cricket(mSelectedPlayers);
  else update_selected_players_x01(mSelectedPlayers);
}
