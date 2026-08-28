#ifndef CLEGSTATSX01MODEL_H
#define CLEGSTATSX01MODEL_H

#include "stats_window_x01.h"

class CLegStatsX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegStatsX01Model(const CStatsWindowX01::SLegStatsData& iLegStatsData, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;
  void update(const CStatsWindowX01::SLegStatsData& iLegStatsData);

private:

  struct SDataRow
  {
    QString RowDescriptor;
    QVariant Value;
  };
  QVector<SDataRow> mLegStatsData;
};

#endif  // CLEGSTATSX01MODEL_H
