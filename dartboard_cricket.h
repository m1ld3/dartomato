#ifndef CDARTBOARDCRICKET_H
#define CDARTBOARDCRICKET_H

#include "cdartboard_base.h"
#include "settings.h"

class CDartBoardCricket : public CDartBoard
{
  Q_OBJECT

public slots:

  void segment_pressed_slot(uint32_t iVal, QChar & iType) override;

signals:

  void signal_segment_pressed(uint32_t iVal, QChar & iType);


public:

  CDartBoardCricket(CDartBoardView * iGraphicsViewDartBoard, const CSettings & ipSettings);
  ~CDartBoardCricket() override = default;
};

#endif  // CDARTBOARDCRICKET_H
