#include "game_results_model.h"

#include <utility>

CGameResultsModel::CGameResultsModel(CGameDataHandler::SGameData  iGameData, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameData(std::move(iGameData))
  , mIsX01(!mGameData.GameDataX01.at(0).empty())
{
  if (mIsX01)
  {
    find_player_ranking_x01();
    mColumnHeaders = {"Rank", "Player", "Final Score", "Legs won", "3-Dart-Average", "Checkout-Rate"};
  }
  else
  {
    find_player_ranking_cricket();
    mColumnHeaders = {"Rank", "Player", "Final Score", "Legs won", "Hits per Round"};
  }
  if (!mGameData.Finished) mColumnHeaders[2] = "Current Score";
}

void CGameResultsModel::find_player_ranking_x01()
{
  for (uint32_t idx = 0; idx < mGameData.Settings.PlayersList.size(); idx++)
  {
    const auto & playerData = mGameData.GameDataX01.at(idx).back();
    mScores.append({playerData.SetsWon, playerData.LegsWonPerSet, idx});
  }

  std::sort(mScores.begin(), mScores.end(), SPlayerScore::compare);
}

void CGameResultsModel::find_player_ranking_cricket()
{
  for (uint32_t idx = 0; idx < mGameData.Settings.PlayersList.size(); idx++)
  {
    const auto & playerData = mGameData.GameDataCricket.at(idx).back();
    mScores.append({.Sets = playerData.SetsWon, .Legs = playerData.LegsWonPerSet, .PlayerId = idx});
  }

  std::sort(mScores.begin(), mScores.end(), SPlayerScore::compare);
}

int CGameResultsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return static_cast<int>(mGameData.Settings.PlayersList.size());
}

int CGameResultsModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return static_cast<int>(mColumnHeaders.size());
}

QVariant CGameResultsModel::headerData(const int iSection, const Qt::Orientation iOrientation, const int iRole) const
{
  if (iRole == Qt::DisplayRole)
  {
    if (iOrientation == Qt::Horizontal)
    {
      if (iSection >= 0 && iSection < mColumnHeaders.size())
      {
        return mColumnHeaders.at(iSection);
      }
    }
  }
  return {};
}

QVariant CGameResultsModel::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount({}) || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)      return mRankLabels.at(iIndex.row());
    if (iIndex.column() == 1) return mGameData.Settings.PlayersList.at(mScores.at(iIndex.row()).PlayerId);
    if (iIndex.column() == 2) return "Sets: " + QString::number(mScores.at(iIndex.row()).Sets) + " / Legs: " + QString::number(mScores.at(iIndex.row()).Legs);
    if (iIndex.column() == 3)
    {
      if (mIsX01)
      {
        return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().TotalLegsWon);
      }
      return QString::number(mGameData.GameDataCricket.at(mScores.at(iIndex.row()).PlayerId).back().TotalLegsWon);
    }
    if (iIndex.column() == 4)
    {
      if (mIsX01)
      {
        return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().Avg3Dart, 'f', 2);
      }
      return QString::number(mGameData.GameDataCricket.at(mScores.at(iIndex.row()).PlayerId).back().HitsPerRound, 'f', 2);
    }
    if (iIndex.column() == 5 && mIsX01)
    {
      return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().CheckoutRate, 'f', 2);
    }
    return {};
  }
  return {};
}



