#include "x01_class.h"
#include "cricket_class.h"
#include "stats_window_if.h"
#include "stats_window_x01.h"
#include "stats_window_cricket.h"

CStatsWindowX01 * const CStatsWindow::create(const CX01Class::CPlayerData & iPlayerData, QWidget * iParent)
{
  return new CStatsWindowX01(iPlayerData, iParent);
}

CStatsWindowCricket * const CStatsWindow::create(const CCricketClass::CPlayerData & iPlayerData, QWidget * iParent)
{
  return new CStatsWindowCricket(iPlayerData, iParent);
}
