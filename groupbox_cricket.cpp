#include "groupbox_cricket.h"
#include "ui_groupbox_cricket.h"
#include <QMessageBox>
#include "dialognameinput.h"
#include <QDebug>
#include <QSoundEffect>
#include <string>
#include <QString>
#include <algorithm>

CCricketGroupBox::CCricketGroupBox(QWidget * iParent, const CSettings & ipSettings,
                                   uint32_t iPlayerNumber,
                                   CCricketClass * iPlayer)
  : QGroupBox(iParent)
  , mUi(new Ui::CCricketGroupBox)
  , mPlayer(iPlayer)
  , mPlayerNumber(iPlayerNumber - 1)
  , mSound1(this)
  , mSound2(this)
  , mSound3(this)
  , mSound4(this)
  , mSound5(this)
  , mSound6(this)
  , mSound7(this)
  , mSound8(this)
  , mSound9(this)
  , mSound10(this)
  , mSound11(this)
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(4);
  mUi->lcdNumber->display(static_cast<int>(mScore));
  mUi->lcdNumber->setPalette(Qt::darkBlue);

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    set_extra_points_label(static_cast<ECricketSlots>(i), mPlayer->get_extra_points(static_cast<ECricketSlots>(i)));
  }

  QString text = "Player " + QString::number(mPlayerNumber);
  mUi->label_playername->setText(text);
  QString hitsPerRound = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->label_hitsPerRound->setText(hitsPerRound);
  mGameWindow = dynamic_cast<CCricketMainWindow*>(iParent);
  mSound1.setSource(QUrl("qrc:/resources/sounds/yousuck1.wav"));
  mSound2.setSource(QUrl("qrc:/resources/sounds/yousuck2.wav"));
  mSound3.setSource(QUrl("qrc:/resources/sounds/yousuck3.wav"));
  mSound4.setSource(QUrl("qrc:/resources/sounds/youfuckedup.wav"));
  mSound5.setSource(QUrl("qrc:/resources/sounds/poorjob.wav"));
  mSound6.setSource(QUrl("qrc:/resources/sounds/nichtgut.wav"));
  mSound7.setSource(QUrl("qrc:/resources/sounds/newwaytosuck.wav"));
  mSound8.setSource(QUrl("qrc:/resources/sounds/loser.wav"));
  mSound9.setSource(QUrl("qrc:/resources/sounds/littlegirl.wav"));
  mSound10.setSource(QUrl("qrc:/resources/sounds/gutschlecht.wav"));
  mSound11.setSource(QUrl("qrc:/resources/sounds/daswarscheisse.wav"));

  connect(mUi->label_pic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CCricketGroupBox::player_active_button_pressed_slot);
  connect(mUi->pushButton_name, &QPushButton::clicked, this, &CCricketGroupBox::push_button_name_clicked_slot);
  connect(mUi->pushButton_score, &QPushButton::clicked, this, &CCricketGroupBox::push_button_score_clicked_slot);
  connect(mUi->pushButton_undo, &QPushButton::clicked, this, &CCricketGroupBox::push_button_undo_clicked_slot);
}

CCricketGroupBox::~CCricketGroupBox()
{
  delete mUi;
}

void CCricketGroupBox::set_active()
{
  mActive = true;
  uint32_t w = 80;
  uint32_t h = 80;
  mUi->label_pic->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
}

void CCricketGroupBox::set_inactive()
{
  mActive = false;
  mUi->label_pic->clear();
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
  return mUi->label_playername->text();
}

void CCricketGroupBox::push_button_name_clicked_slot()
{
  QPointer<CDialogNameInput> dn = new CDialogNameInput(this, mUi->label_playername->text());
  connect(dn, &CDialogNameInput::signal_ok_button_clicked, this, &CCricketGroupBox::ok_button_clicked_slot);
  dn->show();
}

void CCricketGroupBox::ok_button_clicked_slot(const QString & iName)
{
  mPlayerName = iName;
  mUi->label_playername->setText(mPlayerName);
}

