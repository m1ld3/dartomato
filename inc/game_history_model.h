#ifndef CGAMEHISTORYMODEL_H
#define CGAMEHISTORYMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CGameHistoryModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGameHistoryModel(CGameDataHandler & iGameDataHandler, QObject * iParent = nullptr);
//  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
//  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
//  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
//  QVariant headerData(int iSection, Qt::Orientation iOrientation, int iRole) const override;

private:

  CGameDataHandler & mGameDataHandler;

};

#endif  // CGAMEHISTORYMODEL_H
