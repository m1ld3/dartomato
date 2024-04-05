#ifndef STATS_WINDOW_CRICKET_H
#define STATS_WINDOW_CRICKET_H

#include <QString>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QProxyStyle>
#include "stats_window_if.h"
#include <QDialog>

namespace Ui
{
  class CStatsWindowCricket;
}

class CLegStatsCricketModel;
class CLegScoresCricketModel;
class CGlobalGameStatsCricketModel;
class CGlobalSegmentStatsCricketModel;

class CStatsWindowCricket : public QDialog, public CStatsWindow
{
  Q_OBJECT
  Q_INTERFACES(CStatsWindow)

public:

  enum class EDartCountsIdx
  {
      SEG_15  = 0
    , SEG_16  = 1
    , SEG_17  = 2
    , SEG_18  = 3
    , SEG_19  = 4
    , SEG_20  = 5
    , SEG_25  = 6
    , SEG_0   = 6
    , SEG_MAX = 21
  };

  struct SLegStatsData
  {
    double HitsPerRound = 0.0;
    double AvgLegDartCount = 0.0;
    uint32_t BestWonLegDartCount = 0;
    uint32_t WorstWonLegDartCount = 0;
    uint32_t DartCountOfCurrentLeg = 0;
  };

  struct SGlobalGameStatsData
  {
    double HitsPerRound = 0.0;
    uint32_t TotalHits = 0;
    uint32_t LegsWon = 0;
    uint32_t NumLegs = 0;
  };

  explicit CStatsWindowCricket(const CCricketClass::CPlayerData iPlayerData, QWidget * iParent = nullptr);
  ~CStatsWindowCricket() override;

private slots:

  void update_leg_history_slot(int iIndex) { update_leg_history(iIndex); }


private:

  void update_leg_history(int iIndex) override;
  void init_leg_selector() override;
  void calculate_segment_counts() override;
  uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex) override;
  void setup_table_views() override;
  void compute_dart_count_of_won_legs();

private:

  Ui::CStatsWindowCricket * mUi;
  const CCricketClass::CPlayerData mPlayerData;
  std::array<uint32_t, static_cast<int>(EDartCountsIdx::SEG_MAX)> mSegmentCounts = {};
  QVector<uint32_t> mDartCountOfWonLegs = {};
  QVector<uint32_t> mAllCheckouts = {};
  CLegStatsCricketModel * mLegStatsModel = nullptr;
  CLegScoresCricketModel * mLegScoresModel = nullptr;
  CGlobalGameStatsCricketModel * mGlobalGameStatsModel = nullptr;
  CGlobalSegmentStatsCricketModel * mGlobalSegmentStatsModel = nullptr;
  SGlobalGameStatsData mGlobalGameStatsData;
  SLegStatsData mLegStatsData;
};

#endif  // STATS_WINDOW_CRICKET_H
