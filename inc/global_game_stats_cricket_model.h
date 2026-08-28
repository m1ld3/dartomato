#ifndef CGLOBALGAMESTATSCRICKETMODEL_H
#define CGLOBALGAMESTATSCRICKETMODEL_H

#include "stats_window_cricket.h"

class CGlobalGameStatsCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalGameStatsCricketModel(const CStatsWindowCricket::SGlobalGameStatsData & iData, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;

private:

  CStatsWindowCricket::SGlobalGameStatsData mData;
};

#endif  // CGLOBALGAMESTATSCRICKETMODEL_H
