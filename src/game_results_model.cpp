#include "game_results_model.h"

CGameResultsModel::CGameResultsModel(const CGameDataHandler::SGameData & iGameData, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameData(iGameData)
{}

int CGameResultsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mGameData.Settings.PlayersList.size();
}

int CGameResultsModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 5;
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
    if (mGameData.GameDataCricket.size() > 0)
    {
        todo: find ranking of players, get all column data
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



