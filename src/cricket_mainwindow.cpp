#include "cricket_mainwindow.h"
#include "ui_cricket_mainwindow.h"
#include "groupbox_cricket.h"
#include "cricket_class.h"
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>


CCricketMainWindow::CCricketMainWindow(QWidget * iParent, const CSettings & iSettings, CGameDataHandler & iGameDataHandler)
  : QMainWindow(iParent)
  , mUi(new Ui::CCricketMainWindow)
  , mSettings(iSettings)
  , mGameDataHandler(iGameDataHandler)
  , mNumberOfPlayers(mSettings.PlayersList.size())
  , mTimeStamp(QDateTime::currentDateTimeUtc())
{
  mUi->setupUi(this);
  QWidget::setWindowTitle("Cricket" + QString(mSettings.CutThroat ? " (CutThroat)" : ""));
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
}

CCricketMainWindow::~CCricketMainWindow()
{
  delete mUi;
  for (auto box : mPlayerBox)
  {
    delete box;
  }
}

void CCricketMainWindow::add_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox.push_back(new CCricketGroupBox(this, mSettings, i + 1));
    mPlayerBox[i]->setAttribute(Qt::WA_DeleteOnClose);
    mPlayerBox[i]->set_inactive();
    mUi->gridLayoutCricket->addWidget(mPlayerBox[i], i < 4 ? 0 : 1, i % 4);
  }
}

void CCricketMainWindow::save_current_game()
{
  QVector<QVector<CCricketClass::CPlayerData>> gameDataVec;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    gameDataVec.append(mPlayerBox[i]->get_game_data());
  }
  mGameDataHandler.save_game_to_db(gameDataVec, mTimeStamp.toString(), game_finished(), mSettings);
}

bool CCricketMainWindow::game_finished() const
{
  bool finished = true;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    finished = finished && mPlayerBox[i]->is_finished();
  }

  return finished;
}

void CCricketMainWindow::closeEvent(QCloseEvent * iEvent)
{
  if (game_finished())
  {
    save_current_game();
    iEvent->accept();  // TODO: where restart game with same config button?
  }
  else
  {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Game not yet finished!",
                                                               tr("Do you want to save or abort the current game?\n"),
                                                               QMessageBox::Save | QMessageBox::Abort | QMessageBox::Cancel);
    if (resBtn == QMessageBox::Save)
    {
      save_current_game();
      iEvent->accept();
    }
    else if (resBtn == QMessageBox::Abort)
    {
      iEvent->accept();
    }
    else iEvent->ignore();
  }
}

void CCricketMainWindow::set_active_player(uint32_t iPlayer)
{
  mActivePlayer = iPlayer;
}

void CCricketMainWindow::update_active_player()
{
  mActivePlayer = (mActivePlayer + 1) % mNumberOfPlayers;
}

void CCricketMainWindow::inactivate_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_inactive();
  }
}

void CCricketMainWindow::handle_update_default()
{
  update_active_player();
  inactivate_all_players();
  mPlayerBox[mActivePlayer]->set_active();
}

void CCricketMainWindow::handle_update_leg()
{
  inactivate_all_players();
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (mPlayerBox[i]->has_begun_leg())
    {
      mPlayerBox[i]->unset_leg_begin();
      mActivePlayer = i;
      update_active_player();
      mPlayerBox[mActivePlayer]->set_active();
      mPlayerBox[mActivePlayer]->set_leg_begin();
      break;
    }
  }
}

void CCricketMainWindow::unset_leg_begin_for_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->unset_leg_begin();
  }
}

void CCricketMainWindow::handle_update_set()
{
  inactivate_all_players();
  unset_leg_begin_for_all_players();
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (mPlayerBox[i]->has_begun_set())
    {
      mPlayerBox[i]->unset_set_begin();
      mActivePlayer = i;
      update_active_player();
      mPlayerBox[mActivePlayer]->set_active();
      mPlayerBox[mActivePlayer]->set_leg_begin();
      mPlayerBox[mActivePlayer]->set_set_begin();
      break;
    }
  }
  for (auto box: mPlayerBox)
  {
    box->reset_legs();
    box->set_lcd_legs();
  }
}

void CCricketMainWindow::update_players(const EUpdateType iType)
{
  if (iType == EUpdateType::DEFAULT)
  {
    handle_update_default();
  }
  else if (iType == EUpdateType::LEG)
  {
    handle_update_leg();
  }
  else if (iType == EUpdateType::SET)
  {
    handle_update_set();
  }
}

void CCricketMainWindow::reset_scores_of_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->reset();
  }
}

void CCricketMainWindow::handle_game_won(uint32_t iPlayerNumber)
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_finished();
  }

  mPlayerBox[iPlayerNumber]->close_cricket_input();
  QString name = mSettings.PlayersList.at(iPlayerNumber);
  QString text = name + " has won the game. Congratulations! ";
  QMessageBox::about(this,"Game finished", text);
}

void CCricketMainWindow::unset_set_begin_for_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->unset_set_begin();
  }
}

void CCricketMainWindow::inactivate_players(uint32_t iPlayerNumber, bool iLegStarted, bool iSetStarted)
{
  inactivate_all_players();
  set_active_player(iPlayerNumber);

  if (!iLegStarted)
  {
    unset_leg_begin_for_all_players();
    mPlayerBox[iPlayerNumber]->set_leg_begin();
  }

  if (!iSetStarted)
  {
    unset_leg_begin_for_all_players();
    unset_set_begin_for_all_players();
    mPlayerBox[iPlayerNumber]->set_leg_begin();
    mPlayerBox[iPlayerNumber]->set_set_begin();
  }
}

void CCricketMainWindow::create_snapshots_of_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->create_snapshot();
  }
}

bool CCricketMainWindow::is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const
{
  bool free = false;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      free |= mPlayerBox[i]->get_slot(iSlot) < 3;
    }
  }
  return free;
}

bool CCricketMainWindow::is_score_bigger(uint32_t iScore) const
{
  bool result = true;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    result = result && mPlayerBox[i]->get_score() <= iScore;
  }
  return result;
}

bool CCricketMainWindow::is_score_smaller(uint32_t iScore) const
{
  bool result = true;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    result = result && mPlayerBox[i]->get_score() >= iScore;
  }
  return result;
}

void CCricketMainWindow::increase_slot_score(const ECricketSlots iSlot, uint32_t iPoints)
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (mPlayerBox[i]->get_slot(iSlot) != 3)
    {
      mPlayerBox[i]->increase_extra_points(iSlot, iPoints);
    }
    else
    {
      mPlayerBox[i]->increase_extra_points(iSlot, 0);
    }
  }
}

QVector<uint32_t> CCricketMainWindow::compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer)
{
  QVector<uint32_t> extraPoints = {};

  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      uint32_t extra = 0;
      if (mPlayerBox[i]->get_slot(iSlot) != 3)
      {
        extra = mPlayerBox[i]->get_extra_points(iSlot) + iPoints;
      }
      else
      {
        extra = mPlayerBox[i]->get_extra_points(iSlot);
      }
      extraPoints.push_back(extra);
    }
  }
  return extraPoints;
}

void CCricketMainWindow::set_scores()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_score();
  }
}

void CCricketMainWindow::update_extra_points_labels()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->update_extra_points_labels();
  }
}

void CCricketMainWindow::update_darts(uint32_t iPlayer)
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    if (i != iPlayer)
    {
      mPlayerBox[i]->update_darts({""});
    }
  }
}
