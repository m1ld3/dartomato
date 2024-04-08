#include "player_list_model.h"

CPlayerListModel::CPlayerListModel(const CGameDataHandler & iGameDataHandler, QObject * iParent)
  : QAbstractListModel(iParent)
  , mGameDataHandler(iGameDataHandler)
{
  mPlayerNames = mGameDataHandler.get_player_names();
}

int CPlayerListModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return mPlayerNames.count();
}

QVariant CPlayerListModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() > mPlayerNames.count()) return QVariant();
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

void CPlayerListModel::remove_players(const QStringList & iPlayerNames)
{
  bool changed = false;

  for (const auto & player : iPlayerNames)
  {
    int index = mPlayerNames.indexOf(player);
    if (index != -1)
    {
      beginRemoveRows(QModelIndex(), index, index);
      mPlayerNames.removeAt(index);
      endRemoveRows();
    }
  }
}
