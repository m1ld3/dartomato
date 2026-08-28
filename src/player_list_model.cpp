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
  return static_cast<int>(mPlayerNames.count());
}

QVariant CPlayerListModel::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() > mPlayerNames.count()) return {};
  if (iRole == Qt::DisplayRole) return mPlayerNames.at(iIndex.row());
  return {};
}

bool CPlayerListModel::add_player(const QString & iPlayerName)
{
  if (!mPlayerNames.contains(iPlayerName))
  {
    const int row = rowCount({});
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
  for (const auto & player : iPlayerNames)
  {
    if (const auto index = mPlayerNames.indexOf(player); index != -1)
    {
      beginRemoveRows(QModelIndex(), static_cast<int>(index), static_cast<int>(index));
      mPlayerNames.removeAt(index);
      endRemoveRows();
    }
  }
}
