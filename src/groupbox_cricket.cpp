#include "groupbox_cricket.h"
#include "ui_groupbox_cricket.h"
#include "cricket_mainwindow.h"
#include <QMessageBox>
#include <string>
#include <QString>
#include <algorithm>
#include "player_active_button.h"

CCricketGroupBox::CCricketGroupBox(QWidget * iParent,
                                   const CSettings & ipSettings,
                                   uint32_t iPlayerNumber,
                                   CCricketClass * const iPlayer)
  : QGroupBox(iParent)
  , mUi(new Ui::CCricketGroupBox)
  , mPlayer(iPlayer)
  , mPlayerNumber(iPlayerNumber - 1)
  , mpGameWindow(static_cast<CCricketMainWindow*>(iParent))
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(4);
  mUi->lcdNumber->display(static_cast<int>(mScore));

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    set_extra_points_label(static_cast<ECricketSlots>(i), mPlayer->get_extra_points(static_cast<ECricketSlots>(i)));
  }

  mUi->labelPlayerName->setText(mpSettings.mPlayersList.at(iPlayerNumber - 1));
  QString hitsPerRound = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->labelHitsPerRoundInput->setText(hitsPerRound);
  connect_slots();
}

CCricketGroupBox::~CCricketGroupBox()
{
  delete mUi;
}

void CCricketGroupBox::connect_slots()
{
  connect(mUi->labelPic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CCricketGroupBox::player_active_button_pressed_slot);
  connect(mUi->pushButtonScore, &QPushButton::clicked, this, &CCricketGroupBox::push_button_score_clicked_slot);
  connect(mUi->pushButtonUndo, &QPushButton::clicked, this, &CCricketGroupBox::push_button_undo_clicked_slot);
}

void CCricketGroupBox::set_active()
{
  mActive = true;
  uint32_t w = 80;
  uint32_t h = 80;
  mUi->labelPic->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
}

void CCricketGroupBox::set_inactive()
{
  mActive = false;
  mUi->labelPic->clear();
}

void CCricketGroupBox::set_finished()
{
  mFinished = true;
}

void CCricketGroupBox::unset_finished()
{
  mFinished = false;
}

void CCricketGroupBox::close_cricket_input()
{
  this->mScoreInput->close();
}

QString CCricketGroupBox::get_player_number() const
{
  return mUi->labelPlayerName->text();
}

void CCricketGroupBox::ok_button_clicked_slot(const QString & iName)
{
  mPlayerName = iName;
  mUi->labelPlayerName->setText(mPlayerName);
}

void CCricketGroupBox::load_slot_arrays_from_player()
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray.at(i) = mPlayer->get_slot(static_cast<ECricketSlots>(i));
    mExtraPointsArray.at(i) = mPlayer->get_extra_points(static_cast<ECricketSlots>(i));
  }
}

void CCricketGroupBox::write_slot_arrays_to_player(const std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & iExtraPointsCutThroat)
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mPlayer->set_slot(static_cast<ECricketSlots>(i), mSlotArray.at(i));
    if (mpSettings.mCutThroat)
    {
      mpGameWindow->increase_slot_score(static_cast<ECricketSlots>(i), iExtraPointsCutThroat.at(i));
    }
    else
    {
      mPlayer->set_extra_points(static_cast<ECricketSlots>(i), mExtraPointsArray.at(i));
    }
  }
}

void CCricketGroupBox::handle_leg_won()
{
  bool newSet = false;
  newSet = mPlayer->increase_setslegs();
  mPlayer->set_leg_win_array(true);
  update_history_of_all_players();
  reset_scores_of_all_players();
  if (mActive && !newSet)
  {
    update_players(EUpdateType::LEG);
    CCricketGroupBox::mLegStarted = false;
  }
  else if (mActive && newSet)
  {
    update_players(EUpdateType::SET);
    CCricketGroupBox::mSetStarted = false;
    CCricketGroupBox::mLegStarted = false;
  }
  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumberSets->display(static_cast<int>(mPlayer->get_sets()));
}

void CCricketGroupBox::handle_switch_to_next_player()
{
  if (mActive)
  {
    update_players(EUpdateType::DEFAULT);
  }
  mPlayer->set_leg_win_array(false);
  mpGameWindow->update_extra_points_labels();
}

void CCricketGroupBox::calculate_extra_points(uint32_t iSlotIdx, uint32_t iHits, uint32_t iSlotVal,
                                              std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat)
{
  if (mpSettings.mCutThroat)
  {
    oExtraPointsCutThroat.at(iSlotIdx) += iHits * iSlotVal;
  }
  else
  {
    mExtraPointsArray.at(iSlotIdx) += iHits * iSlotVal;
    set_extra_points_label(static_cast<ECricketSlots>(iSlotIdx), mExtraPointsArray.at(iSlotIdx));
  }
}

