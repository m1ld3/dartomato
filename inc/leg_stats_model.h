#ifndef CLEGSTATSMODEL_H
#define CLEGSTATSMODEL_H

#include <QAbstractTableModel>
#include "stats_window.h"

class CLegStatsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegStatsModel(CStatsWindow::SLegStatsData iLegStatsData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(CStatsWindow::SLegStatsData iLegStatsData);

private:

  struct SDataRow
  {
    QString RowDescriptor;
    QVariant Value;
  };
  QVector<SDataRow> mLegStatsData;
};

#endif  // CLEGSTATSMODEL_H
