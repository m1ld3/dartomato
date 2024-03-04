#ifndef CGLOBALGAMESTATSCRICKETMODEL_H
#define CGLOBALGAMESTATSCRICKETMODEL_H

#include <QAbstractTableModel>
#include "stats_window_cricket.h"

class CGlobalGameStatsCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalGameStatsCricketModel(const CStatsWindowCricket::SGlobalGameStatsData & iData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  CStatsWindowCricket::SGlobalGameStatsData mData;
};

#endif  // CGLOBALGAMESTATSCRICKETMODEL_H