void CCricketGroupBox::handle_slot_hits_overflow(uint32_t iSlotIdx, uint32_t iHits, uint32_t iSlotVal, QString & oDart,
                                                 std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat)
{
  uint32_t remainingSlots = 3 - mSlotArray.at(iSlotIdx);
  uint32_t hitsNew = iHits - remainingSlots;
  mSlotArray.at(iSlotIdx) = 3;
  set_slot_label(static_cast<ECricketSlots>(iSlotIdx), mSlotArray.at(iSlotIdx));

  if (mpGameWindow->is_slot_free(static_cast<ECricketSlots>(iSlotIdx), mPlayerNumber))
  {
    calculate_extra_points(iSlotIdx, hitsNew, iSlotVal, oExtraPointsCutThroat);
    mTotalHits += iHits;
  }
  else
  {
    // fill up only the own slot without scoring extra points
    std::array<QString, 2> prefixes {"S", "D"};
    oDart = remainingSlots == 0 ? " X" : (prefixes.at(remainingSlots-1) + QString::number(iSlotVal));
    mTotalHits += remainingSlots;
  }
}

void CCricketGroupBox::fill_slot_hits(uint32_t iSlotIdx, uint32_t iHits)
{
  mSlotArray.at(iSlotIdx) += iHits;
  set_slot_label(static_cast<ECricketSlots>(iSlotIdx), mSlotArray.at(iSlotIdx));
  mTotalHits += iHits;
}

void CCricketGroupBox::process_single_dart(uint32_t iDartIdx, QVector<QString> & oDarts,
                                           std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat)
{
  QString dart = oDarts[iDartIdx].isEmpty() ? "0" : oDarts[iDartIdx];
  QChar type = dart[0];
  uint32_t hits = (type == 't') ? 3 : ((type == 'd') ? 2 : 1);
  uint32_t val = dart.mid(1).toUInt();
  uint32_t idx = static_cast<uint32_t>(Slot2IdxMap[val]);

  if (val == 0) return;

  if (mSlotArray.at(idx) + hits <= 3)
  {
    fill_slot_hits(idx, hits);
  }
  else if (mSlotArray.at(idx) + hits > 3)
  {
    handle_slot_hits_overflow(idx, hits, val, oDarts[iDartIdx], oExtraPointsCutThroat);
  }
}

bool CCricketGroupBox::has_leg_won()
{
  return mScoreInput->are_slots_full()
         && ((mpGameWindow->is_score_bigger(mScore) && !mpSettings.mCutThroat) || (mpGameWindow->is_score_smaller(mScore) && mpSettings.mCutThroat));
}

void CCricketGroupBox::submit_score_to_player(uint32_t iNumberOfDarts, const QVector<QString> & iDarts, const std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & iExtraPointsCutThroat)
{
  write_slot_arrays_to_player(iExtraPointsCutThroat);
  if (mpSettings.mCutThroat) mpGameWindow->set_scores();
  else mPlayer->set_score();
  mScore = mPlayer->get_score();
  mPlayer->compute_hits_per_round(iNumberOfDarts, mTotalHits);
  mPlayer->update_darts(iDarts);
  QString hpr = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->labelHitsPerRoundInput->setText(hpr);
}

void CCricketGroupBox::handle_submit_button_clicked(uint32_t iNumberOfDarts, QVector<QString> & iDarts)
{
  CCricketGroupBox::mLegStarted = true;
  CCricketGroupBox::mSetStarted = true;
  std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> extraPointsCutThroat = {0, 0, 0, 0, 0, 0, 0};

  load_slot_arrays_from_player();

  for (uint32_t i = 0; i < iDarts.size(); i++)
  {
    process_single_dart(i, iDarts, extraPointsCutThroat);
  }

  submit_score_to_player(iNumberOfDarts, iDarts, extraPointsCutThroat);
  if (mpSettings.mCutThroat) mpGameWindow->update_darts(mPlayer->get_player_number());

  if (has_leg_won()) handle_leg_won();
  else               handle_switch_to_next_player();

  set_leg_history();
  close_cricket_input();
}


void CCricketGroupBox::player_active_button_pressed_slot()
{
  if (!mActive)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                          QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      inactivate_players(mPlayer->get_player_number(), CCricketGroupBox::mLegStarted, CCricketGroupBox::mSetStarted);
      set_active();
    }
  }
}

void CCricketGroupBox::push_button_score_clicked_slot()
{
  if (mActive && !mFinished)
  {
    mScoreInput = new CCricketInput(this, mpSettings, mPlayer, mpGameWindow);
    mScoreInput->setAttribute(Qt::WA_DeleteOnClose);
    mScoreInput->show();
  }
  else if (mFinished)
  {
    QMessageBox::about(this, "Warning", "Game already finished!");
  }
  else
  {
    QMessageBox::about(this, "Warning", "It's not your turn!");
  }
}

