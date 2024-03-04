#ifndef CLEGSTATSX01MODEL_H
#define CLEGSTATSX01MODEL_H

#include <QAbstractTableModel>
#include "stats_window_x01.h"

class CLegStatsX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegStatsX01Model(CStatsWindowX01::SLegStatsData iLegStatsData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(CStatsWindowX01::SLegStatsData iLegStatsData);

private:

  struct SDataRow
  {
    QString RowDescriptor;
    QVariant Value;
  };
  QVector<SDataRow> mLegStatsData;
};

#endif  // CLEGSTATSX01MODEL_H
