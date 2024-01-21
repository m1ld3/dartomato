#ifndef CGAMEDATAMODEL_H
#define CGAMEDATAMODEL_H

#include <QAbstractListModel>
#include "game_data_handler.h"

class CGameDataModel : public QAbstractListModel
{
  Q_OBJECT

public:

  explicit CGameDataModel(const CGameDataHandler & iGameDataHandler, QObject * iParent = nullptr);

  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  bool add_player(const QString & iPlayerName);

private:

  CGameDataHandler mGameDataHandler;
  QStringList mPlayerNames;

};

#endif  // CGAMEDATAMODEL_H
