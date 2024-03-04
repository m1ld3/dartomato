#ifndef CGLOBALSCORESTATSMODEL_H
#define CGLOBALSCORESTATSMODEL_H

#include <QAbstractTableModel>
#include "stats_window_x01.h"

class CGlobalScoreStatsX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGlobalScoreStatsX01Model(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> iScores, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> mScores;
  QVector<QString> mCol0Labels
  {
      "180s:"
    , "160+:"
    , "140+:"
    , "120+:"
    , "100+:"
    , "80+:"
    , "60+:"
    , "40+:"
    , "20+:"
    , "0+:"
  };
  QVector<QString> mCol2Labels
  {
      "140s:"
    , "120s:"
    , "100s:"
    , "85s:"
    , "81s:"
    , "60s:"
    , "45s:"
    , "41s:"
    , "30s:"
    , "26s:"
  };
};

#endif  // CGLOBALSCORESTATSMODEL_H
