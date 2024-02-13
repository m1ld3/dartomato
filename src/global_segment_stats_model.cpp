#include "global_segment_stats_model.h"

CGlobalSegmentStatsModel::CGlobalSegmentStatsModel(static_cast<int>(CStatsWindow::EDartCountsIdx::SEG_MAX)> iSegments, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mSegments(iSegments)
{}