void CCricketGroupBox::cricket_submit_button_pressed_slot(uint32_t iNumberOfDarts, const QVector<QString> & iDarts)
{
  CCricketGroupBox::mLegStarted = true;
  CCricketGroupBox::mSetStarted = true;
  bool newset = false;
  uint32_t hits = 0;
  uint32_t hits_old = 0;

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray.at(i) = mPlayer->get_slot(static_cast<ECricketSlots>(i));
    mExtraPointsArray.at(i) = mPlayer->get_extra_points(static_cast<ECricketSlots>(i));
  }

  uint32_t newhits = mTotalHits;

  if (!mpSettings.mCutThroat)
  {
    QString dart = "0";
    for (uint32_t i = 0; i < iDarts.size(); i++)
    {
      if (iDarts[i].size() > 0)
      {
          dart = iDarts[i];
      }
      QChar type = dart[0];
      if (type == 't')
      {
          hits = 3;
      }
      else if (type == 'd')
      {
          hits = 2;
      }
      else
      {
          hits = 1;
      }
      QString temp = dart.remove(0,1);
      uint32_t val = temp.toInt();
      uint32_t idx = static_cast<uint32_t>(Slot2IdxMap[val]);

      if (mSlotArray.at(idx) < 3 && val != 0)
      {
        if (mSlotArray.at(idx) + hits <= 3)
        {
          mSlotArray.at(idx) += hits;
          set_slot_label(static_cast<ECricketSlots>(idx), mSlotArray.at(idx));
          mTotalHits += hits;
        }
        else
        {
          hits_old = hits;
          hits -= 3 - mSlotArray.at(idx);
          mSlotArray.at(idx) = 3;
          set_slot_label(static_cast<ECricketSlots>(idx), mSlotArray.at(idx));
          if (mGameWindow->is_slot_free(static_cast<ECricketSlots>(idx), mPlayerNumber))
          {
            mExtraPointsArray.at(idx) += hits * val;
            set_extra_points_label(static_cast<ECricketSlots>(idx), mExtraPointsArray.at(idx));
            mTotalHits += hits_old;
          }
          else
          {
            mTotalHits += hits;
          }
        }
      }
      else
      {
        if (mGameWindow->is_slot_free(static_cast<ECricketSlots>(idx), mPlayerNumber))
        {
          mExtraPointsArray.at(idx) += hits * val;
          set_extra_points_label(static_cast<ECricketSlots>(idx), mExtraPointsArray.at(idx));
          mTotalHits += hits;
        }
      }
    }

    for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
    {
      mPlayer->set_slot(static_cast<ECricketSlots>(i), mSlotArray.at(i));
      mPlayer->set_extra_points(static_cast<ECricketSlots>(i), mExtraPointsArray.at(i));
    }

    mPlayer->set_score();
    mScore = mPlayer->get_score();
  }
  else
  {
    std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> extraPoints = {0, 0, 0, 0, 0, 0, 0};
    QString dart = "0";

    for (uint32_t i = 0; i < iDarts.size(); i++)
    {
      if (iDarts[i].size() > 0)
      {
        dart = iDarts[i];
      }
      QChar type = dart[0];
      if (type == 't')
      {
        hits = 3;
      }
      else if (type == 'd')
      {
        hits = 2;
      }
      else
      {
        hits = 1;
      }
      QString temp = dart.remove(0,1);
      uint32_t val = temp.toInt();
      uint32_t idx = static_cast<uint32_t>(Slot2IdxMap[val]);

      if (mSlotArray.at(idx) < 3 && val != 0)
      {
        if (mSlotArray.at(idx) + hits <= 3)
        {
          mSlotArray.at(idx) += hits;
          set_slot_label(static_cast<ECricketSlots>(idx), mSlotArray.at(idx));
          mTotalHits += hits;
        }
        else
        {
          hits_old = hits;
          hits -= 3 - mSlotArray.at(idx);
          mSlotArray.at(idx) = 3;
          set_slot_label(static_cast<ECricketSlots>(idx), mSlotArray.at(idx));
          if (mGameWindow->is_slot_free(static_cast<ECricketSlots>(idx), mPlayerNumber))
          {
            extraPoints.at(idx) += hits * val;
            mTotalHits += hits_old;
          }
          else
          {
            mTotalHits += hits;
          }
        }
      }
      else
      {
        if (mGameWindow->is_slot_free(static_cast<ECricketSlots>(idx), mPlayerNumber))
        {
          extraPoints.at(idx) += hits * val;
          mTotalHits += hits;
        }
      }


    }

    for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
    {
      mPlayer->set_slot(static_cast<ECricketSlots>(i), mSlotArray.at(i));
      mGameWindow->increase_slot_score(static_cast<ECricketSlots>(i), extraPoints.at(i));
    }

    mGameWindow->set_scores();
    mScore = mPlayer->get_score();
  }

  if (mTotalHits == newhits && mpSettings.mOffensive)
  {
    play_fail_sounds();
  }

  mPlayer->compute_hits_per_round(iNumberOfDarts, mTotalHits);
  mPlayer->update_darts(iDarts);
  QString hpr = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->label_hitsPerRound->setText(hpr);

  if (!mpSettings.mCutThroat)
  {
    if (mScoreInput->are_slots_full() && mGameWindow->is_score_bigger(mScore))
    {  // leg won
      newset = mPlayer->increase_setslegs();
      mPlayer->set_leg_win_array(true);
      emit signal_update_history();
      emit signal_reset_scores();
      if (mActive && !newset)
      {
        emit signal_update_player(EUpdateType::LEG);
        CCricketGroupBox::mLegStarted = false;
      }
      else if (mActive && newset)
      {
        emit signal_update_player(EUpdateType::SET);
        CCricketGroupBox::mSetStarted = false;
        CCricketGroupBox::mLegStarted = false;
      }
      mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
      mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
    }
    else
    {
      if (mActive)
      {
        emit signal_update_player(EUpdateType::DEFAULT);
      }
      mPlayer->set_leg_win_array(false);
      mGameWindow->update_extra_points_labels();
    }
  }
  else
  {
    mGameWindow->update_darts(mPlayer->get_player_number());
    if (mScoreInput->are_slots_full() && mGameWindow->is_score_smaller(mScore))
    {  // leg won
      newset = mPlayer->increase_setslegs();
      mPlayer->set_leg_win_array(true);
      emit signal_update_history();
      emit signal_reset_scores();
      if (mActive && !newset)
      {
        emit signal_update_player(EUpdateType::LEG);
        CCricketGroupBox::mLegStarted = false;
      }
      else if (mActive && newset)
      {
        emit signal_update_player(EUpdateType::SET);
        CCricketGroupBox::mSetStarted = false;
        CCricketGroupBox::mLegStarted = false;
      }
      mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
      mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
    }
    else
    {
      if (mActive)
      {
        emit signal_update_player(EUpdateType::DEFAULT);
      }
      mPlayer->set_leg_win_array(false);
      mGameWindow->update_extra_points_labels();
    }
  }
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
      emit signal_inactivate_players(mPlayer->get_player_number(), CCricketGroupBox::mLegStarted, CCricketGroupBox::mSetStarted);
      set_active();
    }
  }
}

