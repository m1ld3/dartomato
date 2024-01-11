#include "cricketinput.h"
#include "ui_cricketinput.h"
#include <QGraphicsPathItem>
#include <QPushButton>
#include <QLineEdit>
#include <cmath>
#include <QMessageBox>
#include <QVector>
#include "cricketclass.h"
#include "groupbox_cricket.h"
#include "cricketmainwindow.h"

CCricketInput::CCricketInput(QWidget * iParent, const CSettings & ipSettings, CCricketClass * iPlayer, CCricketMainWindow * ipGameWindow)
  : QDialog(iParent)
  , mUi(new Ui::CCricketInput)
  , mPlayer(iPlayer)
  , mpGameWindow(ipGameWindow)
  , mScore(mPlayer->get_score())
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  mUi->submitButton->setAutoDefault(true);
  mpGroupBox = static_cast<CCricketGroupBox*>(iParent);

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray.at(i) = mPlayer->get_slot(static_cast<ECricketSlots>(i));
    mSlotHistory.at(i).push_back(mSlotArray.at(i));
    mExtraPointsArray.at(i) = mPlayer->get_extra_points(static_cast<ECricketSlots>(i));
    mExtraPointsHistory.at(i).push_back(mExtraPointsArray.at(i));
    mCutThroatExtraPointsArray.at(i) = mpGameWindow->compute_extra_points(static_cast<ECricketSlots>(i), 0, mPlayer->get_player_number());
    mCutThroatExtraPointsHistory.at(i).append(mCutThroatExtraPointsArray.at(i));
  }

  mDartBoard = new CDartBoardCricket(mUi->graphicsView_dartboard, mpSettings, this);

  connect(mUi->submitButton, &QPushButton::clicked, this, &CCricketInput::submit_button_clicked_slot);
  connect(mUi->undoButton, &QPushButton::clicked, this, &CCricketInput::undo_button_clicked_slot);
}

CCricketInput::~CCricketInput()
{
  delete mDartBoard;
  delete mUi;
}

void CCricketInput::set_score_labels(uint32_t iVal, QChar iType)
{
  if (iType == 't') iVal = iVal / 3;
  else if (iType == 'd') iVal = iVal / 2;

  mDarts[3-mCounter] = iType + QString::number(iVal);
  QString temp = iType.toUpper()+QString::number(iVal);
  if (mCounter == 3) mUi->labelScoreDart1->setText(iVal > 0 ? temp : "X");
  else if (mCounter == 2) mUi->labelScoreDart2->setText(iVal > 0 ? temp : "X");
  else if (mCounter == 1) mUi->labelScoreDart3->setText(iVal > 0 ? temp : "X");
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

void CCricketInput::handle_segment_pressed_event(uint32_t iVal, QChar & iType)
{
  process_segment_common(iVal, iType);
  if (mpSettings.mCutThroat) process_segment_cutthroat();
  else process_segment_default();
}

void CCricketInput::notify_cricket_submit_button_clicked(uint32_t iNumberOfDarts, QVector<QString> &iDarts)
{
  mpGroupBox->handle_submit_button_clicked(iNumberOfDarts, iDarts);
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

void CCricketInput::handle_input_stop()
{
  mStop = true;
  mUi->submitButton->setFocus();
}

void CCricketInput::handle_warnings(bool iWarningCondition)
{
  const char * warningMsg = iWarningCondition ? "You have already won this leg!" : "You only have three darts!";
  QMessageBox::warning(this, "Warning", warningMsg);
}

void CCricketInput::process_segment_cutthroat()
{
  if (!mStop && mCounter > 0)
  {
    QVector<uint32_t> scores(mpSettings.mNumberOfPlayers - 1);
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

void CCricketInput::compute_cutthroat_scores_for_other_players(QVector<uint32_t> & iScores)
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    QVector<uint32_t> temp = mpGameWindow->compute_extra_points(static_cast<ECricketSlots>(i), mExtraPointsArray.at(i), mPlayer->get_player_number());
    for (uint32_t j = 0; j < temp.size(); j++)
    {
      mCutThroatExtraPointsArray.at(i)[j] += temp.at(j);
      iScores[j] += temp.at(j);
    }
    mCutThroatExtraPointsHistory.at(i).push_back(mCutThroatExtraPointsArray.at(i));
  }
}


void CCricketInput::handle_game_shot()
{
  mStop = true;
  mDartBoard->play_game_shot_sound();
  mUi->submitButton->setFocus();
}

void CCricketInput::increase_extra_points(uint32_t iSlotIdx, uint32_t iSlotVal, uint32_t iHits)
{
  if (mpGameWindow->is_slot_free(static_cast<ECricketSlots>(iSlotIdx), mPlayer->get_player_number()))
  {
    mExtraPointsArray.at(iSlotIdx) += iHits * iSlotVal;
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

void CCricketInput::process_segment_common(uint32_t iVal, QChar & iType)
{
  if (!mStop && mCounter > 0)
  {
    set_score_labels(iVal, iType);
    handle_slots_and_extra_points(iVal, iType);
    save_history();
  }
}

void CCricketInput::process_segment_default()
{
  if (!mStop && mCounter > 0)
  {
    compute_score();
    bool gameShotCondition = are_slots_full() && mpGameWindow->is_score_bigger(mScore);
    if (gameShotCondition) handle_game_shot();
    mCounter--;
    if (mCounter == 0 || gameShotCondition) handle_input_stop();
  }
  else
  {
    handle_warnings(are_slots_full() && mpGameWindow->is_score_bigger(mScore));
  }
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
    uint32_t numberofdarts = 3 - mCounter;
    notify_cricket_submit_button_clicked(numberofdarts, mDarts);
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
    for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
    {
      mSlotHistory.at(i).pop_back();
      mExtraPointsHistory.at(i).pop_back();
      mSlotArray.at(i) = mSlotHistory.at(i).back();
      mExtraPointsArray.at(i) = mExtraPointsHistory.at(i).back();
      if (mpSettings.mCutThroat)
      {
        mCutThroatExtraPointsHistory.at(i).pop_back();
        mCutThroatExtraPointsArray.at(i) = mCutThroatExtraPointsHistory.at(i).back();
      }
    }

    if (mCounter == 2) mUi->labelScoreDart1->setText("---");
    else if (mCounter == 1) mUi->labelScoreDart2->setText("---");
    else if (mCounter == 0) mUi->labelScoreDart3->setText("---");
    mCounter++;
    mStop = false;
  }
}
