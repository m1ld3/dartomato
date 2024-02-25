#include "game_history_model.h"

CGameHistoryModel::CGameHistoryModel(CGameDataHandler & iGameDataHandler, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameDataHandler(iGameDataHandler)
  , mGameData(mGameDataHandler.get_game_data())
{}

int CGameHistoryModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mGameData.size();
}

int CGameHistoryModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGameHistoryModel::headerData(int iSection, Qt::Orientation iOrientation, int iRole) const
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
    else if (iOrientation == Qt::Vertical)
    {
      const CGameDataHandler::SGameData & gameDataRow = mGameData.at(iSection);
      return gameDataRow.Finished ? "Finished" : "Open";
    }
  }
  return QVariant();
}

QVariant CGameHistoryModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    const CGameDataHandler::SGameData & gameDataRow = mGameData.at(iIndex.row());
    if (iIndex.column() == 0) return gameDataRow.TimeStamp;
    else if (iIndex.column() == 1)
    {
      int type = static_cast<int>(mGameData.at(iIndex.row()).Settings.Game);
      return type == 0 ? "Cricket" : QString::number(type);
    }
    else if (iIndex.column() == 2)
    {
      if (gameDataRow.Finished)
      {
        return gameDataRow.Settings.PlayersList.at(gameDataRow.WinnerIdx);
      }
      else
      {
        return "--";
      }
    }
    else if (iIndex.column() == 3) return QString::number(gameDataRow.Settings.Sets);
    else if (iIndex.column() == 4) return QString::number(gameDataRow.Settings.Legs);
    else
    {
      QString mode;
      if (gameDataRow.Settings.Game == EGame::GAME_CRICKET) mode += gameDataRow.Settings.CutThroat ? "Cut Throat" : "Default";
      else mode += MapX01InMode2Str[gameDataRow.Settings.InMode] + " / " + MapX01OutMode2Str[gameDataRow.Settings.OutMode];
      return mode;
    }
  }
  else
  {
    return QVariant();
  }
}



