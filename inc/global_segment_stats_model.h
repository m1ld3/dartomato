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
  QVector<QString> mCol0Labels
  {
      "0:"
    , "1:"
    , "2:"
    , "3:"
    , "4:"
    , "5:"
    , "6:"
    , "7:"
    , "8:"
    , "9:"
  };
  QVector<QString> mCol2Labels
  {
      "10:"
    , "11:"
    , "12:"
    , "13:"
    , "14:"
    , "15:"
    , "16:"
    , "17:"
    , "18:"
    , "19:"
  };
  QVector<QString> mCol4Labels
  {
      "20:"
    , "25:"
    , "T15-20:"
  };
};

#endif  // CGLOBALSEGMENTSTATSMODEL_H
