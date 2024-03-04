#ifndef STATS_WINDOW_IF_H
#define STATS_WINDOW_IF_H

#include "x01_class.h"
#include "cricket_class.h"

class CStatsWindowX01;
class CStatsWindowCricket;

class CStatsWindow
{

public:

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

  virtual ~CStatsWindow() = default;
  static CStatsWindowX01 * const create(const CX01Class::CPlayerData & iPlayerData, QWidget * iParent = nullptr);
  static CStatsWindowCricket * const create(const CCricketClass::CPlayerData & iPlayerData, QWidget * iParent = nullptr);

private:

  virtual void init_leg_selector() = 0;
  virtual void update_leg_history(int iIndex) = 0;
  virtual void calculate_segment_counts() = 0;
  virtual double compute_average(QVector<uint32_t> iScoresOfLeg) = 0;
  virtual uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex) = 0;
  virtual void setup_table_views() = 0;
};

Q_DECLARE_INTERFACE(CStatsWindow, "com.CStatsWindow")

#endif  // STATS_WINDOW_IF_H
