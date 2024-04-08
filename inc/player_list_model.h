#ifndef CGAMEDATAMODEL_H
#define CGAMEDATAMODEL_H

#include <QAbstractListModel>
#include "game_data_handler.h"

class CPlayerListModel : public QAbstractListModel
{
  Q_OBJECT

public:

  explicit CPlayerListModel(const CGameDataHandler & iGameDataHandler, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  bool add_player(const QString & iPlayerName);
  void remove_players(const QStringList & iPlayerNames);

private:

  CGameDataHandler mGameDataHandler;
  QStringList mPlayerNames;

};

#endif  // CGAMEDATAMODEL_H
