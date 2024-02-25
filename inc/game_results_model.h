#ifndef CGAMERESULTMODEL_H
#define CGAMERESULTMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CGameResultsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGameResultsModel(const CGameDataHandler::SGameData & iGameData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant headerData(int iSection, Qt::Orientation iOrientation, int iRole) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  const CGameDataHandler::SGameData mGameData;
  const QVector<QString> mColumnHeaders {"Rank",
                                         "Player",
                                         "Score",
                                         "3-Dart-Average",
                                         "Checkout-Rate",
                                        };
};

#endif  // CGAMERESULTMODEL_H
