#ifndef STATS_WINDOW_IF_H
#define STATS_WINDOW_IF_H

#include "x01_class.h"
#include "cricket_class.h"

class CStatsWindowX01;
class CStatsWindowCricket;
class QWidget;

class IStatsWindow
{

public:

  virtual ~IStatsWindow() = default;
  static CStatsWindowX01 * const create(const CX01Class::CPlayerData & iPlayerData, QWidget * iParent = nullptr);
  static CStatsWindowCricket * const create(const CCricketClass::CPlayerData & iPlayerData, QWidget * iParent = nullptr);

private:

  virtual void init_leg_selector() = 0;
  virtual void update_leg_history(int iIndex) = 0;
  virtual void calculate_segment_counts() = 0;
  virtual uint32_t compute_dart_count_of_indexed_leg(uint32_t iIndex) = 0;
  virtual void setup_table_views() = 0;
};

//Q_DECLARE_INTERFACE(CStatsWindow, "com.CStatsWindow")

#endif  // STATS_WINDOW_IF_H
