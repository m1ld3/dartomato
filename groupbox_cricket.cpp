#include "groupbox_cricket.h"
#include "ui_groupbox_cricket.h"
#include <QMessageBox>
#include "dialognameinput.h"
#include <QDebug>
#include <QSoundEffect>
#include <string>
#include <QString>
#include <algorithm>

CCricketGroupBox::CCricketGroupBox(QWidget * iParent, uint32_t iPlayerNumber, uint32_t iSets, uint32_t iLegs,
                                   CCricketClass * iPlayer, bool iCutThroat, bool iOffensive)
  : QGroupBox(iParent), mUi(new Ui::CCricketGroupBox), mPlayer(iPlayer), mPlayerNumber(iPlayerNumber - 1), mSets(iSets)
  , mLegs(iLegs), mScore(0), mFinished(false), mSetBegin(false), mLegBegin(false), mCutThroat(iCutThroat)
  , mOffensive(iOffensive), mTotalHits(0), mSlot15(0), mSlot16(0), mSlot17(0), mSlot18(0), mSlot19(0)
  , mSlot20(0), mSlot25(0), mExtra15(0), mExtra16(0), mExtra17(0), mExtra18(0), mExtra19(0), mExtra20(0), mExtra25(0)
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
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(4);
  mUi->lcdNumber->display(static_cast<int>(mScore));
  mUi->lcdNumber->setPalette(Qt::darkBlue);
  set_label_extra15(mPlayer->get_extra15());
  set_label_extra16(mPlayer->get_extra16());
  set_label_extra17(mPlayer->get_extra17());
  set_label_extra18(mPlayer->get_extra18());
  set_label_extra19(mPlayer->get_extra19());
  set_label_extra20(mPlayer->get_extra20());
  set_label_extra25(mPlayer->get_extra25());
  QString text = "Player " + QString::number(mPlayerNumber);
  mUi->label_playername->setText(text);
  QString hitsPerRound = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->label_hitsPerRound->setText(hitsPerRound);
  uint32_t w = 80;
  uint32_t h = 80;

  if (mActive)
  {
    mUi->label_pic->setPixmap(mPixMap.scaled(w, h, Qt::KeepAspectRatio));
  }
  else
  {
    mUi->label_pic->clear();
  }

  connect(mUi->label_pic,SIGNAL(signal_player_active_button_pressed()), this, SLOT(signal_player_active_button_pressed()));
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

void CCricketGroupBox::on_pushButton_name_clicked()
{
  CDialogNameInput *dn = new CDialogNameInput(this, mUi->label_playername->text());
  connect(dn, SIGNAL(ok_button_clicked(QString&)), this, SLOT(ok_button_clicked(QString&)));
  dn->show();
}

void CCricketGroupBox::ok_button_clicked(QString && iName)
{
  mPlayerName = iName;
  mUi->label_playername->setText(mPlayerName);
}