void CCricketGroupBox::set_set_begin()
{
  mSetBegin = true;
}

void CCricketGroupBox::unset_set_begin()
{
  mSetBegin = false;
}

void CCricketGroupBox::set_leg_begin()
{
  mLegBegin = true;
}

void CCricketGroupBox::unset_leg_begin()
{
  mLegBegin = false;
}

bool CCricketGroupBox::has_begun_leg() const
{
  return mLegBegin;
}

bool CCricketGroupBox::has_begun_set() const
{
  return mSetBegin;
}

void CCricketGroupBox::update_history()
{
  mPlayer->update_history();
}

void CCricketGroupBox::reset_legs()
{
  mPlayer->reset_legs();
}

void CCricketGroupBox::reset()
{
  mScore = 0;
  mPlayer->reset_score();
  mUi->lcdNumber->display(static_cast<int>(mScore));
  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumberSets->display(static_cast<int>(mPlayer->get_sets()));

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    set_slot_label(static_cast<ECricketSlots>(i), 0);
    set_extra_points_label(static_cast<ECricketSlots>(i), 0);
  }
}

void CCricketGroupBox::set_leg_started()
{
  mLegStarted = true;
}

void CCricketGroupBox::set_set_started()
{
  mSetStarted = true;
}

void CCricketGroupBox::unset_leg_started()
{
  mLegStarted = false;
}

void CCricketGroupBox::unset_set_started()
{
  mSetStarted = false;
}

uint32_t CCricketGroupBox::get_slot(const ECricketSlots iSlot) const
{
  return mPlayer->get_slot(iSlot);
}

void CCricketGroupBox::set_slot(const ECricketSlots iSlot, uint32_t iHits)
{
  mPlayer->set_slot(iSlot, iHits);
}

void CCricketGroupBox::set_extra_points(const ECricketSlots iSlot, uint32_t iPoints)
{
  mPlayer->set_extra_points(iSlot, iPoints);
}

uint32_t CCricketGroupBox::get_extra_points(const ECricketSlots iSlot) const
{
  return mPlayer->get_extra_points(iSlot);
}

void CCricketGroupBox::set_extra_points_label(const ECricketSlots iSlot, uint32_t iPoints)
{
  switch (iSlot)
  {
  case ECricketSlots::SLOT_15:
    mUi->labelExtra15->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_16:
    mUi->labelExtra16->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_17:
    mUi->labelExtra17->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_18:
    mUi->labelExtra18->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_19:
    mUi->labelExtra19->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_20:
    mUi->labelExtra20->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_25:
    mUi->labelExtra25->setNum(static_cast<int>(iPoints));
    break;
  default:;
  }
}

void CCricketGroupBox::set_slot_label(const ECricketSlots iSlot, uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;

  for (uint32_t i = 0; i <= iHits; i++)
  {
    switch (iSlot)
    {
    case ECricketSlots::SLOT_15:
      if (i == 0)
      {
        mUi->label15Slot1->clear(); mUi->label15Slot2->clear(); mUi->label15Slot3->clear();
      }
      else if (i == 1) mUi->label15Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label15Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label15Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_16:
      if (i == 0)
      {
        mUi->label16Slot1->clear(); mUi->label16Slot2->clear(); mUi->label16Slot3->clear();
      }
      else if (i == 1) mUi->label16Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label16Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label16Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_17:
      if (i == 0)
      {
        mUi->label17Slot1->clear(); mUi->label17Slot2->clear(); mUi->label17Slot3->clear();
      }
      else if (i == 1) mUi->label17Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label17Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label17Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_18:
      if (i == 0)
      {
        mUi->label18Slot1->clear(); mUi->label18Slot2->clear(); mUi->label18Slot3->clear();
      }
      else if (i == 1) mUi->label18Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label18Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label18Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_19:
      if (i == 0)
      {
        mUi->label19Slot1->clear(); mUi->label19Slot2->clear(); mUi->label19Slot3->clear();
      }
      else if (i == 1) mUi->label19Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label19Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label19Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_20:
      if (i == 0)
      {
        mUi->label20Slot1->clear(); mUi->label20Slot2->clear(); mUi->label20Slot3->clear();
      }
      else if (i == 1) mUi->label20Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label20Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label20Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_25:
      if (i == 0)
      {
        mUi->label25Slot1->clear(); mUi->label25Slot2->clear(); mUi->label25Slot3->clear();
      }
      else if (i == 1) mUi->label25Slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label25Slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label25Slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    default:;
    }
  }
}

