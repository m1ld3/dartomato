#ifndef CGLOBALSEGMENTSTATSMODEL_H
#define CGLOBALSEGMENTSTATSMODEL_H

#include <QAbstractTableModel>
#include "stats_window.h"

class CGlobalSegmentStatsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalSegmentStatsModel(std::array<uint32_t, static_cast<int>(CStatsWindow::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  std::array<uint32_t, static_cast<int>(CStatsWindow::EDartCountsIdx::SEG_MAX)> mSegments;
};

#endif  // CGLOBALSEGMENTSTATSMODEL_H
