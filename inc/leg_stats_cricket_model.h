#ifndef CLEGSTATSCRICKETMODEL_H
#define CLEGSTATSCRICKETMODEL_H

#include "stats_window_cricket.h"

class CLegStatsCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegStatsCricketModel(const CStatsWindowCricket::SLegStatsData& iLegStatsData, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;
  void update(const CStatsWindowCricket::SLegStatsData& iLegStatsData);

private:

  struct SDataRow
  {
    QString RowDescriptor;
    QVariant Value;
  };
  QVector<SDataRow> mLegStatsData;
};

#endif  // CLEGSTATSCRICKETMODEL_H
