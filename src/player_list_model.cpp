#include "player_list_model.h"

CPlayerListModel::CPlayerListModel(const CGameDataHandler & iGameDataHandler, QObject * iParent)
  : QAbstractListModel(iParent)
  , mGameDataHandler(iGameDataHandler)
{
  mPlayerNames = mGameDataHandler.get_player_names();
}

int CPlayerListModel::rowCount(const QModelIndex & iParent) const
{
  if (iParent.isValid()) return 0;
  return mPlayerNames.count();
}

QVariant CPlayerListModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid())        return QVariant();
  if (iRole == Qt::DisplayRole) return mPlayerNames.at(iIndex.row());
  return QVariant();
}

bool CPlayerListModel::add_player(const QString & iPlayerName)
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
