#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QDialog>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include <QGraphicsSvgItem>
#include <QLabel>
#include "dartboardview.h"
#include <QSoundEffect>

class CDartBoard : public QWidget
{
  Q_OBJECT

public slots:

  void signalSegmentPressed(uint32_t & iValue, QChar & iType);

signals:

  void signal_submit_button_pressed_to_game_window(uint32_t &iScore, uint32_t &iNumberOfDarts, uint32_t &iCheckoutAttempts, QVector<QString> iDarts);
  void signal_set_score_to_group_box_player();
  void signal_erase_dart1();
  void signal_erase_dart2();
  void signal_erase_dart3();
  void signal_display_dart1(uint32_t iVal);
  void signal_display_dart2(uint32_t iVal);
  void signal_display_dart3(uint32_t iVal);
  void signal_display_score(uint32_t iScore);
  void signal_update_finishes(uint32_t iScore, uint32_t iNumberOfDarts);
  void signal_set_focus_to_submit_button();

public:

  CDartBoard(CDartBoardView * iGraphicsViewDartBoard, uint32_t iStartVal, uint32_t iScore, bool iSingleIn, bool iSingleOut,
             bool iDoubleIn, bool iDoubleOut, bool iMasterIn, bool iMasterOut);
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

  QGraphicsView* mView;
  uint32_t mStartVal, mScore, mCounter, mOldScore;
  bool mSingleIn, mSingleOut, mDoubleIn, mDoubleOut, mMasterIn, mMasterOut, mStop, mBusted, mFinished;
  QVector<uint32_t> mUndo{0,0,0};
  QVector<QString> mDarts;
  QVector<uint32_t> mCheckoutAttempts;
  QGraphicsScene *mScene;
  QSoundEffect mBustedSound;
  QSoundEffect mGameShotSound;
};

#endif  // DARTBOARD_H
