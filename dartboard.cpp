#include "dartboard.h"
#include <cmath>
#include <QMessageBox>

CDartBoardX01::CDartBoardX01(CDartBoardView * iGraphicsViewDartBoard, const CSettings & ipSettings)
  : CDartBoard(iGraphicsViewDartBoard, ipSettings)
{
  erase_all_darts();
  display_score(mScore);
}

void CDartBoardX01::set_score(uint32_t iVal, QChar iType, uint32_t iCheckout)
{
  mScore -= iVal;
  display_score(mScore);
  if (mCounter == 3)
  {
    display_dart1(iVal);
    emit signal_update_finishes(mScore, 2);
    mUndo[0] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType + QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[0] = iCheckout;
  }
  else if (mCounter == 2)
  {
    display_dart2(iVal);
    emit signal_update_finishes(mScore, 1);
    mUndo[1] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType+QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[1] = iCheckout;
  }
  else if (mCounter == 1)
  {
    display_dart3(iVal);
    mUndo[2] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType+QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[2] = iCheckout;
  }
}

void CDartBoardX01::init_dartboard(uint32_t iScore)
{
  mStop = false;
  mBusted = false;
  mScore = iScore;
  mOldScore = mScore;
  mCheckoutAttempts = {0, 0, 0};
  erase_all_darts();
  display_score(mScore);
  mCounter = 3;
  mDarts = {};
}

void CDartBoardX01::segment_pressed_slot(uint32_t iVal, QChar & iType)
{
  if (mFinished)
  {
    QMessageBox::warning(this, "", "Game already finished!");
    return;
  }
  QVector<uint32_t> array {23, 29, 31, 35, 37, 41, 43, 44, 46, 47, 49, 52, 53, 55, 56, 58, 59};
  uint32_t checkout = 0;
  if (!mStop)
  {
    if (mCounter > 0)
    {
      uint32_t startVal = static_cast<uint32_t>(mpSettings.mGame);
      if (mScore == startVal)
      {
        if (mpSettings.mSingleIn)
        {
          set_score(iVal, iType, checkout);
        }
        else if (mpSettings.mDoubleIn)
        {
          if (iType == 'd')
          {
            set_score(iVal, iType, checkout);
          } else
          {
            set_score(0, iType, checkout);
          }
        }
        else if (mpSettings.mMasterIn)
        {
          if (iType == 't')
          {
            set_score(iVal, iType, checkout);
          }
          else
          {
            set_score(0, iType, checkout);
          }
        }
      }
      else if (mScore > iVal && mScore < startVal && mpSettings.mSingleOut)
      {
        if (mScore <= 60 && !(std::find(array.begin(), array.end(), mScore)))
        {
          checkout = 1;
        }
        set_score(iVal, iType, checkout);
      }
      else if (mScore - iVal > 1 && mScore < startVal && mpSettings.mDoubleOut)
      {
        if ((mScore <= 40 && mScore % 2 == 0 && mScore > 1) || mScore == 50)
        {
          checkout = 1;
        }
        set_score(iVal, iType, checkout);
      }
      else if (mScore - iVal > 2 && mScore < startVal && mpSettings.mMasterOut)
      {
        if (mScore <= 60 && mScore % 3 == 0 && mScore > 2)
        {
          checkout = 1;
        }
        set_score(iVal, iType, checkout);
      }
      else if (mScore == iVal)
      {
        if (mpSettings.mSingleOut)
        {
          mStop = true;  // Game shot
          emit signal_set_focus_to_submit_button();
          play_game_shot_sound();
          checkout = 1;
          set_score(iVal, iType, checkout);
        } else if (mpSettings.mDoubleOut)
        {
          if (iType == 'd')
          {
            mStop = true;  // Game shot
            emit signal_set_focus_to_submit_button();
            play_game_shot_sound();
            checkout = 1;
            set_score(iVal, iType, checkout);
          }
          else
          {
            if ((mScore > 1 && mScore % 2 == 0 && mScore <= 40) || mScore == 50)
            {
              checkout = 1;
            }
            set_score(iVal, iType, checkout);
            display_score(mOldScore);
            mStop = true;  // Überwofen
            mBusted = true;
            emit signal_set_focus_to_submit_button();
            mBustedSound.play();
          }
        }
        else if (mpSettings.mMasterOut)
        {
          if (iType == 't')
          {
            checkout = 1;
            mStop = true;  // Game shot
            play_game_shot_sound();
            set_score(iVal, iType, checkout);
          }
          else
          {
            if (mScore <= 60 && mScore % 3 == 0 && mScore > 2)
            {
              checkout = 1;
            }
            set_score(iVal, iType, checkout);
            display_score(mOldScore);
            mStop = true;  // Überwofen
            mBusted = true;
            emit signal_set_focus_to_submit_button();
            mBustedSound.play();
          }
        }
      }
      else
      {
        if (mpSettings.mSingleOut && (mScore <= 60 && !(std::find(array.begin(), array.end(), mScore))))
        {
          checkout = 1;
        }
        else if (mpSettings.mDoubleOut && ((mScore <= 40 && mScore % 2 == 0 && mScore > 1) || mScore == 50))
        {
          checkout = 1;
        }
        else if (mpSettings.mMasterOut && (mScore <= 60 && mScore % 3 == 0 && mScore > 2))
        {
          checkout = 1;
        }
        mStop = true;
        mBusted = true;  // Überworfen
        emit signal_set_focus_to_submit_button();
        set_score(iVal, iType, checkout);
        display_score(mOldScore);
        mBustedSound.play();
      }

      mCounter--;
      if (mCounter == 0)
      {
        mStop = true;
        emit signal_set_focus_to_submit_button();
      }
    }
  }
  else if (mBusted)
  {
    QMessageBox::warning(this, "Warning", "You are already busted!");
  }
  else if (mScore == 0)
  {
    QMessageBox::warning(this, "Warning", "You have already won this leg!");
  }
  else if (mScore > 0 && mCounter == 0)
  {
    QMessageBox::warning(this, "Warning", "You only have three darts!");
  }
}

