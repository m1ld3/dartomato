#ifndef CLEGSTATSCRICKETMODEL_H
#define CLEGSTATSCRICKETMODEL_H

#include <QAbstractTableModel>
#include "stats_window_cricket.h"

class CLegStatsCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegStatsCricketModel(CStatsWindowCricket::SLegStatsData iLegStatsData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(CStatsWindowCricket::SLegStatsData iLegStatsData);

private:

  struct SDataRow
  {
    QString RowDescriptor;
    QVariant Value;
  };
  QVector<SDataRow> mLegStatsData;
};

#endif  // CLEGSTATSCRICKETMODEL_H
