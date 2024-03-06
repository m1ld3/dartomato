#include "game_results_model.h"

CGameResultsModel::CGameResultsModel(const CGameDataHandler::SGameData & iGameData, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameData(iGameData)
  , mIsX01(mGameData.GameDataX01.at(0).size() > 0)
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
    mScores.append({playerData.SetsWon, playerData.LegsWonPerSet, idx});
  }

  std::sort(mScores.begin(), mScores.end(), SPlayerScore::compare);
}

int CGameResultsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mGameData.Settings.PlayersList.size();
}

int CGameResultsModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return mColumnHeaders.size();
}

QVariant CGameResultsModel::headerData(int iSection, Qt::Orientation iOrientation, int iRole) const
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
  return QVariant();
}

QVariant CGameResultsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)      return mRankLabels.at(iIndex.row());
    else if (iIndex.column() == 1) return mGameData.Settings.PlayersList.at(mScores.at(iIndex.row()).PlayerId);
    else if (iIndex.column() == 2) return "Sets: " + QString::number(mScores.at(iIndex.row()).Sets) + " / Legs: " + QString::number(mScores.at(iIndex.row()).Legs);
    else if (iIndex.column() == 3)
    {
      if (mIsX01)
      {
        return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().TotalLegsWon);
      }
      else
      {
        return QString::number(mGameData.GameDataCricket.at(mScores.at(iIndex.row()).PlayerId).back().TotalLegsWon);
      }
    }
    else if (iIndex.column() == 4)
    {
      if (mIsX01)
      {
        return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().Avg3Dart, 'f', 2);
      }
      else
      {
        return QString::number(mGameData.GameDataCricket.at(mScores.at(iIndex.row()).PlayerId).back().HitsPerRound, 'f', 2);
      }
    }
    else if (iIndex.column() == 5 && mIsX01)
    {
      return QString::number(mGameData.GameDataX01.at(mScores.at(iIndex.row()).PlayerId).back().CheckoutRate, 'f', 2);
    }

    return QVariant();
  }
  else
  {
    return QVariant();
  }
}



