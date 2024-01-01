#include "cricketinput.h"
#include "ui_cricketinput.h"
#include <QGraphicsPathItem>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsLinearLayout>
#include <QtDebug>
#include <cmath>
#include <QMessageBox>
#include <QVector>
#include "cricketclass.h"

CCricketInput::CCricketInput(QWidget * iParent, const CSettings & ipSettings, CCricketClass * iPlayer, CCricketMainWindow * iGameWindow)
  : QDialog(iParent)
  , mUi(new Ui::CCricketInput)
  , mPlayer(iPlayer)
  , mGameWindow(iGameWindow)
  , mSlot15(mPlayer->get_slot15())
  , mSlot16(mPlayer->get_slot16())
  , mSlot17(mPlayer->get_slot17())
  , mSlot18(mPlayer->get_slot18())
  , mSlot19(mPlayer->get_slot19())
  , mSlot20(mPlayer->get_slot20())
  , mSlot25(mPlayer->get_slot25())
  , mExtra15(mPlayer->get_extra15())
  , mExtra16(mPlayer->get_extra16())
  , mExtra17(mPlayer->get_extra17())
  , mExtra18(mPlayer->get_extra18())
  , mExtra19(mPlayer->get_extra19())
  , mExtra20(mPlayer->get_extra20())
  , mExtra25(mPlayer->get_extra25())
  , mScore(mPlayer->get_score())
  , mExtra15s(mGameWindow->compute_extra15s(0, mPlayer->get_player_number()))
  , mExtra16s(mGameWindow->compute_extra16s(0, mPlayer->get_player_number()))
  , mExtra17s(mGameWindow->compute_extra17s(0, mPlayer->get_player_number()))
  , mExtra18s(mGameWindow->compute_extra18s(0, mPlayer->get_player_number()))
  , mExtra19s(mGameWindow->compute_extra19s(0, mPlayer->get_player_number()))
  , mExtra20s(mGameWindow->compute_extra20s(0, mPlayer->get_player_number()))
  , mExtra25s(mGameWindow->compute_extra25s(0, mPlayer->get_player_number()))
  , mSlot15Array({mSlot15})
  , mSlot16Array({mSlot16})
  , mSlot17Array({mSlot17})
  , mSlot18Array({mSlot18})
  , mSlot19Array({mSlot19})
  , mSlot20Array({mSlot20})
  , mSlot25Array({mSlot25})
  , mExtra15Array({mExtra15})
  , mExtra16Array({mExtra16})
  , mExtra17Array({mExtra17})
  , mExtra18Array({mExtra18})
  , mExtra19Array({mExtra19})
  , mExtra20Array({mExtra20})
  , mExtra25Array({mExtra25})
  , mExtra15sArray({mExtra15s})
  , mExtra16sArray({mExtra16s})
  , mExtra17sArray({mExtra17s})
  , mExtra18sArray({mExtra18s})
  , mExtra19sArray({mExtra19s})
  , mExtra20sArray({mExtra20s})
  , mExtra25sArray({mExtra25s})
  , mGameShotSound(this)
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  mUi->submitButton->setAutoDefault(true);

  mDartBoard = new CDartBoardCricket(mUi->graphicsView_dartboard, mpSettings);

  connect(mUi->submitButton, &QPushButton::clicked, this, &CCricketInput::submit_button_clicked_slot);
  connect(mUi->undoButton, &QPushButton::clicked, this, &CCricketInput::undo_button_clicked_slot);
  connect(mDartBoard, &CDartBoardCricket::signal_segment_pressed, this, &CCricketInput::segment_pressed_slot);
}

CCricketInput::~CCricketInput()
{
  delete mDartBoard;
  delete mUi;
}

void CCricketInput::set_score(uint32_t iVal, QChar iType)
{
  if (iType == 't')
  {
    iVal = iVal / 3;
  }
  else if (iType == 'd')
  {
    iVal = iVal / 2;
  }
  if (iVal > 0)
  {
    if (mCounter == 3)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart1->setText(temp);
      mDarts[0] = iType + QString::number(iVal);
    }
    else if (mCounter == 2)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart2->setText(temp);
      mDarts[1] = iType + QString::number(iVal);
    }
    else if (mCounter == 1)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart3->setText(temp);
      mDarts[2] = iType + QString::number(iVal);
    }
  }
  else
  {
    if (mCounter == 3)
    {
      mUi->labelScoreDart1->setText("X");
      mDarts[0] = iType + QString::number(iVal);
    }
    else if (mCounter == 2)
    {
      mUi->labelScoreDart2->setText("X");
      mDarts[1] = iType + QString::number(iVal);
    }
    else if (mCounter == 1)
    {
      mUi->labelScoreDart3->setText("X");
      mDarts[2] = iType + QString::number(iVal);
    }
  }
}

