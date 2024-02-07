#ifndef STATS_WINDOW_H
#define STATS_WINDOW_H

#include <QDialog>
#include <QString>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QProxyStyle>
#include "x01_class.h"

class CLegStatsModel;

namespace Ui
{
  class CStatsWindow;
}

class CStatsWindow : public QDialog
{
  Q_OBJECT

public:

  enum class EScoreCountsIdx
  {
      PLUS_0   = 0
    , PLUS_20  = 1
    , PLUS_40  = 2
    , PLUS_60  = 3
    , PLUS_80  = 4
    , PLUS_100 = 5
    , PLUS_120 = 6
    , PLUS_140 = 7
    , PLUS_160 = 8
    , THE_180  = 9
    , THE_140  = 10
    , THE_120  = 11
    , THE_100  = 12
    , THE_85   = 13
    , THE_81   = 14
    , THE_60   = 15
    , THE_45   = 16
    , THE_41   = 17
    , THE_30   = 18
    , THE_26   = 19
  };

  enum class EDartCountsIdx
  {
      SEG_0       = 0
    , SEG_1       = 1
    , SEG_2       = 2
    , SEG_3       = 3
    , SEG_4       = 4
    , SEG_5       = 5
    , SEG_6       = 6
    , SEG_7       = 7
    , SEG_8       = 8
    , SEG_9       = 9
    , SEG_10      = 10
    , SEG_11      = 11
    , SEG_12      = 12
    , SEG_13      = 13
    , SEG_14      = 14
    , SEG_15      = 15
    , SEG_16      = 16
    , SEG_17      = 17
    , SEG_18      = 18
    , SEG_19      = 19
    , SEG_20      = 20
    , SEG_25      = 21
    , SEG_TRIPLES = 22
  };

  struct SLegStatsData
  {
    double Avg3Dart = 0;
    double Avg1Dart = 0;
    double AvgLegDartCount = 0.0;
    uint32_t BestWonLegDartCount = 0;
    uint32_t WorstWonLegDartCount = 0;
    uint32_t DartCountOfCurrentLeg = 0;
  };

  explicit CStatsWindow(const CX01Class::CPlayerData & iPlayerData, QWidget * iParent = nullptr);
  ~CStatsWindow() override;

private slots:

  void update_leg_history(int iIndex);

private:

  void set_label_checkout();
  void set_text(QString iText);
  void clear_text();
  void init_leg_selector(uint32_t iNumberOfLegs);
  void count_scores();
  std::map<uint32_t, uint32_t> calculate_score_counts();
  void calculate_segment_counts();
  void set_stats_labels();
  void display_current_leg_scores();
  void compute_dart_count_and_checkouts();
  double compute_average(QVector<uint32_t> iScoresOfLeg);
  void display_highest_checkout();
  uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex);

private:

  Ui::CStatsWindow * mUi;
  const CX01Class::CPlayerData & mPlayerData;
  std::array<uint32_t, 20> mScoreCounts = {};
  std::array<uint32_t, 23> mSegmentCounts = {};
  QVector<uint32_t> mDartCountOfWonLegs = {};
  QVector<uint32_t> mAllCheckouts = {};
  uint32_t mHighestCheckout = 0;
  QPixmap mPixMap = QPixmap(":/resources/img/dart.svg");
  SLegStatsData mLegStatsData;
  CLegStatsModel * mLegStatsModel = nullptr;
};

#endif  // STATS_WINDOW_H
