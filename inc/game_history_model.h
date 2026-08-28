#ifndef CGAMEHISTORYMODEL_H
#define CGAMEHISTORYMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CGameHistoryModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGameHistoryModel(QVector<CGameDataHandler::SGameData> & iGameData, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant headerData(int iSection, Qt::Orientation iOrientation, int iRole) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;
  void delete_row(int iRowIdx);

private:

  QVector<CGameDataHandler::SGameData> & mGameData;
  const QVector<QString> mColumnHeaders {"Date",
                                         "Game Type",
                                         "Winner",
                                         "Best of Sets",
                                         "Best of Legs",
                                         "Game Mode"
                                        };
};

#endif  // CGAMEHISTORYMODEL_H
