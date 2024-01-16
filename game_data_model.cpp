#include "game_data_model.h"

CGameDataModel::CGameDataModel(const CGameDataHandler & iGameDataHandler, QObject * iParent)
  : QAbstractListModel(iParent)
  , mGameDataHandler(iGameDataHandler)
{
  mPlayerNames = mGameDataHandler.get_player_names();
}

int CGameDataModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) return 0;
  return mPlayerNames.count();
}

QVariant CGameDataModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid())        return QVariant();
  if (iRole == Qt::DisplayRole) return mPlayerNames.at(iIndex.row());
  return QVariant();
}

bool CGameDataModel::add_player(const QString & iPlayerName)
{
  if (!mPlayerNames.contains(iPlayerName))
  {
    int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    mPlayerNames.append(iPlayerName);
    endInsertRows();
    mGameDataHandler.add_new_player(iPlayerName);
    return true;
  }
  return false;
}
