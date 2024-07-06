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

#ifndef TESTING
class CStatsWindowCricket : public QDialog, public IStatsWindow
{
  Q_OBJECT
//  Q_INTERFACES(CStatsWindow)
#else
class CStatsWindowCricket : public IStatsWindow
{
  friend class CStatsWindowCricketTest;
#endif

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


    bool operator==(const SLegStatsData & iOther)
    {
      return (HitsPerRound == iOther.HitsPerRound &&
              AvgLegDartCount == iOther.AvgLegDartCount &&
              BestWonLegDartCount == iOther.BestWonLegDartCount &&
              WorstWonLegDartCount == iOther.WorstWonLegDartCount &&
              DartCountOfCurrentLeg == iOther.DartCountOfCurrentLeg);
    }
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

#ifndef TESTING
private slots:
#else
private:
#endif

  void update_leg_history_slot(int iIndex) { update_leg_history(iIndex); }


private:

  void update_leg_history(int iIndex) override;
  void init_leg_selector() override;
  void calculate_segment_counts() override;
  uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex) override;
  void setup_table_views() override;
  void compute_dart_count_of_won_legs();
  void get_global_game_stats_data();
  void compute_hits_per_round(const QVector<QVector<QString> > &iTotalDarts, const QVector<uint32_t> &iTotalHits);
  void update_leg_scores_table_view(const QVector<QVector<QString> > &iTotalDarts);
  void update_leg_stats_table_view();

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
