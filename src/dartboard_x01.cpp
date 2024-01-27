#include "dartboard_x01.h"
#include "x01_mainwindow.h"
#include <cmath>
#include <QMessageBox>

CDartBoardX01::CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, CX01MainWindow * iX01MainWindow, const CSettings & iSettings)
  : CDartBoard(iGraphicsViewDartBoard, iSettings)
  , mGameWindow(static_cast<CX01MainWindow*>(iX01MainWindow))
{
  erase_all_darts();
  display_score(mScore);
}

void CDartBoardX01::set_score(uint32_t iVal, QChar iType, bool iCheckoutAttempt)
{
  mScore -= iVal;
  display_score(mScore);

  if (mCounter > 0)
  {
    mUndo[3 - mCounter] = iVal;
    mDarts.append(mBusted ? "S0" : iType + QString::number(iVal));
    mCheckoutAttempts[3 - mCounter] = iCheckoutAttempt;
    update_finishes(mScore, mCounter - 1);
  }
  if (mCounter == 3)      display_dart1(iVal);
  else if (mCounter == 2) display_dart2(iVal);
  else if (mCounter == 1) display_dart3(iVal);
}

void CDartBoardX01::init_dartboard(uint32_t iScore)
{
  mStop = false;
  mBusted = false;
  mScore = iScore;
  mOldScore = mScore;
  mCheckoutAttempts = {false, false, false};
  erase_all_darts();
  display_score(mScore);
  mCounter = 3;
  mDarts = {};
}

void CDartBoardX01::handle_start_val(uint32_t iVal, QChar & iType)
{
  if (mSettings.SingleIn)      set_score(iVal, iType, false);
  else if (mSettings.DoubleIn) set_score(iType == 'd' ? iVal : 0, iType, false);
  else if (mSettings.MasterIn) set_score(iType == 't' ? iVal : 0, iType, false);
}

void CDartBoardX01::handle_score_in_range(uint32_t iVal, QChar & iType)
{
  set_score(iVal, iType, checkout_attempt_happened());
}

bool CDartBoardX01::checkout_attempt_happened()
{
  return ((mSettings.SingleOut && mScore <= 60 && !mBoogieNumbers.contains(mScore)) ||
         (mSettings.DoubleOut && ((mScore <= 40 && mScore % 2 == 0 && mScore > 1) || mScore == 50)) ||
         (mSettings.MasterOut && mScore <= 60 && mScore % 3 == 0 && mScore > 2));
}

void CDartBoardX01::handle_game_shot_score(uint32_t iVal, QChar & iType)
{
  mStop = true;  // Game shot
  set_focus_to_submit_button();
  play_game_shot_sound();
  set_score(iVal, iType, true);
}

void CDartBoardX01::handle_score_equals_remaining(uint32_t iVal, QChar & iType)
{
  if (mSettings.SingleOut)
  {
    handle_game_shot_score(iVal, iType);
  }
  else if (mSettings.DoubleOut)
  {
    if (iType == 'd') handle_game_shot_score(iVal, iType);
    else handle_busted_score(iVal, iType, (mScore > 1 && mScore % 2 == 0 && mScore <= 40) || mScore == 50);
  }
  else if (mSettings.MasterOut)
  {
    if (iType == 't') handle_game_shot_score(iVal, iType);
    else handle_busted_score(iVal, iType, mScore <= 60 && mScore % 3 == 0 && mScore > 2);
  }
}

void CDartBoardX01::handle_busted_score(uint32_t iVal, QChar & iType, bool iCheckoutAttempt)
{
  mStop = true;
  mBusted = true;
  set_focus_to_submit_button();
  set_score(iVal, iType, iCheckoutAttempt);
  display_score(mOldScore);
#ifndef USE_TTS
  mBustedSound.play();
#endif
}

bool CDartBoardX01::is_score_in_range(uint32_t iVal)
{
  if (mSettings.DoubleOut) return mScore  > (iVal + 1);
  else if (mSettings.MasterOut) return mScore > (iVal + 2);
  else return mScore > iVal;
}

void CDartBoardX01::handle_segment_pressed_event(uint32_t iVal, QChar & iType)
{
  if (mFinished)
  {
    QMessageBox::warning(this, "", "Game already finished!");
    return;
  }

  if (!mStop && mCounter > 0)
  {
    uint32_t startVal = static_cast<uint32_t>(mSettings.Game);
    if (mScore == startVal)
    {
      handle_start_val(iVal, iType);
    }
    else if (is_score_in_range(iVal))
    {
      handle_score_in_range(iVal, iType);
    }
    else if (mScore == iVal)
    {
      handle_score_equals_remaining(iVal, iType);
    }
    else
    {
      handle_busted_score(iVal, iType, checkout_attempt_happened());
    }

    mCounter--;
    if (mCounter == 0)
    {
      mStop = true;
      set_focus_to_submit_button();
    }
  }
  else if (mBusted)                     QMessageBox::warning(this, "Warning", "You are already busted!");
  else if (mScore == 0)                 QMessageBox::warning(this, "Warning", "You have already won this leg!");
  else if (mScore > 0 && mCounter == 0) QMessageBox::warning(this, "Warning", "You only have three darts!");
}

void CDartBoardX01::perform_undo()
{
  if (mCounter == 3) return;

  mScore += mUndo[2 - mCounter];
  mUndo[2 - mCounter] = 0;
  mDarts.pop_back();
  mCheckoutAttempts[2 - mCounter] = false;
  display_score(mScore);

  if (mCounter == 2)      erase_dart1();
  else if (mCounter == 1) erase_dart2();
  else if (mCounter == 0) erase_dart3();

  update_finishes(mScore, mCounter + 1);

  mCounter++;
  mStop = false;
  mBusted = false;
}

void CDartBoardX01::submit_score()
{
  if (mStop)
  {
    QVector<QString> darts = mDarts;
    uint32_t score = mOldScore - mScore;

    if (mBusted)
    {
      darts = {"S0", "S0", "S0"};
      score = 0;
    }

    uint32_t numberofdarts = 3 - mCounter;
    uint32_t checkoutattempts = static_cast<uint32_t>(std::count(mCheckoutAttempts.begin(), mCheckoutAttempts.end(), true));
    submit_score_to_player(score, numberofdarts, checkoutattempts, darts);
  }
  else if (!mFinished)
  {
    QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
  }
  else
  {
    QMessageBox::warning(this, "", "Game already finished!");
  }
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