void CDartBoardX01::perform_undo()
{
  if (mCounter < 3)
  {
    mScore += mUndo[2 - mCounter];
    mUndo[2 - mCounter] = 0;
    mDarts.pop_back();
    mCheckoutAttempts[2 - mCounter] = 0;
    display_score(mScore);

    if (mCounter == 2)
    {
      erase_dart1();
      emit signal_update_finishes(mScore, 3);
    }
    else if (mCounter == 1)
    {
      erase_dart2();
      emit signal_update_finishes(mScore, 2);
    }
    else if (mCounter == 0)
    {
      erase_dart3();
      emit signal_update_finishes(mScore, 1);
    }

    mCounter++;
    mStop = false;
    mBusted = false;
  }
}

void CDartBoardX01::submit_score()
{
  if (mStop)
  {
    QVector<QString> darts = mDarts;
    uint32_t score;
    if (mBusted)
    {
      darts = {"S0", "S0", "S0"};
      score = 0;
    }
    else
    {
      score = mOldScore - mScore;
    }
    uint32_t numberofdarts = 3 - mCounter;
    uint32_t checkoutattempts = std::accumulate(mCheckoutAttempts.begin(), mCheckoutAttempts.end(), 0);
    emit signal_submit_button_pressed_to_game_window(score, numberofdarts, checkoutattempts, darts);
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
  emit signal_display_score(iScore);
}

void CDartBoardX01::display_dart1(uint32_t iVal)
{
  emit signal_display_dart1(iVal);
}

void CDartBoardX01::display_dart2(uint32_t iVal)
{
  emit signal_display_dart2(iVal);
}

void CDartBoardX01::display_dart3(uint32_t iVal)
{
  emit signal_display_dart3(iVal);
}

void CDartBoardX01::erase_all_darts()
{
  erase_dart1();
  erase_dart2();
  erase_dart3();
}

void CDartBoardX01::erase_dart1()
{
  emit signal_erase_dart1();
}

void CDartBoardX01::erase_dart2()
{
  emit signal_erase_dart2();
}

void CDartBoardX01::erase_dart3()
{
  emit signal_erase_dart3();
}

void CDartBoardX01::set_finished()
{
  mFinished = true;
}

void CDartBoardX01::unset_finished()
{
  mFinished = false;
}



