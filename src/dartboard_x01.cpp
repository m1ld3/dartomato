#include "dartboard_x01.h"
#include "x01_mainwindow.h"
#include <cmath>
#include <QMessageBox>

#ifndef TESTING
#define PUT_WARNING(TITLE, MESSAGE) \
  QMessageBox::warning(this, (TITLE), (MESSAGE));
#else
#include <iostream>
#define PUT_WARNING(TITLE, MESSAGE) \
  mOutput << TITLE << ": " << MESSAGE;
#endif

#ifndef TESTING
CDartBoardX01::CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, IX01MainWindow * iX01MainWindow, const CSettings iSettings)
  : CDartBoard(iGraphicsViewDartBoard, iSettings)
  , mGameWindow(iX01MainWindow)
{
  erase_all_darts();
  display_remaining(mRemaining);
  display_score(0);
}
#endif

void CDartBoardX01::set_scores(uint32_t iVal, QChar iType, bool iCheckoutAttempt)
{
  mRemaining -= iVal;
  display_remaining(mRemaining);

  if (mCounter > 0)
  {
    mUndo[3 - mCounter] = iVal;
    mDarts.append(mBusted ? "s0" : iType + QString::number(iVal));
    mCheckoutAttempts[3 - mCounter] = iCheckoutAttempt;
    update_finishes(mRemaining, mCounter - 1);
  }
  if      (mCounter == 3) display_dart1(iVal);
  else if (mCounter == 2) display_dart2(iVal);
  else if (mCounter == 1) display_dart3(iVal);
  display_score(std::accumulate(mUndo.begin(), mUndo.end(), 0));
}

void CDartBoardX01::init_dartboard(uint32_t iStartVal)
{
  mStop = false;
  mBusted = false;
  mRemaining = iStartVal;
  mOldRemaining = mRemaining;
  mOldScore = 0;
  mCheckoutAttempts = {false, false, false};
  erase_all_darts();
  display_remaining(mRemaining);
  mCounter = 3;
  mDarts = {};
  mUndo = {0, 0, 0};
  display_score(std::accumulate(mUndo.begin(), mUndo.end(), 0));
}

void CDartBoardX01::handle_start_val(uint32_t iVal, QChar & iType)
{
  switch(mSettings.InMode)
  {
  case EX01InMode::SINGLE_IN:
    set_scores(iVal, iType, false);
    break;
  case EX01InMode::DOUBLE_IN:
    set_scores(iType == 'd' ? iVal : 0, iType, false);
    break;
  case EX01InMode::MASTER_IN:
    set_scores(iType == 't' ? iVal : 0, iType, false);
    break;
  default:;
  }
}

void CDartBoardX01::handle_score_in_range(uint32_t iVal, QChar & iType)
{
  set_scores(iVal, iType, checkout_attempt_happened());
}

bool CDartBoardX01::checkout_attempt_happened()
{
  return ((mSettings.OutMode == EX01OutMode::SINGLE_OUT && mRemaining <= 60 && !mBoogieNumbers.contains(mRemaining)) ||
          (mSettings.OutMode == EX01OutMode::DOUBLE_OUT && ((mRemaining <= 40 && mRemaining % 2 == 0 && mRemaining > 1) || mRemaining == 50)) ||
          (mSettings.OutMode == EX01OutMode::MASTER_OUT && mRemaining <= 60 && mRemaining % 3 == 0 && mRemaining > 2));
}

void CDartBoardX01::handle_game_shot_score(uint32_t iVal, QChar & iType)
{
  mStop = true;  // Game shot
  set_focus_to_submit_button();
  play_game_shot_sound();
  set_scores(iVal, iType, true);
}

void CDartBoardX01::handle_score_equals_remaining(uint32_t iVal, QChar & iType)
{
  switch(mSettings.OutMode)
  {
  case EX01OutMode::SINGLE_OUT:
    handle_game_shot_score(iVal, iType);
    break;
  case EX01OutMode::DOUBLE_OUT:
    if (iType == 'd') handle_game_shot_score(iVal, iType);
    else handle_busted_score(iType, (mRemaining > 1 && mRemaining % 2 == 0 && mRemaining <= 40) || mRemaining == 50);
    break;
  case EX01OutMode::MASTER_OUT:
    if (iType == 't') handle_game_shot_score(iVal, iType);
    else handle_busted_score(iType, mRemaining <= 60 && mRemaining % 3 == 0 && mRemaining > 2);
    break;
  default:;
  }
}

void CDartBoardX01::handle_busted_score(QChar & iType, bool iCheckoutAttempt)
{
  mStop = true;
  mBusted = true;
  set_focus_to_submit_button();
  set_scores(0, iType, iCheckoutAttempt);
#ifndef TESTING
  mSoundHandler.play_busted_sound();
#endif
}

