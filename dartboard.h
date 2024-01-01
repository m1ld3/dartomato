#ifndef DARTBOARD_H
#define DARTBOARD_H

#include "cdartboard_base.h"
#include "settings.h"

class CDartBoardX01 : public CDartBoard
{
  Q_OBJECT

public slots:

  void segment_pressed_slot(uint32_t iVal, QChar & iType) override;

signals:

  void signal_submit_button_pressed_to_game_window(uint32_t iScore, uint32_t iNumberOfDarts,
                                                   uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  void signal_set_score_to_group_box_player();
  void signal_erase_dart1();
  void signal_erase_dart2();
  void signal_erase_dart3();
  void signal_display_dart1(uint32_t iVal);
  void signal_display_dart2(uint32_t iVal);
  void signal_display_dart3(uint32_t iVal);
  void signal_display_score(uint32_t iScore);
  void signal_update_finishes(uint32_t iScore, uint32_t iNumberOfDarts);

public:

  CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, const CSettings & ipSettings);
  ~CDartBoardX01() override = default;
  void set_score(uint32_t iVal, QChar iType, uint32_t iCheckout);
  void init_dartboard(uint32_t iScore);
  void perform_undo();
  void submit_score();
  void display_score(uint32_t iScore);
  void display_dart1(uint32_t iVal);
  void display_dart2(uint32_t iVal);
  void display_dart3(uint32_t iVal);
  void erase_all_darts();
  void erase_dart1();
  void erase_dart2();
  void erase_dart3();
  void set_finished();
  void unset_finished();

private:

  uint32_t mScore = 0;
  uint32_t mCounter = 3;
  uint32_t mOldScore = 0;
  bool mStop = false;
  bool mBusted = false;
  bool mFinished = false;
  QVector<uint32_t> mUndo{0, 0, 0};
  QVector<QString> mDarts = {};
  QVector<uint32_t> mCheckoutAttempts = {0, 0, 0};
};

#endif  // DARTBOARD_H