void CCricketGroupBox::signal_cricket_submit_button_pressed(uint32_t iNumberOfDarts, QVector<QString> & iDarts)
{
  CCricketGroupBox::mLegStarted = true;
  CCricketGroupBox::mSetStarted = true;
  bool newset = false;
  uint32_t hits = 0;
  uint32_t hits_old = 0;
  mSlot15 = mPlayer->get_slot15();
  mSlot16 = mPlayer->get_slot16();
  mSlot17 = mPlayer->get_slot17();
  mSlot18 = mPlayer->get_slot18();
  mSlot19 = mPlayer->get_slot19();
  mSlot20 = mPlayer->get_slot20();
  mSlot25 = mPlayer->get_slot25();
  mExtra15 = mPlayer->get_extra15();
  mExtra16 = mPlayer->get_extra16();
  mExtra17 = mPlayer->get_extra17();
  mExtra18 = mPlayer->get_extra18();
  mExtra19 = mPlayer->get_extra19();
  mExtra20 = mPlayer->get_extra20();
  mExtra25 = mPlayer->get_extra25();
  uint32_t newhits = mTotalHits;

  if (!mCutThroat)
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
      switch (val)
      {
      case 15:
        if (mSlot15 < 3)
        {
          if (mSlot15 + hits <= 3)
          {
            mSlot15 += hits;
            set_label15(mSlot15);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot15;
            mSlot15 = 3;
            set_label15(mSlot15);
            if (mGameWindow->is_slot15_free(mPlayerNumber))
            {
              mExtra15 += hits * 15;
              set_label_extra15(mExtra15);
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
          if (mGameWindow->is_slot15_free(mPlayerNumber))
          {
            mExtra15 += hits * 15;
            set_label_extra15(mExtra15);
            mTotalHits += hits;
          }
        }
        break;
      case 16:
        if (mSlot16 < 3)
        {
          if (mSlot16 + hits <= 3)
          {
            mSlot16 += hits;
            set_label16(mSlot16);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot16;
            mSlot16 = 3;
            set_label16(mSlot16);
            if (mGameWindow->is_slot16_free(mPlayerNumber))
            {
              mExtra16 += hits * 16;
              set_label_extra16(mExtra16);
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
          if (mGameWindow->is_slot16_free(mPlayerNumber))
          {
            mExtra16 += hits * 16;
            set_label_extra16(mExtra16);
            mTotalHits += hits;
          }
        }
        break;
      case 17:
        if (mSlot17 < 3)
        {
          if (mSlot17 + hits <= 3)
          {
            mSlot17 += hits;
            set_label17(mSlot17);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot17;
            mSlot17 = 3;
            set_label17(mSlot17);
            if (mGameWindow->is_slot17_free(mPlayerNumber))
            {
              mExtra17 += hits * 17;
              set_label_extra17(mExtra17);
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
          if (mGameWindow->is_slot17_free(mPlayerNumber))
          {
            mExtra17 += hits * 17;
            set_label_extra17(mExtra17);
            mTotalHits += hits;
          }
        }
        break;
      case 18:
        if (mSlot18 < 3)
        {
          if (mSlot18 + hits <= 3)
          {
            mSlot18 += hits;
            set_label18(mSlot18);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot18;
            mSlot18 = 3;
            set_label18(mSlot18);
            if (mGameWindow->is_slot18_free(mPlayerNumber))
            {
              mExtra18 += hits * 18;
              set_label_extra18(mExtra18);
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
          if (mGameWindow->is_slot18_free(mPlayerNumber))
          {
            mExtra18 += hits * 18;
            set_label_extra18(mExtra18);
            mTotalHits += hits;
          }
        }
        break;
      case 19:
        if (mSlot19 < 3)
        {
          if (mSlot19 + hits <= 3)
          {
            mSlot19 += hits;
            set_label19(mSlot19);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot19;
            mSlot19 = 3;
            set_label19(mSlot19);
            if (mGameWindow->is_slot19_free(mPlayerNumber))
            {
              mExtra19 += hits * 19;
              set_label_extra19(mExtra19);
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
          if (mGameWindow->is_slot19_free(mPlayerNumber))
          {
            mExtra19 += hits * 19;
            set_label_extra19(mExtra19);
            mTotalHits += hits;
          }
        }
        break;
      case 20:
        if (mSlot20 < 3)
        {
          if (mSlot20 + hits <= 3)
          {
            mSlot20 += hits;
            set_label20(mSlot20);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot20;
            mSlot20 = 3;
            set_label20(mSlot20);
            if (mGameWindow->is_slot20_free(mPlayerNumber))
            {
              mExtra20 += hits * 20;
              set_label_extra20(mExtra20);
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
          if (mGameWindow->is_slot20_free(mPlayerNumber))
          {
            mExtra20 += hits * 20;
            set_label_extra20(mExtra20);
            mTotalHits += hits;
          }
        }
        break;
      case 25:
        if (mSlot25 < 3)
        {
          if (mSlot25 + hits <= 3)
          {
            mSlot25 += hits;
            set_label25(mSlot25);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot25;
            mSlot25 = 3;
            set_label25(mSlot25);
            if (mGameWindow->is_slot25_free(mPlayerNumber))
            {
              mExtra25 += hits * 25;
              set_label_extra25(mExtra25);
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
          if (mGameWindow->is_slot25_free(mPlayerNumber))
          {
            mExtra25 += hits * 25;
            set_label_extra25(mExtra25);
            mTotalHits += hits;
          }
        }
        break;
      default:;
      }
    }

    mPlayer->set_slot15(mSlot15);
    mPlayer->set_slot16(mSlot16);
    mPlayer->set_slot17(mSlot17);
    mPlayer->set_slot18(mSlot18);
    mPlayer->set_slot19(mSlot19);
    mPlayer->set_slot20(mSlot20);
    mPlayer->set_slot25(mSlot25);
    mPlayer->set_extra15(mExtra15);
    mPlayer->set_extra16(mExtra16);
    mPlayer->set_extra17(mExtra17);
    mPlayer->set_extra18(mExtra18);
    mPlayer->set_extra19(mExtra19);
    mPlayer->set_extra20(mExtra20);
    mPlayer->set_extra25(mExtra25);
    mPlayer->set_score();
    mScore = mPlayer->get_score();
  }
  else
  {
    uint32_t Extra15CT = 0;
    uint32_t Extra16CT = 0;
    uint32_t Extra17CT = 0;
    uint32_t Extra18CT = 0;
    uint32_t Extra19CT = 0;
    uint32_t Extra20CT = 0;
    uint32_t Extra25CT = 0;
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
      switch (val)
      {
      case 15:
        if (mSlot15 < 3)
        {
          if (mSlot15 + hits <= 3)
          {
            mSlot15 += hits;
            set_label15(mSlot15);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot15;
            mSlot15 = 3;
            set_label15(mSlot15);
            if (mGameWindow->is_slot15_free(mPlayerNumber))
            {
              Extra15CT += hits * 15;
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
          if (mGameWindow->is_slot15_free(mPlayerNumber))
          {
            Extra15CT += hits * 15;
            mTotalHits += hits;
          }
        }
        break;
      case 16:
        if (mSlot16 < 3)
        {
          if (mSlot16 + hits <= 3)
          {
            mSlot16 += hits;
            set_label16(mSlot16);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot16;
            mSlot16 = 3;
            set_label16(mSlot16);
            if (mGameWindow->is_slot16_free(mPlayerNumber))
            {
              Extra16CT += hits * 16;
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
          if (mGameWindow->is_slot16_free(mPlayerNumber))
          {
            Extra16CT += hits * 16;
            mTotalHits += hits;
          }
        }
        break;
      case 17:
        if (mSlot17 < 3)
        {
          if (mSlot17 + hits <= 3)
          {
            mSlot17 += hits;
            set_label17(mSlot17);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot17;
            mSlot17 = 3;
            set_label17(mSlot17);
            if (mGameWindow->is_slot17_free(mPlayerNumber))
            {
              Extra17CT += hits * 17;
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
          if (mGameWindow->is_slot17_free(mPlayerNumber))
          {
            Extra17CT += hits * 17;
            mTotalHits += hits;
          }
        }
        break;
      case 18:
        if (mSlot18 < 3)
        {
          if (mSlot18 + hits <= 3)
          {
            mSlot18 += hits;
            set_label18(mSlot18);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot18;
            mSlot18 = 3;
            set_label18(mSlot18);
            if (mGameWindow->is_slot18_free(mPlayerNumber))
            {
              Extra18CT += hits * 18;
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
          if (mGameWindow->is_slot18_free(mPlayerNumber))
          {
            Extra18CT += hits * 18;
            mTotalHits += hits;
          }
        }
        break;
      case 19:
        if (mSlot19 < 3)
        {
          if (mSlot19 + hits <= 3)
          {
            mSlot19 += hits;
            set_label19(mSlot19);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot19;
            mSlot19 = 3;
            set_label19(mSlot19);
            if (mGameWindow->is_slot19_free(mPlayerNumber))
            {
              Extra19CT += hits * 19;
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
          if (mGameWindow->is_slot19_free(mPlayerNumber))
          {
            Extra19CT += hits * 19;
            mTotalHits += hits;
          }
        }
        break;
      case 20:
        if (mSlot20 < 3)
        {
          if (mSlot20 + hits <= 3)
          {
            mSlot20 += hits;
            set_label20(mSlot20);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot20;
            mSlot20 = 3;
            set_label20(mSlot20);
            if (mGameWindow->is_slot20_free(mPlayerNumber))
            {
              Extra20CT += hits * 20;
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
          if (mGameWindow->is_slot20_free(mPlayerNumber))
          {
            Extra20CT += hits * 20;
            mTotalHits += hits;
          }
        }
        break;
      case 25:
        if (mSlot25 < 3)
        {
          if (mSlot25 + hits <= 3)
          {
            mSlot25 += hits;
            set_label25(mSlot25);
            mTotalHits += hits;
          }
          else
          {
            hits_old = hits;
            hits -= 3 - mSlot25;
            mSlot25 = 3;
            set_label25(mSlot25);
            if (mGameWindow->is_slot25_free(mPlayerNumber))
            {
              Extra25CT += hits * 25;
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
          if (mGameWindow->is_slot25_free(mPlayerNumber))
          {
              Extra25CT += hits * 25;
            mTotalHits += hits;
          }
        }
        break;
      default:;
      }
    }

    mPlayer->set_slot15(mSlot15);
    mPlayer->set_slot16(mSlot16);
    mPlayer->set_slot17(mSlot17);
    mPlayer->set_slot18(mSlot18);
    mPlayer->set_slot19(mSlot19);
    mPlayer->set_slot20(mSlot20);
    mPlayer->set_slot25(mSlot25);
    mGameWindow->increase_score15(Extra15CT);
    mGameWindow->increase_score16(Extra16CT);
    mGameWindow->increase_score17(Extra17CT);
    mGameWindow->increase_score18(Extra18CT);
    mGameWindow->increase_score19(Extra19CT);
    mGameWindow->increase_score20(Extra20CT);
    mGameWindow->increase_score25(Extra25CT);
    mGameWindow->set_scores();
    mScore = mPlayer->get_score();
  }

  if (mTotalHits == newhits && mOffensive)
  {
    play_fail_sounds();
  }

  mPlayer->compute_hits_per_round(iNumberOfDarts, mTotalHits);
  mPlayer->update_darts(iDarts);
  QString hpr = QString::number(mPlayer->get_hits_per_round(), 'f', 3);
  mUi->label_hitsPerRound->setText(hpr);

  if (!mCutThroat)
  {
    if (mScoreInput->are_slots_full() && mGameWindow->is_score_bigger(mScore))
    {  // leg won
      newset = mPlayer->increase_setslegs();
      mPlayer->set_leg_win_array(true);
      emit signal_update_history();
      emit signal_reset_scores();
      if (mActive && !newset)
      {
        emit signal_update_player("leg");
        CCricketGroupBox::mLegStarted = false;
      }
      else if (mActive && newset)
      {
        emit signal_update_player("set");
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
        emit signal_update_player("default");
      }
      mPlayer->set_leg_win_array(false);
      mGameWindow->update_labels();
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
        emit signal_update_player("leg");
        CCricketGroupBox::mLegStarted = false;
      }
      else if (mActive && newset)
      {
        emit signal_update_player("set");
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
        emit signal_update_player("default");
      }
      mPlayer->set_leg_win_array(false);
      mGameWindow->update_labels();
    }
  }
  set_leg_history();
  close_cricket_input();
}


void CCricketGroupBox::signal_player_active_button_pressed()
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

void CCricketGroupBox::on_pushButton_score_clicked()
{
  if (mActive && !mFinished)
  {
    mScoreInput = new CCricketInput(this, mSets, mLegs, mPlayer, mGameWindow, mCutThroat);
    mScoreInput->setAttribute(Qt::WA_DeleteOnClose);
    connect(mScoreInput, SIGNAL (signal_cricket_submit_button_pressed(int&, QVector<QString>)), this, SLOT (signal_cricket_submit_button_pressed(int&, QVector<QString>)));
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
  set_label15(0);
  set_label16(0);
  set_label17(0);
  set_label18(0);
  set_label19(0);
  set_label20(0);
  set_label25(0);
  set_label_extra15(0);
  set_label_extra16(0);
  set_label_extra17(0);
  set_label_extra18(0);
  set_label_extra19(0);
  set_label_extra20(0);
  set_label_extra25(0);
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

uint32_t CCricketGroupBox::get_slot15() const
{
  return mPlayer->get_slot15();
}

uint32_t CCricketGroupBox::get_slot16() const
{
  return mPlayer->get_slot16();
}

uint32_t CCricketGroupBox::get_slot17() const
{
  return mPlayer->get_slot17();
}

uint32_t CCricketGroupBox::get_slot18() const
{
  return mPlayer->get_slot18();
}

uint32_t CCricketGroupBox::get_slot19() const
{
  return mPlayer->get_slot19();
}

uint32_t CCricketGroupBox::get_slot20() const
{
  return mPlayer->get_slot20();
}

uint32_t CCricketGroupBox::get_slot25() const
{
  return mPlayer->get_slot25();
}

void CCricketGroupBox::set_slot15(uint32_t iHits)
{
  mPlayer->set_slot15(iHits);
}

void CCricketGroupBox::set_slot16(uint32_t iHits)
{
  mPlayer->set_slot16(iHits);
}

void CCricketGroupBox::set_slot17(uint32_t iHits)
{
  mPlayer->set_slot17(iHits);
}

void CCricketGroupBox::set_slot18(uint32_t iHits)
{
  mPlayer->set_slot18(iHits);
}

void CCricketGroupBox::set_slot19(uint32_t iHits)
{
  mPlayer->set_slot19(iHits);
}

void CCricketGroupBox::set_slot20(uint32_t iHits)
{
  mPlayer->set_slot20(iHits);
}

void CCricketGroupBox::set_slot25(uint32_t iHits)
{
  mPlayer->set_slot25(iHits);
}

void CCricketGroupBox::set_extra15(uint32_t iPoints)
{
  mPlayer->set_extra15(iPoints);
}

void CCricketGroupBox::set_extra16(uint32_t iPoints)
{
  mPlayer->set_extra16(iPoints);
}

void CCricketGroupBox::set_extra25(uint32_t iPoints)
{
  mPlayer->set_extra25(iPoints);
}

uint32_t CCricketGroupBox::get_extra15() const
{
  return mPlayer->get_extra15();
}

uint32_t CCricketGroupBox::get_extra16() const
{
  return mPlayer->get_extra16();
}

uint32_t CCricketGroupBox::get_extra17() const
{
  return mPlayer->get_extra17();
}

uint32_t CCricketGroupBox::get_extra18() const
{
  return mPlayer->get_extra18();
}

uint32_t CCricketGroupBox::get_extra19() const
{
  return mPlayer->get_extra19();
}

uint32_t CCricketGroupBox::get_extra20() const
{
  return mPlayer->get_extra20();
}

uint32_t CCricketGroupBox::get_extra25() const
{
  return mPlayer->get_extra25();
}

void CCricketGroupBox::set_label_extra15(uint32_t iPoints)
{
  mUi->label_extra15->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra16(uint32_t iPoints)
{
  mUi->label_extra16->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra17(uint32_t iPoints)
{
  mUi->label_extra17->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra18(uint32_t iPoints)
{
  mUi->label_extra18->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra19(uint32_t iPoints)
{
  mUi->label_extra19->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra20(uint32_t iPoints)
{
  mUi->label_extra20->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label_extra25(uint32_t iPoints)
{
  mUi->label_extra25->setNum(static_cast<int>(iPoints));
}

void CCricketGroupBox::set_label15(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_15slot1->clear();
    mUi->label_15slot2->clear();
    mUi->label_15slot3->clear();
    break;
  case 1:
    mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_15slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_15slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_15slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label16(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_16slot1->clear();
    mUi->label_16slot2->clear();
    mUi->label_16slot3->clear();
    break;
  case 1:
    mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_16slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_16slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_16slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label17(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_17slot1->clear();
    mUi->label_17slot2->clear();
    mUi->label_17slot3->clear();
    break;
  case 1:
    mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_17slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_17slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_17slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label18(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_18slot1->clear();
    mUi->label_18slot2->clear();
    mUi->label_18slot3->clear();
    break;
  case 1:
    mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_18slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_18slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_18slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label19(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_19slot1->clear();
    mUi->label_19slot2->clear();
    mUi->label_19slot3->clear();
    break;
  case 1:
    mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_19slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_19slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_19slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label20(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_20slot1->clear();
    mUi->label_20slot2->clear();
    mUi->label_20slot3->clear();
    break;
  case 1:
    mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_20slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_20slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_20slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
  }
}

void CCricketGroupBox::set_label25(uint32_t iHits)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iHits)
  {
  default:
    mUi->label_25slot1->clear();
    mUi->label_25slot2->clear();
    mUi->label_25slot3->clear();
    break;
  case 1:
    mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 2:
    mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_25slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    break;
  case 3:
    mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_25slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    mUi->label_25slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
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
  if (mCutThroat)
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

void CCricketGroupBox::set_extra17(uint32_t iPoints)
{
  mPlayer->set_extra17(iPoints);
}

void CCricketGroupBox::set_extra18(uint32_t iPoints)
{
  mPlayer->set_extra18(iPoints);
}

void CCricketGroupBox::set_extra19(uint32_t iPoints)
{
  mPlayer->set_extra19(iPoints);
}

void CCricketGroupBox::set_extra20(uint32_t iPoints)
{
  mPlayer->set_extra20(iPoints);
}

void CCricketGroupBox::on_pushButton_undo_clicked()
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
  set_label15(get_slot15());
  set_label16(get_slot16());
  set_label17(get_slot17());
  set_label18(get_slot18());
  set_label19(get_slot19());
  set_label20(get_slot20());
  set_label25(get_slot25());
  set_label_extra15(get_extra15());
  set_label_extra16(get_extra16());
  set_label_extra17(get_extra17());
  set_label_extra18(get_extra18());
  set_label_extra19(get_extra19());
  set_label_extra20(get_extra20());
  set_label_extra25(get_extra25());
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

void CCricketGroupBox::signal_set_label_slot(uint32_t iHits, uint32_t iSlot)
{
  uint32_t w = 25;
  uint32_t h = 25;
  switch (iSlot)
  {
  case 15:
    switch (iHits)
    {
    default:
      mUi->label_15slot1->clear();
      mUi->label_15slot2->clear();
      mUi->label_15slot3->clear();
      break;
    case 1:
      mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_15slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_15slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_15slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_15slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 16:
    switch (iHits)
    {
    default:
      mUi->label_16slot1->clear();
      mUi->label_16slot2->clear();
      mUi->label_16slot3->clear();
      break;
    case 1:
      mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_16slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_16slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_16slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_16slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 17:
    switch (iHits)
    {
    default:
      mUi->label_17slot1->clear();
      mUi->label_17slot2->clear();
      mUi->label_17slot3->clear();
      break;
    case 1:
      mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
        mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
        mUi->label_17slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
      mUi->label_17slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_17slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_17slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 18:
    switch (iHits)
    {
    default:
      mUi->label_18slot1->clear();
      mUi->label_18slot2->clear();
      mUi->label_18slot3->clear();
      break;
    case 1:
      mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_18slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_18slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_18slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_18slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 19:
    switch (iHits)
    {
    default:
      mUi->label_19slot1->clear();
      mUi->label_19slot2->clear();
      mUi->label_19slot3->clear();
      break;
    case 1:
      mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_19slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_19slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_19slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_19slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 20:
    switch (iHits)
    {
    default:
      mUi->label_20slot1->clear();
      mUi->label_20slot2->clear();
      mUi->label_20slot3->clear();
      break;
    case 1:
      mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_20slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_20slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_20slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_20slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
    break;
  case 25:
    switch (iHits)
    {
    default:
      mUi->label_25slot1->clear();
      mUi->label_25slot2->clear();
      mUi->label_25slot3->clear();
      break;
    case 1:
      mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 2:
      mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_25slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      break;
    case 3:
      mUi->label_25slot1->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_25slot2->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
      mUi->label_25slot3->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
    }
  default:;
  }
}

void CCricketGroupBox::increase_extra15(uint32_t iPoints)
{
  mPlayer->set_extra15(iPoints);
}

void CCricketGroupBox::increase_extra16(uint32_t iPoints)
{
  mPlayer->set_extra16(iPoints);
}

void CCricketGroupBox::increase_extra17(uint32_t iPoints)
{
  mPlayer->set_extra17(iPoints);
}

void CCricketGroupBox::increase_extra18(uint32_t iPoints)
{
  mPlayer->set_extra18(iPoints);
}

void CCricketGroupBox::increase_extra19(uint32_t iPoints)
{
  mPlayer->set_extra19(iPoints);
}

void CCricketGroupBox::increase_extra20(uint32_t iPoints)
{
  mPlayer->set_extra20(iPoints);
}

void CCricketGroupBox::increase_extra25(uint32_t iPoints)
{
  mPlayer->set_extra25(iPoints);
}

void CCricketGroupBox::set_score()
{
  mPlayer->set_score();
}

void CCricketGroupBox::update_labels()
{
  set_label_extra15(mPlayer->get_extra15());
  set_label_extra16(mPlayer->get_extra16());
  set_label_extra17(mPlayer->get_extra17());
  set_label_extra18(mPlayer->get_extra18());
  set_label_extra19(mPlayer->get_extra19());
  set_label_extra20(mPlayer->get_extra20());
  set_label_extra25(mPlayer->get_extra25());
  mUi->lcdNumber->display(static_cast<int>(mPlayer->get_score()));
}

void CCricketGroupBox::update_darts(QVector<QString> && iDarts)
{
  mPlayer->update_darts(iDarts);
  mPlayer->set_slot15(mPlayer->get_slot15());
  mPlayer->set_slot16(mPlayer->get_slot16());
  mPlayer->set_slot17(mPlayer->get_slot17());
  mPlayer->set_slot18(mPlayer->get_slot18());
  mPlayer->set_slot19(mPlayer->get_slot19());
  mPlayer->set_slot20(mPlayer->get_slot20());
  mPlayer->set_slot25(mPlayer->get_slot25());
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
