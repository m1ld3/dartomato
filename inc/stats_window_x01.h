#ifndef STATS_WINDOW_X01_H
#define STATS_WINDOW_X01_H

#include <QString>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QProxyStyle>
#include "stats_window_if.h"
#include <QDialog>

namespace Ui
{
  class CStatsWindowX01;
}

class CLegStatsX01Model;
class CLegScoresX01Model;
class CGlobalGameStatsX01Model;
class CGlobalScoreStatsX01Model;
class CGlobalSegmentStatsX01Model;

class CStatsWindowX01 : public QDialog, public CStatsWindow
{
  Q_OBJECT
  Q_INTERFACES(CStatsWindow)

public:

  enum class EScoreCountsIdx
  {
      THE_180         = 0
    , PLUS_160        = 1
    , PLUS_140        = 2
    , PLUS_120        = 3
    , PLUS_100        = 4
    , PLUS_80         = 5
    , PLUS_60         = 6
    , PLUS_40         = 7
    , PLUS_20         = 8
    , PLUS_0          = 9
    , THE_140         = 10
    , THE_120         = 11
    , THE_100         = 12
    , THE_85          = 13
    , THE_81          = 14
    , THE_60          = 15
    , THE_45          = 16
    , THE_41          = 17
    , THE_30          = 18
    , THE_26          = 19
    , SCORE_COUNT_MAX = 20
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
    , SEG_MAX     = 23
  };

  struct SLegStatsData
  {
    double Avg3Dart = 0.0;
    double Avg1Dart = 0.0;
    double AvgLegDartCount = 0.0;
    double First9Avg = 0.0;
    uint32_t BestWonLegDartCount = 0;
    uint32_t WorstWonLegDartCount = 0;
    uint32_t DartCountOfCurrentLeg = 0;
  };

  struct SGlobalGameStatsData
  {
    double Avg3Dart = 0.0;
    double Avg1Dart = 0.0;
    double First9Avg = 0.0;
    uint32_t LegsWon = 0;
    uint32_t NumLegs = 0;
    uint32_t HighestCheckout = 0;
    uint32_t CheckoutAttempts = 0;
    uint32_t CheckoutHits = 0;
  };

  explicit CStatsWindowX01(const CX01Class::CPlayerData iPlayerData, QWidget * iParent = nullptr);
  ~CStatsWindowX01() override;

private slots:

  void update_leg_history_slot(int iIndex) { update_leg_history(iIndex); }

private:

  void update_leg_history(int iIndex) override;
  void init_leg_selector() override;
  void calculate_segment_counts() override;
  uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex) override;
  void setup_table_views() override;
  void count_scores();
  std::map<uint32_t, uint32_t> calculate_score_counts();
  void compute_dart_count_and_checkouts();
  void compute_first9_leg_average(const QVector<uint32_t> & iScores);

private:

  Ui::CStatsWindowX01 * mUi;
  const CX01Class::CPlayerData mPlayerData;
  std::array<uint32_t, static_cast<int>(EScoreCountsIdx::SCORE_COUNT_MAX)> mScoreCounts = {};
  std::array<uint32_t, static_cast<int>(EDartCountsIdx::SEG_MAX)> mSegmentCounts = {};
  QVector<uint32_t> mDartCountOfWonLegs = {};
  QVector<uint32_t> mAllCheckouts = {};
  SLegStatsData mLegStatsData;
  CLegStatsX01Model * mLegStatsModel = nullptr;
  CLegScoresX01Model * mLegScoresModel = nullptr;
  CGlobalGameStatsX01Model * mGlobalGameStatsModel = nullptr;
  CGlobalScoreStatsX01Model * mGlobalScoreStatsModel = nullptr;
  CGlobalSegmentStatsX01Model * mGlobalSegmentStatsModel = nullptr;
  SGlobalGameStatsData mGlobalGameStatsData;
};

#endif  // STATS_WINDOW_X01_H
