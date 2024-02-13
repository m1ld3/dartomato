#ifndef CGLOBALGAMESTATSMODEL_H
#define CGLOBALGAMESTATSMODEL_H

#include <QAbstractTableModel>
#include "stats_window.h"

class CGlobalGameStatsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalGameStatsModel(const CStatsWindow::SGlobalGameStatsData & iData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  CStatsWindow::SGlobalGameStatsData mData;
  QVector<QString> mCol0Labels = {"3-Dart-Average", "1-Dart-Average"};
  QVector<double> mCol1Data;
  QVector<QString> mCol2Labels = {"Checkout-Rate", "Highest Checkout"};
};

#endif  // CGLOBALGAMESTATSMODEL_H
