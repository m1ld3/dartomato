#include "dartboard_cricket.h"


CDartBoardCricket::CDartBoardCricket(CDartBoardView *iGraphicsViewDartBoard, const CSettings &ipSettings)
  : CDartBoard(iGraphicsViewDartBoard, ipSettings)
{
}

void CDartBoardCricket::segment_pressed_slot(uint32_t iVal, QChar &iType)
{
  emit signal_segment_pressed(iVal, iType);
}
