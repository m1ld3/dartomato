#include "cricketmainwindow.h"
#include "ui_cricketmainwindow.h"
#include "groupbox_cricket.h"
#include "cricketclass.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>


CCricketMainWindow::CCricketMainWindow(QWidget * iParent, const CSettings & ipSettings)
  : QMainWindow(iParent)
  , mUi(new Ui::CCricketMainWindow)
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  QWidget::setWindowTitle("Cricket");

  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketPlayer.push_back(new CCricketClass(this, i+1, mpSettings));
    mCricketBox.push_back(new CCricketGroupBox(this, mpSettings, i+1, mCricketPlayer[i]));
    mCricketBox[i]->setAttribute(Qt::WA_DeleteOnClose);
    mCricketBox[i]->set_inactive();
    mUi->gridLayoutCricket->addWidget(mCricketBox[i], i<4 ? 0 : 1, i%4);
    connect(mCricketBox[i], &CCricketGroupBox::signal_update_player, this, &CCricketMainWindow::update_player_slot);
    connect(mCricketBox[i], &CCricketGroupBox::signal_reset_scores, this, &CCricketMainWindow::reset_scores_slot);
    connect(mCricketPlayer[i], &CCricketClass::signal_game_won, this, &CCricketMainWindow::game_won_slot);
    connect(mCricketBox[i], &CCricketGroupBox::signal_inactivate_players, this, &CCricketMainWindow::inactivate_players_slot);
    connect(mCricketBox[i], &CCricketGroupBox::signal_update_history, this, &CCricketMainWindow::update_history_slot);
  }

  mCricketBox[mActivePlayer]->set_set_begin();
  mCricketBox[mActivePlayer]->set_leg_begin();
  mCricketBox[mActivePlayer]->set_active();
}

CCricketMainWindow::~CCricketMainWindow()
{
  delete mUi;
  for (auto player : mCricketPlayer)
  {
    delete player;
  }
  for (auto box : mCricketBox)
  {
    delete box;
  }
}

void CCricketMainWindow::closeEvent(QCloseEvent * iEvent)
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Quit game",
                                                             tr("Are you sure you want to quit the game?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                             QMessageBox::No);
  if (resBtn != QMessageBox::Yes)
  {
    iEvent->ignore();
  }
  else
  {
    iEvent->accept();
  }
}

void CCricketMainWindow::set_active_player(uint32_t iPlayer)
{
  mActivePlayer = iPlayer;
}

void CCricketMainWindow::update_player()
{
  mActivePlayer = (mActivePlayer + 1) % mpSettings.mNumberOfPlayers;
}

void CCricketMainWindow::update_player_slot(const EUpdateType iType)
{
  if (iType == EUpdateType::DEFAULT)
  {
    update_player();
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      mCricketBox[i]->set_inactive();
    }
    mCricketBox[mActivePlayer]->set_active();
  }
  else if (iType == EUpdateType::LEG)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      if (mCricketBox[i]->has_begun_leg())
      {
        mCricketBox[i]->unset_leg_begin();
        mActivePlayer = i;
        update_player();
        for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
        {
          mCricketBox[i]->set_inactive();
        }
        mCricketBox[mActivePlayer]->set_active();
        mCricketBox[mActivePlayer]->set_leg_begin();
        break;
      }
    }
  }
  else if (iType == EUpdateType::SET)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      if (mCricketBox[i]->has_begun_set())
      {
        mCricketBox[i]->unset_set_begin();
        mActivePlayer = i;
        update_player();
        for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
        {
          mCricketBox[i]->set_inactive();
          mCricketBox[i]->unset_leg_begin();
        }
        mCricketBox[mActivePlayer]->set_active();
        mCricketBox[mActivePlayer]->set_leg_begin();
        mCricketBox[mActivePlayer]->set_set_begin();
        break;
      }
    }
    for (auto box: mCricketBox)
    {
      box->reset_legs();
      box->set_lcd_legs();
    }
  }
}

void CCricketMainWindow::reset_scores_slot()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->reset();
  }
}

void CCricketMainWindow::game_won_slot(uint32_t iPlayerNumber)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->set_finished();
  }

  mCricketBox[iPlayerNumber]->close_cricket_input();
  QString name = mCricketBox[iPlayerNumber]->get_player_number();
  QString text = name + " has won the game. Congratulations!. ";
  QMessageBox::about(this,"Game finished", text);
}

