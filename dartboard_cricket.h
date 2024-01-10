#ifndef CDARTBOARDCRICKET_H
#define CDARTBOARDCRICKET_H

#include "cdartboard_base.h"
#include "settings.h"

class CCricketInput;

class CDartBoardCricket : public CDartBoard
{
  Q_OBJECT

public slots:

  void handle_segment_pressed_event(uint32_t iVal, QChar & iType) override;

public:

  CDartBoardCricket(CDartBoardView * iGraphicsViewDartBoard, const CSettings & ipSettings, CCricketInput * ipCricketInput);
  ~CDartBoardCricket() override = default;

private:

  CCricketInput * mpCricketInput;
};

#endif  // CDARTBOARDCRICKET_H