void CCricketGroupBox::push_button_score_clicked_slot()
{
  if (mActive && !mFinished)
  {
    mScoreInput = new CCricketInput(this, mpSettings, mPlayer, mGameWindow);
    mScoreInput->setAttribute(Qt::WA_DeleteOnClose);
    connect(mScoreInput, &CCricketInput::signal_cricket_submit_button_pressed, this, &CCricketGroupBox::cricket_submit_button_pressed_slot);
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
  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));

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
    mUi->label_extra15->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_16:
    mUi->label_extra16->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_17:
    mUi->label_extra17->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_18:
    mUi->label_extra18->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_19:
    mUi->label_extra19->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_20:
    mUi->label_extra20->setNum(static_cast<int>(iPoints));
    break;
  case ECricketSlots::SLOT_25:
    mUi->label_extra25->setNum(static_cast<int>(iPoints));
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
        mUi->label_15slot1->clear();
        mUi->label_15slot2->clear();
        mUi->label_15slot3->clear();
      }
      else if (i == 1) mUi->label_15slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_15slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_15slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_16:
      if (i == 0)
      {
        mUi->label_16slot1->clear();
        mUi->label_16slot2->clear();
        mUi->label_16slot3->clear();
      }
      else if (i == 1) mUi->label_16slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_16slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_16slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_17:
      if (i == 0)
      {
        mUi->label_17slot1->clear();
        mUi->label_17slot2->clear();
        mUi->label_17slot3->clear();
      }
      else if (i == 1) mUi->label_17slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_17slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_17slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_18:
      if (i == 0)
      {
        mUi->label_18slot1->clear();
        mUi->label_18slot2->clear();
        mUi->label_18slot3->clear();
      }
      else if (i == 1) mUi->label_18slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_18slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_18slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_19:
      if (i == 0)
      {
        mUi->label_19slot1->clear();
        mUi->label_19slot2->clear();
        mUi->label_19slot3->clear();
      }
      else if (i == 1) mUi->label_19slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_19slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_19slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_20:
      if (i == 0)
      {
        mUi->label_20slot1->clear();
        mUi->label_20slot2->clear();
        mUi->label_20slot3->clear();
      }
      else if (i == 1) mUi->label_20slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_20slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_20slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    case ECricketSlots::SLOT_25:
      if (i == 0)
      {
        mUi->label_25slot1->clear();
        mUi->label_25slot2->clear();
        mUi->label_25slot3->clear();
      }
      else if (i == 1) mUi->label_25slot1->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 2) mUi->label_25slot2->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      else if (i == 3) mUi->label_25slot3->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
      break;
    default:;
    }
  }
}

