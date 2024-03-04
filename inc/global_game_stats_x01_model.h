#ifndef CGLOBALGAMESTATSX01MODEL_H
#define CGLOBALGAMESTATSX01MODEL_H

#include <QAbstractTableModel>
#include "stats_window_x01.h"

class CGlobalGameStatsX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalGameStatsX01Model(const CStatsWindowX01::SGlobalGameStatsData & iData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  CStatsWindowX01::SGlobalGameStatsData mData;
  QVector<QString> mCol0Labels = {"3-Dart-Average:", "1-Dart-Average:"};
  QVector<double> mCol1Data;
  QVector<QString> mCol2Labels = {"Checkout-Rate:", "Highest Checkout:"};
};

#endif  // CGLOBALGAMESTATSX01MODEL_H
