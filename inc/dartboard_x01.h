#ifndef DARTBOARD_X01_H
#define DARTBOARD_X01_H

#include "dartboard_base.h"
#include "settings.h"

class CX01MainWindow;

class CDartBoardX01 : public CDartBoard
{
  Q_OBJECT

public:

  CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, CX01MainWindow * ipX01MainWindow, const CSettings & ipSettings);
  ~CDartBoardX01() override = default;
  void init_dartboard(uint32_t iScore);
  void perform_undo();
  void submit_score();
  void set_finished();
  void unset_finished();
  void handle_segment_pressed_event(uint32_t iVal, QChar & iType) override;
  
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
  void handle_busted_score(uint32_t iVal, QChar & iType, bool iCheckoutAttempt);
  void handle_game_shot_score(uint32_t iVal, QChar & iType);
  bool is_score_in_range(uint32_t iVal);

  CX01MainWindow * mpGameWindow;
  uint32_t mScore = 0;
  uint32_t mCounter = 3;
  uint32_t mOldScore = 0;
  bool mStop = false;
  bool mBusted = false;
  bool mFinished = false;
  QVector<uint32_t> mUndo{0, 0, 0};
  QVector<QString> mDarts = {};
  QVector<bool> mCheckoutAttempts = {false, false, false};
  QVector<uint32_t> mBoogieNumbers = {23, 29, 31, 35, 37, 41, 43, 44, 46, 47, 49, 52, 53, 55, 56, 58, 59};
};

#endif  // DARTBOARD_X01_H