uint32_t CCricketGroupBox::get_score() const
{
  return mPlayer->get_score();
}

void CCricketGroupBox::filter_leg_scores_cutthroat(QVector<QVector<QString>> & oLegScores)
{
  if (mpSettings.mCutThroat)
  {
    QVector<QVector<QString>> filteredScores = {};
    for (uint32_t i = 0; i < oLegScores.size(); i++)
    {
      if (oLegScores[i].last() != "")
      {
        filteredScores.push_back(oLegScores[i]);
      }
    }
    oLegScores = filteredScores;
  }
}

void CCricketGroupBox::display_leg_scores(const QVector<QVector<QString>> & iLegScores)
{
  mUi->textBrowser->clear();

  for (uint32_t i = 0; i < iLegScores.size(); i++)
  {
    display_leg_score_line(i, iLegScores[i]);
  }
}

QString CCricketGroupBox::format_leg_score(const QVector<QString> & iLegScore)
{
  QString formattedScore;
  for (const auto & scorePart : iLegScore)
  {
    QString temp = scorePart;
    if (temp.size() > 1)
    {
      if (temp[1] == '0') temp = " X";
      else                temp[0] = temp[0].toUpper();
    }
    formattedScore += temp + "  ";
  }
  formattedScore = formattedScore.trimmed();

  return formattedScore;
}

void CCricketGroupBox::display_leg_score_line(uint32_t iLegNumber, const QVector<QString> & iLegScore)
{
  QString line = QString::number(iLegNumber + 1) + ": " + format_leg_score(iLegScore);
  mUi->textBrowser->append(line);
}

void CCricketGroupBox::set_leg_history()
{
  QVector<QVector<QString>> legscores = mPlayer->get_score_legs();
  QVector<QVector<QVector<QString>>> totalscores = mPlayer->get_scoring_history();
  if (legscores.size() == 0 && totalscores.size() > 0)
  {
    legscores = totalscores.last();
  }

  filter_leg_scores_cutthroat(legscores);
  display_leg_scores(legscores);
}

void CCricketGroupBox::push_button_undo_clicked_slot()
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Undo",
                                                             tr("Are you sure you want to undo your last score?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                             QMessageBox::No);
  if (resBtn == QMessageBox::Yes)
  {
    perform_undo();
  }
}

void CCricketGroupBox::perform_undo()
{
  mPlayer->undo();
  mScore = mPlayer->get_score();
  mUi->lcdNumber->display(static_cast<int>(mScore));
  mTotalHits = mPlayer->get_total_hits();

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    set_slot_label(static_cast<ECricketSlots>(i), get_slot(static_cast<ECricketSlots>(i)));
    set_extra_points_label(static_cast<ECricketSlots>(i), get_extra_points(static_cast<ECricketSlots>(i)));
  }

  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumberSets->display(static_cast<int>(mPlayer->get_sets()));
  QString hpr = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->labelHitsPerRoundInput->setText(hpr);
  set_leg_history();

  if (mFinished)
  {
    unset_finished();
    }
}

void CCricketGroupBox::update_players(const EUpdateType iType)
{
  mpGameWindow->update_players(iType);
}

void CCricketGroupBox::reset_scores_of_all_players()
{
  mpGameWindow->reset_scores_of_all_players();
}

void CCricketGroupBox::inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted)
{
  mpGameWindow->inactivate_players(iPlayer, iLegStarted, iSetStarted);
}

void CCricketGroupBox::update_history_of_all_players()
{
  mpGameWindow->update_history_of_all_players();
}

void CCricketGroupBox::increase_extra_points(const ECricketSlots iSlot, uint32_t iPoints)
{
  mPlayer->set_extra_points(iSlot, iPoints);
}

void CCricketGroupBox::set_score()
{
  mPlayer->set_score();
}

void CCricketGroupBox::update_extra_points_labels()
{
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    set_extra_points_label(static_cast<ECricketSlots>(i), mPlayer->get_extra_points(static_cast<ECricketSlots>(i)));
  }
  mUi->lcdNumber->display(static_cast<int>(mPlayer->get_score()));
}

void CCricketGroupBox::update_darts(QVector<QString> && iDarts)
{
  mPlayer->update_darts(iDarts);

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mPlayer->set_slot(static_cast<ECricketSlots>(i), mPlayer->get_slot(static_cast<ECricketSlots>(i)));
  }

  mPlayer->set_total_hits(mPlayer->get_total_hits());
  mPlayer->set_total_darts(mPlayer->get_total_darts());
  mPlayer->set_leg_win_array(false);
}

void CCricketGroupBox::set_lcd_legs()
{
  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer->get_legs()));
}

bool CCricketGroupBox::mLegStarted = false;
bool CCricketGroupBox::mSetStarted = false;