#include "game_history_model.h"

CGameHistoryModel::CGameHistoryModel(QVector<CGameDataHandler::SGameData> & iGameData, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameData(iGameData)
{}

int CGameHistoryModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return static_cast<int>(mGameData.size());
}

int CGameHistoryModel::columnCount(const QModelIndex &iParent) const
{
  Q_UNUSED(iParent);
  return 6;
}

QVariant CGameHistoryModel::headerData(const int iSection, const Qt::Orientation iOrientation, const int iRole) const
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
  return {};
}

QVariant CGameHistoryModel::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= rowCount({}) || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  if (iRole == Qt::DisplayRole)
  {
    const CGameDataHandler::SGameData & gameDataRow = mGameData.at(iIndex.row());
    if (iIndex.column() == 0) return gameDataRow.TimeStamp;
    if (iIndex.column() == 1)
    {
      int type = static_cast<int>(mGameData.at(iIndex.row()).Settings.Game);
      return type == 0 ? "Cricket" : QString::number(type);
    }
    if (iIndex.column() == 2)
    {
      if (gameDataRow.Finished)
      {
        return gameDataRow.Settings.PlayersList.at(gameDataRow.WinnerIdx);
      }
      return "--";
    }
    if (iIndex.column() == 3) return QString::number(gameDataRow.Settings.Sets);
    if (iIndex.column() == 4) return QString::number(gameDataRow.Settings.Legs);
    QString mode;
    if (gameDataRow.Settings.Game == EGame::GAME_CRICKET) mode += gameDataRow.Settings.CutThroat ? "Cut Throat" : "Default";
    else mode += MapX01InMode2Str[gameDataRow.Settings.InMode] + " / " + MapX01OutMode2Str[gameDataRow.Settings.OutMode];
    return mode;
  }
  return {};
}

void CGameHistoryModel::delete_row(const int iRowIdx)
{
  beginRemoveRows(QModelIndex(), iRowIdx, iRowIdx);
  mGameData.removeAt(iRowIdx);
  endRemoveRows();
}