bool CDartBoardX01::is_score_in_range(uint32_t iVal)
{
  if (mSettings.OutMode == EX01OutMode::DOUBLE_OUT) return mRemaining  > (iVal + 1);
  else if (mSettings.OutMode == EX01OutMode::MASTER_OUT) return mRemaining > (iVal + 2);
  else return mRemaining > iVal;
}

void CDartBoardX01::handle_segment_pressed_event(uint32_t iVal, QChar iType)
{
  if (mFinished)
  {
    PUT_WARNING("", "Game already finished!")
    return;
  }

  if (!mStop && mCounter > 0)
  {
    uint32_t startVal = static_cast<uint32_t>(mSettings.Game);
    if (mRemaining == startVal)
    {
      handle_start_val(iVal, iType);
    }
    else if (is_score_in_range(iVal))
    {
      handle_score_in_range(iVal, iType);
    }
    else if (mRemaining == iVal)
    {
      handle_score_equals_remaining(iVal, iType);
    }
    else
    {
      handle_busted_score(iType, checkout_attempt_happened());
    }

    mCounter--;
    if (mCounter == 0)
    {
      mStop = true;
      set_focus_to_submit_button();
    }
  }
  else if (mBusted)                     PUT_WARNING("Warning", "You are already busted!")
  else if (mRemaining == 0)                 PUT_WARNING("Warning", "You have already won this leg!")
  else if (mRemaining > 0 && mCounter == 0) PUT_WARNING("Warning", "You only have three darts!")
}

void CDartBoardX01::perform_undo()
{
  if (mCounter == 3) return;

  mRemaining += mUndo[2 - mCounter];
  mUndo[2 - mCounter] = 0;
  mDarts.pop_back();
  mCheckoutAttempts[2 - mCounter] = false;
  display_remaining(mRemaining);
  display_score(std::accumulate(mUndo.begin(), mUndo.end(), 0));

  if (mCounter == 2)      erase_dart1();
  else if (mCounter == 1) erase_dart2();
  else if (mCounter == 0) erase_dart3();

  mCounter++;
  update_finishes(mRemaining, mCounter);

  mStop = false;
  mBusted = false;
}

void CDartBoardX01::submit_score()
{
  if (mStop)
  {
    QVector<QString> darts = mDarts;
    uint32_t score = mOldRemaining - mRemaining;
    uint32_t numberOfDarts = 3 - mCounter;

    if (mBusted)
    {
      darts = {"s0", "s0", "s0"};
      score = 0;
      numberOfDarts = 3;
    }

    uint32_t checkoutattempts = static_cast<uint32_t>(std::count(mCheckoutAttempts.begin(), mCheckoutAttempts.end(), true));
    submit_score_to_player(score, numberOfDarts, checkoutattempts, darts);
  }
  else if (!mFinished)
  {
    PUT_WARNING("Score incomplete", "Please enter all darts.")
  }
  else
  {
    PUT_WARNING("", "Game already finished!")
  }
}

void CDartBoardX01::display_remaining(uint32_t iRemaining)
{
  mGameWindow->display_remaining(iRemaining);
}

void CDartBoardX01::display_score(uint32_t iScore)
{
  mGameWindow->display_score(iScore);
}

void CDartBoardX01::display_dart1(uint32_t iVal)
{
  mGameWindow->display_dart1(iVal);
}

void CDartBoardX01::display_dart2(uint32_t iVal)
{
  mGameWindow->display_dart2(iVal);
}

void CDartBoardX01::display_dart3(uint32_t iVal)
{
  mGameWindow->display_dart3(iVal);
}

void CDartBoardX01::erase_all_darts()
{
  erase_dart1();
  erase_dart2();
  erase_dart3();
}

void CDartBoardX01::erase_dart1()
{
  mGameWindow->erase_dart1();
}

void CDartBoardX01::erase_dart2()
{
  mGameWindow->erase_dart2();
}

void CDartBoardX01::erase_dart3()
{
  mGameWindow->erase_dart3();
}

void CDartBoardX01::set_finished()
{
  mFinished = true;
}

void CDartBoardX01::unset_finished()
{
  mFinished = false;
}

void CDartBoardX01::submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> &iDarts)
{
  mGameWindow->submit_score_to_player(iScore, iNumberOfDarts, iCheckoutAttempts, iDarts);
}

void CDartBoardX01::update_finishes(uint32_t iScore, uint32_t iNumberOfDarts)
{
  mGameWindow->update_finishes(iScore, iNumberOfDarts);
}

void CDartBoardX01::set_focus_to_submit_button()
{
  mGameWindow->set_focus_to_submit_button();
}

