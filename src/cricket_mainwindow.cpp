#include "cricket_mainwindow.h"
#include "ui_cricket_mainwindow.h"
#include "groupbox_cricket.h"
#include "cricket_class.h"
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>


CCricketMainWindow::CCricketMainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler)
  : QMainWindow(iParent)
  , mUi(new Ui::CCricketMainWindow)
  , mSettings(iSettings)
  , mGameDataHandler(iGameDataHandler)
  , mNumberOfPlayers(mSettings.PlayersList.size())
  , mTimeStamp(QDateTime::currentDateTime())
{
  mUi->setupUi(this);
  QWidget::setWindowTitle("Cricket" + QString(mSettings.CutThroat ? " (CutThroat)" : ""));
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
}

CCricketMainWindow::CCricketMainWindow(QWidget *iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const QVector<QVector<CCricketClass::CPlayerData> > iGameData)
  : CCricketMainWindow(iParent, iSettings, iGameDataHandler)
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_game_data(iGameData.at(i));
    if (iGameData.at(i).back().Active) mActivePlayer = i;
  }
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
    mPlayerBox.push_back(new CCricketGroupBox(this, mSettings, i));
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
  auto gameData = CGameDataHandler::SGameData(mTimeStamp.toString(), game_finished(), mSettings, mWinningPlayer, {}, gameDataVec);
  mGameDataHandler.save_game_to_db(gameData);
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

void CCricketMainWindow::start_new_game_with_same_settings()
{
  mActivePlayer = mWinningPlayer = 0;
  clear_group_box_widgets();
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
  mTimeStamp = QDateTime::currentDateTime();
}

void CCricketMainWindow::clear_group_box_widgets()
{
  QLayoutItem * item;
  while ((item = mUi->gridLayoutCricket->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }
  mPlayerBox = {};
}

void CCricketMainWindow::save_unfinished_game_file()
{
  QFile file(OpenGamePath);

  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QTextStream stream(&file);
    stream << mTimeStamp.toString();
    file.close();
  }
}

void CCricketMainWindow::closeEvent(QCloseEvent * iEvent)
{
  if (game_finished())
  {
    save_current_game();
    iEvent->accept();
  }
  else
  {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Game not yet finished!",
                                                               tr("Do you want to save or abort the current game?\n"),
                                                               QMessageBox::Save | QMessageBox::Abort | QMessageBox::Cancel);
    if (resBtn == QMessageBox::Save)
    {
      save_current_game();
      save_unfinished_game_file();
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

  mWinningPlayer = iPlayerNumber;
  QString name = mSettings.PlayersList.at(iPlayerNumber);
  QString text = name + " has won the game. Congratulations!\n Play again?";
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Game finished!", text,
                                                             QMessageBox::Yes | QMessageBox::Close);
  if (resBtn == QMessageBox::Yes)
  {
    save_current_game();
    start_new_game_with_same_settings();
  }
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