void CCricketInput::compute_score()
{
  mScore = mExtra15 + mExtra16 + mExtra17 + mExtra18 + mExtra19 + mExtra20 + mExtra25;
}

bool CCricketInput::are_slots_full() const
{
  bool full15 = mSlot15 == 3;
  bool full16 = mSlot16 == 3;
  bool full17 = mSlot17 == 3;
  bool full18 = mSlot18 == 3;
  bool full19 = mSlot19 == 3;
  bool full20 = mSlot20 == 3;
  bool full25 = mSlot25 == 3;

  return full15 && full16 && full17 && full18 && full19 && full20 && full25;
}

void CCricketInput::segment_pressed_slot(uint32_t iVal, QChar & iType)
{
  bool result = true;

  if (!mpSettings.mCutThroat)
  {
    if (!mStop)
    {
      if (mCounter > 0)
      {
        set_score(iVal, iType);
        uint32_t hits;
        if (iType == 't')
        {
            hits = 3;
        }
        else if (iType == 'd')
        {
            hits = 2;
        }
        else
        {
            hits = 1;
        }
        uint32_t val = iVal;
        val /= hits;
        switch (val)
        {
        case 15:
          if (mSlot15 < 3)
          {
            if (mSlot15 + hits <= 3)
            {
              mSlot15 += hits;
            }
            else
            {
              hits -= 3 - mSlot15;
              mSlot15 = 3;
              if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
              {
                mExtra15 += hits * 15;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
            {
                mExtra15 += hits * 15;
            }
          }
          break;
        case 16:
          if (mSlot16 < 3)
          {
            if (mSlot16 + hits <= 3)
            {
              mSlot16 += hits;
            }
            else
            {
              hits -= 3 - mSlot16;
              mSlot16 = 3;
              if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
              {
                mExtra16 += hits * 16;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
            {
              mExtra16 += hits * 16;
            }
          }
          break;
        case 17:
          if (mSlot17 < 3)
          {
            if (mSlot17 + hits <= 3)
            {
              mSlot17 += hits;
            }
            else
            {
              hits -= 3 - mSlot17;
              mSlot17 = 3;
              if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
              {
                mExtra17 += hits * 17;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
            {
              mExtra17 += hits * 17;
            }
          }
          break;
        case 18:
          if (mSlot18 < 3)
          {
            if (mSlot18 + hits <= 3)
            {
              mSlot18 += hits;
            }
            else
            {
              hits -= 3 - mSlot18;
              mSlot18 = 3;
              if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
              {
                mExtra18 += hits * 18;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
            {
              mExtra18 += hits * 18;
            }
          }
          break;
        case 19:
          if (mSlot19 < 3)
          {
            if (mSlot19 + hits <= 3)
            {
              mSlot19 += hits;
            }
            else
            {
              hits -= 3 - mSlot19;
              mSlot19 = 3;
              if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
              {
                mExtra19 += hits * 19;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
            {
              mExtra19 += hits * 19;
            }
          }
          break;
        case 20:
          if (mSlot20 < 3)
          {
            if (mSlot20 + hits <= 3)
            {
              mSlot20 += hits;
            }
            else
            {
              hits -= 3 - mSlot20;
              mSlot20 = 3;
              if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
              {
                mExtra20 += hits * 20;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
            {
              mExtra20 += hits * 20;
            }
          }
          break;
        case 25:
          if (mSlot25 < 3)
          {
            if (mSlot25 + hits <= 3)
            {
              mSlot25 += hits;
            }
            else
            {
              hits -= 3 - mSlot25;
              mSlot25 = 3;
              if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
              {
                mExtra25 += hits * 25;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
            {
              mExtra25 += hits * 25;
            }
          }
          break;
          default:;
        }

        compute_score();
        if (are_slots_full() && mGameWindow->is_score_bigger(mScore))
        {
          mStop = true;  // Game shot
          mGameShotSound.play();
          mUi->submitButton->setFocus();
        }

        mCounter--;
        if (mCounter == 0)
        {
          mStop = true;
          mUi->submitButton->setFocus();
        }
      }
    }
    else if (are_slots_full() && mGameWindow->is_score_bigger(mScore))
    {
      QMessageBox::warning(this, "Warning", "You have already won this leg!");
    }
    else
    {
      QMessageBox::warning(this, "Warning", "You only have three darts!");
    }
  }
  else
  {
    if (!mStop)
    {
      if (mCounter > 0)
      {
        set_score(iVal, iType);
        uint32_t hits;
        if (iType == 't')
        {
          hits = 3;
        }
        else if (iType == 'd')
        {
          hits = 2;
        }
        else
        {
          hits = 1;
        }
        uint32_t val = iVal;
        val /= hits;
        switch (val)
        {
        case 15:
          if (mSlot15 < 3)
          {
            if (mSlot15 + hits <= 3)
            {
              mSlot15 += hits;
            }
            else
            {
              hits -= 3 - mSlot15;
              mSlot15 = 3;
              if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
              {
                mExtra15 += hits * 15;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
            {
              mExtra15 += hits * 15;
            }
          }
          break;
        case 16:
          if (mSlot16 < 3)
          {
            if (mSlot16 + hits <= 3)
            {
              mSlot16 += hits;
            }
            else
            {
              hits -= 3 - mSlot16;
              mSlot16 = 3;
              if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
              {
                mExtra16 += hits * 16;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
            {
              mExtra16 += hits * 16;
            }
          }
          break;
        case 17:
          if (mSlot17 < 3)
          {
            if (mSlot17 + hits <= 3)
            {
              mSlot17 += hits;
            }
            else
            {
              hits -= 3 - mSlot17;
              mSlot17 = 3;
              if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
              {
                mExtra17 += hits * 17;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
            {
              mExtra17 += hits * 17;
            }
          }
          break;
        case 18:
          if (mSlot18 < 3)
          {
            if (mSlot18 + hits <= 3)
            {
              mSlot18 += hits;
            }
            else
            {
              hits -= 3 - mSlot18;
              mSlot18 = 3;
              if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
              {
                mExtra18 += hits * 18;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
            {
              mExtra18 += hits * 18;
            }
          }
          break;
        case 19:
          if (mSlot19 < 3)
          {
            if (mSlot19 + hits <= 3)
            {
              mSlot19 += hits;
            }
            else
            {
              hits -= 3 - mSlot19;
              mSlot19 = 3;
              if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
              {
                mExtra19 += hits * 19;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
            {
              mExtra19 += hits * 19;
            }
          }
          break;
        case 20:
          if (mSlot20 < 3)
          {
            if (mSlot20 + hits <= 3)
            {
              mSlot20 += hits;
            }
            else
            {
              hits -= 3 - mSlot20;
              mSlot20 = 3;
              if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
              {
                mExtra20 += hits * 20;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
            {
              mExtra20 += hits * 20;
            }
          }
          break;
        case 25:
          if (mSlot25 < 3)
          {
            if (mSlot25 + hits <= 3)
            {
              mSlot25 += hits;
            }
            else
            {
              hits -= 3 - mSlot25;
              mSlot25 = 3;
              if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
              {
                mExtra25 += hits * 25;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
            {
              mExtra25 += hits * 25;
            }
          }
          break;
          default:;
        }

        QVector<uint32_t> temp15s = mGameWindow->compute_extra15s(mExtra15, mPlayer->get_player_number());
        QVector<uint32_t> temp16s = mGameWindow->compute_extra16s(mExtra16, mPlayer->get_player_number());
        QVector<uint32_t> temp17s = mGameWindow->compute_extra17s(mExtra17, mPlayer->get_player_number());
        QVector<uint32_t> temp18s = mGameWindow->compute_extra18s(mExtra18, mPlayer->get_player_number());
        QVector<uint32_t> temp19s = mGameWindow->compute_extra19s(mExtra19, mPlayer->get_player_number());
        QVector<uint32_t> temp20s = mGameWindow->compute_extra20s(mExtra20, mPlayer->get_player_number());
        QVector<uint32_t> temp25s = mGameWindow->compute_extra25s(mExtra25, mPlayer->get_player_number());
        QVector<uint32_t> scores(temp15s.size());

        for (uint32_t i = 0; i < mExtra15s.size(); i++)
        {
          mExtra15s[i] += temp15s[i];
          mExtra16s[i] += temp16s[i];
          mExtra17s[i] += temp17s[i];
          mExtra18s[i] += temp18s[i];
          mExtra19s[i] += temp19s[i];
          mExtra20s[i] += temp20s[i];
          mExtra25s[i] += temp25s[i];
          scores[i] = temp15s[i] + temp16s[i] + temp17s[i] + temp18s[i] + temp19s[i] + temp20s[i] + temp25s[i];
        }

        mExtra15sArray.push_back(mExtra15s);
        mExtra16sArray.push_back(mExtra16s);
        mExtra17sArray.push_back(mExtra17s);
        mExtra18sArray.push_back(mExtra18s);
        mExtra19sArray.push_back(mExtra19s);
        mExtra20sArray.push_back(mExtra20s);
        mExtra25sArray.push_back(mExtra25s);

        for (uint32_t i = 0; i < scores.size(); i++)
        {
          result = result && scores[i] >= mScore;
        }

        if (are_slots_full() && result)
        {
          mStop = true;  // Game shot
          mGameShotSound.play();
          mUi->submitButton->setFocus();
        }

        mCounter--;
        if (mCounter == 0)
        {
          mStop = true;
          mUi->submitButton->setFocus();
        }
      }
    }
    else if (are_slots_full() && result)
    {
      QMessageBox::warning(this, "Warning", "You have already won this leg!");
    }
    else
    {
      QMessageBox::warning(this, "Warning", "You only have three darts!");
    }
  }

  mSlot15Array.push_back(mSlot15);
  mSlot16Array.push_back(mSlot16);
  mSlot17Array.push_back(mSlot17);
  mSlot18Array.push_back(mSlot18);
  mSlot19Array.push_back(mSlot19);
  mSlot20Array.push_back(mSlot20);
  mSlot25Array.push_back(mSlot25);
  mExtra15Array.push_back(mExtra15);
  mExtra16Array.push_back(mExtra16);
  mExtra17Array.push_back(mExtra17);
  mExtra18Array.push_back(mExtra18);
  mExtra19Array.push_back(mExtra19);
  mExtra20Array.push_back(mExtra20);
  mExtra25Array.push_back(mExtra25);
}

void CCricketInput::submit_button_clicked_slot()
{
  if (mStop)
  {
    uint32_t numberofdarts = 3 - mCounter;
    emit signal_cricket_submit_button_pressed(numberofdarts, mDarts);
  }
  else
  {
    QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
  }
}

void CCricketInput::undo_button_clicked_slot()
{
  if (mCounter < 3)
  {
    mDarts[2-mCounter] = "";
    mSlot15Array.pop_back();
    mSlot16Array.pop_back();
    mSlot17Array.pop_back();
    mSlot18Array.pop_back();
    mSlot19Array.pop_back();
    mSlot20Array.pop_back();
    mSlot25Array.pop_back();
    mExtra15Array.pop_back();
    mExtra16Array.pop_back();
    mExtra17Array.pop_back();
    mExtra18Array.pop_back();
    mExtra19Array.pop_back();
    mExtra20Array.pop_back();
    mExtra25Array.pop_back();
    mSlot15 = mSlot15Array.back();
    mSlot16 = mSlot16Array.back();
    mSlot17 = mSlot17Array.back();
    mSlot18 = mSlot18Array.back();
    mSlot19 = mSlot19Array.back();
    mSlot20 = mSlot20Array.back();
    mSlot25 = mSlot25Array.back();
    mExtra15 = mExtra15Array.back();
    mExtra16 = mExtra16Array.back();
    mExtra17 = mExtra17Array.back();
    mExtra18 = mExtra18Array.back();
    mExtra19 = mExtra19Array.back();
    mExtra20 = mExtra20Array.back();
    mExtra25 = mExtra25Array.back();

    if (mpSettings.mCutThroat)
    {
      mExtra15sArray.pop_back();
      mExtra16sArray.pop_back();
      mExtra17sArray.pop_back();
      mExtra18sArray.pop_back();
      mExtra19sArray.pop_back();
      mExtra20sArray.pop_back();
      mExtra25sArray.pop_back();
      mExtra15s = mExtra15sArray.back();
      mExtra16s = mExtra16sArray.back();
      mExtra17s = mExtra17sArray.back();
      mExtra18s = mExtra18sArray.back();
      mExtra19s = mExtra19sArray.back();
      mExtra20s = mExtra20sArray.back();
      mExtra25s = mExtra25sArray.back();
    }

    if (mCounter == 2)
    {
      mUi->labelScoreDart1->setText("---");
    }
    else if (mCounter == 1)
    {
      mUi->labelScoreDart2->setText("---");
    }
    else if (mCounter == 0)
    {
      mUi->labelScoreDart3->setText("---");
    }
    mCounter++;
    mStop = false;
  }
}