void CCricketMainWindow::inactivate_players_slot(uint32_t iPlayerNumber, bool iLegStarted, bool iSetStarted)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->set_inactive();
  }
  set_active_player(iPlayerNumber);

  if (!iLegStarted)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      mCricketBox[i]->unset_leg_begin();
    }
    mCricketBox[iPlayerNumber]->set_leg_begin();
  }

  if (!iSetStarted)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      mCricketBox[i]->unset_set_begin();
      mCricketBox[i]->unset_leg_begin();
    }
    mCricketBox[iPlayerNumber]->set_leg_begin();
    mCricketBox[iPlayerNumber]->set_set_begin();
  }
}

void CCricketMainWindow::update_history_slot()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->update_history();
  }
}

bool CCricketMainWindow::is_slot15_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot15() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot16_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot16() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot17_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot17() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot18_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot18() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot19_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot19() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot20_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot20() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_slot25_free(uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mCricketBox[i]->get_slot25() < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_score_bigger(uint32_t iScore) const
{
  bool result = true;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    result = result && mCricketBox[i]->get_score() <= iScore;
  }
  return result;
}

bool CCricketMainWindow::is_score_smaller(uint32_t iScore) const
{
  bool result = true;
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    result = result && mCricketBox[i]->get_score() >= iScore;
  }
  return result;
}

void CCricketMainWindow::increase_score15(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot15() != 3)
    {
      mCricketBox[i]->increase_extra15(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra15(0);
    }
  }
}

void CCricketMainWindow::increase_score16(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot16() != 3)
    {
      mCricketBox[i]->increase_extra16(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra16(0);
    }
  }
}

void CCricketMainWindow::increase_score17(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot17() != 3)
    {
      mCricketBox[i]->increase_extra17(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra17(0);
    }
  }
}

void CCricketMainWindow::increase_score18(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot18() != 3)
    {
      mCricketBox[i]->increase_extra18(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra18(0);
    }
  }
}

void CCricketMainWindow::increase_score19(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot19() != 3)
    {
      mCricketBox[i]->increase_extra19(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra19(0);
    }
  }
}

void CCricketMainWindow::increase_score20(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot20() != 3)
    {
      mCricketBox[i]->increase_extra20(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra20(0);
    }
  }
}

void CCricketMainWindow::increase_score25(uint32_t iPoints)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mCricketBox[i]->get_slot25() != 3)
    {
      mCricketBox[i]->increase_extra25(iPoints);
    }
    else
    {
      mCricketBox[i]->increase_extra25(0);
    }
  }
}

QVector<uint32_t> CCricketMainWindow::compute_extra15s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra15s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot15() != 3)
      {
        extra = mCricketBox[i]->get_extra15() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra15();
      }
      extra15s.push_back(extra);
    }
  }
  return extra15s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra16s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra16s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot16() != 3)
      {
        extra = mCricketBox[i]->get_extra16() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra16();
      }
      extra16s.push_back(extra);
    }
  }
  return extra16s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra17s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra17s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot17() != 3)
      {
        extra = mCricketBox[i]->get_extra17() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra17();
      }
      extra17s.push_back(extra);
    }
  }
  return extra17s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra18s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra18s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot18() != 3)
      {
        extra = mCricketBox[i]->get_extra18() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra18();
      }
      extra18s.push_back(extra);
    }
  }
  return extra18s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra19s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra19s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot19() != 3)
      {
        extra = mCricketBox[i]->get_extra19() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra19();
      }
      extra19s.push_back(extra);
    }
  }
  return extra19s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra20s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra20s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot20() != 3)
      {
        extra = mCricketBox[i]->get_extra20() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra20();
      }
      extra20s.push_back(extra);
    }
  }
  return extra20s;
}

QVector<uint32_t> CCricketMainWindow::compute_extra25s(uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extra25s = {};
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mCricketBox[i]->get_slot25() != 3)
      {
        extra = mCricketBox[i]->get_extra25() + iPoints;
      }
      else
      {
        extra = mCricketBox[i]->get_extra25();
      }
      extra25s.push_back(extra);
    }
  }
  return extra25s;
}

void CCricketMainWindow::set_scores()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->set_score();
  }
}

void CCricketMainWindow::update_labels()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mCricketBox[i]->update_labels();
  }
}

void CCricketMainWindow::update_darts(uint32_t iPlayer)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      mCricketBox[i]->update_darts({""});
    }
  }
}