uint32_t CCricketGroupBox::get_score() const
{
  return mPlayer->get_score();
}

void CCricketGroupBox::set_leg_history()
{
  QVector<QVector<QString>> legscores = mPlayer->get_score_legs();
  QVector<QVector<QVector<QString>>> totalscores = mPlayer->get_scoring_history();
  if (legscores.size() == 0 && totalscores.size() > 0)
  {
    legscores = totalscores.last();
  }

  if (mpSettings.mCutThroat)
  {
    QVector<QVector<QString>> temp = {};
    for (uint32_t i = 0; i < legscores.size(); i++)
    {
      if (legscores[i].last() != "")
      {
        temp.push_back(legscores[i]);
      }
    }
    legscores = {};
    legscores = temp;
  }

  mUi->textBrowser->clear();
  for (uint32_t i = 0; i < legscores.size(); i++)
  {
    for (uint32_t j = 0; j < legscores[i].size(); j++)
    {
      QString temp = legscores[i][j];
      if (temp.size() > 1)
      {
        if (temp[1] == '0')
        {
          legscores[i][j] = " X";
        }
        else
        {
          legscores[i][j][0] = legscores[i][j][0].toUpper();
        }
      }
    }

    if (legscores[i].size() == 3)
    {
      QString line = QString::number(i+1) + ": " + legscores[i][0] + "  " + legscores[i][1] + "  " + legscores[i][2];
      mUi->textBrowser->append(line);
    }
    else if (legscores[i].size() == 2)
    {
      QString line = QString::number(i+1) + ": " + legscores[i][0] + "  " + legscores[i][1];
      mUi->textBrowser->append(line);
    }
    else
    {
      QString line = QString::number(i+1) + ": " + legscores[i][0];
      mUi->textBrowser->append(line);
    }
  }
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

  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
  QString hpr = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->label_hitsPerRound->setText(hpr);
  set_leg_history();

  if (mFinished)
  {
    unset_finished();
  }
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

void CCricketGroupBox::play_fail_sounds()
{
  std::srand(static_cast<unsigned> (std::time(0)));
  uint32_t rnd = 1 + (std::rand() % 12);

  switch (rnd)
  {
  case 1:
    mSound1.play();
    break;
  case 2:
    mSound2.play();
    break;
  case 3:
    mSound3.play();
    break;
  case 4:
    mSound4.play();
    break;
  case 5:
    mSound5.play();
    break;
  case 6:
    mSound6.play();
    break;
  case 7:
    mSound7.play();
    break;
  case 8:
    mSound8.play();
    break;
  case 9:
    mSound9.play();
    break;
  case 10:
    mSound10.play();
    break;
  case 11:
    mSound11.play();
    break;
  default:;
  }
}

void CCricketGroupBox::set_lcd_legs()
{
  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
}

bool CCricketGroupBox::mLegStarted = false;
bool CCricketGroupBox::mSetStarted = false;
