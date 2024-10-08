#include "cricket_input.h"
#include "ui_cricket_input.h"
#include <QGraphicsPathItem>
#include <QPushButton>
#include <QLineEdit>
#include <cmath>
#include <QMessageBox>
#include <QVector>
#include "cricket_class.h"
#include "groupbox_cricket.h"
#include "cricket_mainwindow.h"

#ifndef TESTING
#define PUT_WARNING(TITLE, MESSAGE) \
  QMessageBox::warning(this, (TITLE), (MESSAGE));
#else
#include <iostream>
#define PUT_WARNING(TITLE, MESSAGE) \
  mOutput << TITLE << ": " << MESSAGE;
#endif

#ifndef TESTING
CCricketInput::CCricketInput(QWidget * iParent, const CSettings & iSettings, CCricketClass * iPlayer, ICricketMainWindow * iGameWindow)
  : QDialog(iParent)
  , mUi(new Ui::CCricketInput)
  , mPlayer(iPlayer)
  , mGameWindow(iGameWindow)
  , mScore(mPlayer->get_score())
  , mPlayerNumber(mPlayer->get_player_number())
  , mSettings(iSettings)
  , mGroupBox(static_cast<CCricketGroupBox*>(iParent))
{
  mUi->setupUi(this);
  mUi->submitButton->setAutoDefault(true);

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray.at(i) = mPlayer->get_slot(static_cast<ECricketSlots>(i));
    mSlotHistory.at(i).push_back(mSlotArray.at(i));
    mExtraPointsArray.at(i) = mPlayer->get_extra_points(static_cast<ECricketSlots>(i));
    mExtraPointsHistory.at(i).push_back(mExtraPointsArray.at(i));
    mCutThroatExtraPointsArray.at(i) = mGameWindow->compute_extra_points(static_cast<ECricketSlots>(i), 0, mPlayerNumber);
    mCutThroatExtraPointsHistory.at(i).append(mCutThroatExtraPointsArray.at(i));
  }
  mDartBoard = new CDartBoardCricket(mUi->graphicsViewDartBoard, mSettings, this);
  connect(mUi->submitButton, &QPushButton::clicked, this, &CCricketInput::submit_button_clicked_slot);
  connect(mUi->undoButton, &QPushButton::clicked, this, &CCricketInput::undo_button_clicked_slot);
}
#endif

CCricketInput::~CCricketInput()
{
#ifndef TESTING
  delete mDartBoard;
  delete mUi;
#endif
}

void CCricketInput::set_score_labels(uint32_t iVal, QChar iType)
{
  if (iType == 't') iVal /= 3;
  else if (iType == 'd') iVal /= 2;

  mDarts[3 - mCounter] = iType + QString::number(iVal);
  QString temp = iType.toUpper() + QString::number(iVal);
#ifndef TESTING
  if (mCounter == 3) mUi->labelScoreDart1->setText(iVal > 0 ? temp : "X");
  else if (mCounter == 2) mUi->labelScoreDart2->setText(iVal > 0 ? temp : "X");
  else if (mCounter == 1) mUi->labelScoreDart3->setText(iVal > 0 ? temp : "X");
#endif
}

void CCricketInput::compute_score()
{
  mScore = 0;
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mScore += mExtraPointsArray.at(i);
  }
}

bool CCricketInput::are_slots_full() const
{
  bool temp = true;
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    temp = temp && (mSlotArray.at(i) == 3);
  }

  return temp;
}

void CCricketInput::handle_segment_pressed_event(uint32_t iVal, QChar iType)
{
  process_segment_common(iVal, iType);
  if (mSettings.CutThroat) process_segment_cutthroat();
  else process_segment_default();
}

void CCricketInput::process_segment_common(uint32_t iVal, QChar & iType)
{
  if (!mStop && mCounter > 0)
  {
    set_score_labels(iVal, iType);
    handle_slots_and_extra_points(iVal, iType);
    save_history();
  }
}

void CCricketInput::handle_slots_and_extra_points(uint32_t iVal, QChar & iType)
{
  uint32_t hits = (iType == 't') ? 3 : ((iType == 'd') ? 2 : 1);
  uint32_t val = iVal / hits;
  uint32_t idx = static_cast<uint32_t>(Slot2IdxMap[val]);
  if (mSlotArray.at(idx) < 3 && val != 0)
  {
    if (mSlotArray.at(idx) + hits <= 3)
    {
      mSlotArray.at(idx) += hits;
    }
    else
    {
      hits -= 3 - mSlotArray.at(idx);
      mSlotArray.at(idx) = 3;
      increase_extra_points(idx, val, hits);
    }
  }
  else
  {
    increase_extra_points(idx, val, hits);
  }
}

