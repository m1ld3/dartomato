#include "dartboard_cricket.h"
#include "cricketinput.h"

CDartBoardCricket::CDartBoardCricket(CDartBoardView * iGraphicsViewDartBoard, const CSettings & ipSettings, CCricketInput * ipCricketInput)
  : CDartBoard(iGraphicsViewDartBoard, ipSettings)
  , mpCricketInput(ipCricketInput)
{
  mS14->set_value(0);
  mS13->set_value(0);
  mS12->set_value(0);
  mS11->set_value(0);
  mS10->set_value(0);
  mS9->set_value(0);
  mS8->set_value(0);
  mS7->set_value(0);
  mS6->set_value(0);
  mS5->set_value(0);
  mS4->set_value(0);
  mS3->set_value(0);
  mS2->set_value(0);
  mS1->set_value(0);
  mD14->set_value(0);
  mD13->set_value(0);
  mD12->set_value(0);
  mD11->set_value(0);
  mD10->set_value(0);
  mD9->set_value(0);
  mD8->set_value(0);
  mD7->set_value(0);
  mD6->set_value(0);
  mD5->set_value(0);
  mD4->set_value(0);
  mD3->set_value(0);
  mD2->set_value(0);
  mD1->set_value(0);
  mT14->set_value(0);
  mT13->set_value(0);
  mT12->set_value(0);
  mT11->set_value(0);
  mT10->set_value(0);
  mT9->set_value(0);
  mT8->set_value(0);
  mT7->set_value(0);
  mT6->set_value(0);
  mT5->set_value(0);
  mT4->set_value(0);
  mT3->set_value(0);
  mT2->set_value(0);
  mT1->set_value(0);
}

void CDartBoardCricket::handle_segment_pressed_event(uint32_t iVal, QChar &iType)
{
  mpCricketInput->handle_segment_pressed_event(iVal, iType);
}
