#ifndef CGLOBALSEGMENTSTATSCRICKETMODEL_H
#define CGLOBALSEGMENTSTATSCRICKETMODEL_H

#include <QAbstractTableModel>
#include "stats_window_cricket.h"

class CGlobalSegmentStatsCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalSegmentStatsCricketModel(std::array<uint32_t, static_cast<int>(CStatsWindowCricket::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  std::array<uint32_t, static_cast<int>(CStatsWindowCricket::EDartCountsIdx::SEG_MAX)> mSegments;
  QVector<QString> mCol0Labels
  {
      "S15:"
    , "S16:"
    , "S17:"
    , "S18:"
    , "S19:"
    , "S20:"
    , "S25:"
  };
  QVector<QString> mCol2Labels
  {
      "D15:"
    , "D16:"
    , "D17:"
    , "D18:"
    , "D19:"
    , "D20:"
    , "D25:"
  };
  QVector<QString> mCol4Labels
  {
      "T15:"
    , "T16:"
    , "T17:"
    , "T18:"
    , "T19:"
    , "T20:"
    , "0:"
  };
};

#endif  // CGLOBALSEGMENTSTATSCRICKETMODEL_H