void CCricketInput::increase_extra_points(uint32_t iSlotIdx, uint32_t iSlotVal, uint32_t iHits)
{
  if (mGameWindow->is_slot_free(static_cast<ECricketSlots>(iSlotIdx), mPlayerNumber))
  {
    mExtraPointsArray.at(iSlotIdx) += iHits * iSlotVal;
  }
}

void CCricketInput::process_segment_default()
{
  if (!mStop && mCounter > 0)
  {
    compute_score();
    bool gameShotCondition = are_slots_full() && mGameWindow->is_score_bigger(mScore);
    if (gameShotCondition) handle_game_shot();
    mCounter--;
    if (mCounter == 0 || gameShotCondition) handle_input_stop();
  }
  else
  {
    handle_warnings(are_slots_full() && mGameWindow->is_score_bigger(mScore));
  }
}

void CCricketInput::process_segment_cutthroat()
{
  if (!mStop && mCounter > 0)
  {
    QVector<uint32_t> scores(mSettings.PlayersList.size() - 1);
    compute_cutthroat_scores_for_other_players(scores);
    check_if_game_shot_cutthroat(scores);
    mCounter--;
    if (mCounter == 0) handle_input_stop();
  }
  else
  {
    handle_warnings(are_slots_full());
  }
}

void CCricketInput::check_if_game_shot_cutthroat(QVector<uint32_t> & iScores)
{
  bool result = true;
  for (uint32_t i = 0; i < iScores.size(); i++)
  {
    result = result && iScores[i] >= mScore;
  }

  if (are_slots_full() && result) handle_game_shot();
}

void CCricketInput::compute_cutthroat_scores_for_other_players(QVector<uint32_t> & iScores)
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    QVector<uint32_t> temp = mGameWindow->compute_extra_points(static_cast<ECricketSlots>(i), mExtraPointsArray.at(i), mPlayerNumber);
    for (uint32_t j = 0; j < temp.size(); j++)
    {
      mCutThroatExtraPointsArray.at(i)[j] += temp.at(j);
      iScores[j] += temp.at(j);
    }
    mCutThroatExtraPointsHistory.at(i).push_back(mCutThroatExtraPointsArray.at(i));
  }
}

void CCricketInput::notify_cricket_submit_button_clicked(uint32_t iNumberOfDarts, QVector<QString> &iDarts)
{
  mGroupBox->handle_submit_button_clicked(iNumberOfDarts, iDarts);
}

void CCricketInput::handle_input_stop()
{
  mStop = true;
#ifndef TESTING
  mUi->submitButton->setFocus();
#endif
}

void CCricketInput::handle_warnings(bool iWarningCondition)
{
  const char * warningMsg = iWarningCondition ? "You have already won this leg!" : "You only have three darts!";
  PUT_WARNING("Warning", warningMsg);
}

void CCricketInput::handle_game_shot()
{
  mStop = true;
  mDartBoard->play_game_shot_sound();
#ifndef TESTING
  mUi->submitButton->setFocus();
#endif
}

void CCricketInput::save_history()
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotHistory.at(i).push_back(mSlotArray.at(i));
    mExtraPointsHistory.at(i).push_back(mExtraPointsArray.at(i));
  }
}

void CCricketInput::submit_button_clicked_slot()
{
  if (mStop)
  {
    uint32_t numberOfDarts = 3 - mCounter;
    notify_cricket_submit_button_clicked(numberOfDarts, mDarts);
  }
  else
  {
    PUT_WARNING("Score incomplete", "Please enter all darts.");
  }
}

void CCricketInput::undo_button_clicked_slot()
{
  if (mCounter < 3)
  {
    mDarts[2 - mCounter] = "";
    for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
    {
      mSlotHistory.at(i).pop_back();
      mExtraPointsHistory.at(i).pop_back();
      mSlotArray.at(i) = mSlotHistory.at(i).back();
      mExtraPointsArray.at(i) = mExtraPointsHistory.at(i).back();
      if (mSettings.CutThroat)
      {
        mCutThroatExtraPointsHistory.at(i).pop_back();
        mCutThroatExtraPointsArray.at(i) = mCutThroatExtraPointsHistory.at(i).back();
      }
    }

#ifndef TESTING
    if (mCounter == 2) mUi->labelScoreDart1->setText("---");
    else if (mCounter == 1) mUi->labelScoreDart2->setText("---");
    else if (mCounter == 0) mUi->labelScoreDart3->setText("---");
#endif
    mCounter++;
    mStop = false;
  }
}
