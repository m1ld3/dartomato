#ifndef CGLOBALGAMESTATSX01MODEL_H
#define CGLOBALGAMESTATSX01MODEL_H

#include "stats_window_x01.h"

class CGlobalGameStatsX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalGameStatsX01Model(const CStatsWindowX01::SGlobalGameStatsData & iData, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;

private:

  CStatsWindowX01::SGlobalGameStatsData mData;
  QVector<QString> mCol0Labels = {"3-Dart-Average:", "1-Dart-Average:", "First-9-Average:"};
  QVector<double> mCol1Data;
  QVector<QString> mCol2Labels = {"Legs Won:", "Checkout-Rate:", "Highest Checkout:"};
};

#endif  // CGLOBALGAMESTATSX01MODEL_H
