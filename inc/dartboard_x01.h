#ifndef DARTBOARD_X01_H
#define DARTBOARD_X01_H

#include "dartboard_base.h"
#include "settings.h"
#ifdef TESTING
#include "mocks.h"
#include <sstream>
#endif

class IX01MainWindow;

#ifdef TESTING
class CDartBoardX01 : public CDartBoardMock
{
  friend class CDartBoardX01Test;
#else
class CDartBoardX01 : public CDartBoard
{
  Q_OBJECT
#endif
public:

#ifdef TESTING
  CDartBoardX01(IX01MainWindow * iMainWindow)
    : mGameWindow(iMainWindow)
    , CDartBoardMock()
  {}
#else
  CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, IX01MainWindow * iX01MainWindow, const CSettings iSettings);
  ~CDartBoardX01() override = default;
#endif

  void perform_undo() override;
  void submit_score() override;
  void set_finished() override;
  void init_dartboard(uint32_t iScore) override;
  void unset_finished() override;
  void handle_segment_pressed_event(uint32_t iVal, QChar iType) override;
  
private:

  void set_score(uint32_t iVal, QChar iType, bool iCheckoutAttempt);
  void display_score(uint32_t iScore);
  void display_dart1(uint32_t iVal);
  void display_dart2(uint32_t iVal);
  void display_dart3(uint32_t iVal);
  void erase_all_darts();
  void erase_dart1();
  void erase_dart2();
  void erase_dart3();
  void set_focus_to_submit_button();
  void update_finishes(uint32_t iScore, uint32_t iNumberOfDarts);
  void submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  void handle_start_val(uint32_t iVal, QChar & iType);
  void handle_score_in_range(uint32_t iVal, QChar & iType);
  bool checkout_attempt_happened();
  void handle_score_equals_remaining(uint32_t iVal, QChar & iType);
  void handle_busted_score(QChar & iType, bool iCheckoutAttempt);
  void handle_game_shot_score(uint32_t iVal, QChar & iType);
  bool is_score_in_range(uint32_t iVal);

  IX01MainWindow * mGameWindow = nullptr;
  uint32_t mScore = 0;
  uint32_t mCounter = 3;
  uint32_t mOldScore = 0;
  bool mStop = false;
  bool mBusted = false;
  bool mFinished = false;
  QVector<uint32_t> mUndo{0, 0, 0};
  QVector<QString> mDarts = {};
  QVector<bool> mCheckoutAttempts = {false, false, false};
  const QVector<uint32_t> mBoogieNumbers = {23, 29, 31, 35, 37, 41, 43, 44, 46, 47, 49, 52, 53, 55, 56, 58, 59};
#ifdef TESTING
  std::stringstream mOutput = std::stringstream("");
#endif
};
#endif  // DARTBOARD_X01_H
