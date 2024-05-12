#ifndef CDARTBOARDCRICKET_H
#define CDARTBOARDCRICKET_H

#include "dartboard_base.h"
#include "settings.h"

class CCricketInput;

class CDartBoardCricket : public CDartBoard
{
  Q_OBJECT

public:

  CDartBoardCricket(CDartBoardView * iGraphicsViewDartBoard, const CSettings & iSettings, CCricketInput * iCricketInput);
  ~CDartBoardCricket() override = default;
  void handle_segment_pressed_event(uint32_t iVal, QChar iType) override;

private:

  CCricketInput * mCricketInput;
};

#endif  // CDARTBOARDCRICKET_H
