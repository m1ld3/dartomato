#ifndef CDARTBOARDCRICKET_H
#define CDARTBOARDCRICKET_H

#include "dartboard_base.h"
#include "settings.h"
#ifdef TESTING
#include "mocks.h"
#endif

class CCricketInput;

#ifdef TESTING
class CDartBoardCricket : public CDartBoardMock
{
#else
class CDartBoardCricket : public CDartBoard
{
  Q_OBJECT
#endif
public:

#ifdef TESTING
  CDartBoardCricket()
    : CDartBoardMock()
  {}
#else
  CDartBoardCricket(CDartBoardView * iGraphicsViewDartBoard, const CSettings & iSettings, CCricketInput * iCricketInput);
  ~CDartBoardCricket() override = default;
#endif

  void handle_segment_pressed_event(uint32_t iVal, QChar iType) override;
  void set_finished() override { /*unused*/ }
  void unset_finished() override { /*unused*/ }
  void init_dartboard([[maybe_unused]] uint32_t iScore) override { /*unused*/ }
  void perform_undo() override { /*unused*/ };
  void submit_score() override { /*unused*/ };

private:

  CCricketInput * mCricketInput;
};

#endif  // CDARTBOARDCRICKET_H
