#include "x01_mainwindow.h"
#include "ui_x01_mainwindow.h"
#include "groupbox_x01.h"
#include "x01_class.h"
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPushButton>
#include <QFile>


CX01MainWindow::CX01MainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler)
  : QMainWindow(iParent)
  , mUi(new Ui::CX01MainWindow)
  , mDartBoard(nullptr)
  , mSettings(iSettings)
  , mGameDataHandler(iGameDataHandler)
  , mNumberOfPlayers(mSettings.PlayersList.size())
  , mTimeStamp(QDateTime::currentDateTime())
{
  mUi->setupUi(this);
  QString text = QString::number(static_cast<uint32_t>(mSettings.Game));
  QWidget::setWindowTitle(text);
#ifndef TESTING
  mDartBoard = new CDartBoardX01(mUi->graphicsViewDartBoard, this, mSettings);
#else
  mDartBoard = new CDartBoardX01();
#endif
  mDartBoard->init_dartboard(static_cast<uint32_t>(mSettings.Game));
  connect_main_window_slots();
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
  display_score(mPlayerBox[mActivePlayer]->get_remaining_points());
  erase_dart1();
  erase_dart2();
  erase_dart3();
  mUi->submitButton->setAutoDefault(true);
}

CX01MainWindow::CX01MainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, QVector<QVector<CX01Class::CPlayerData>> iGameData)
  : CX01MainWindow(iParent, iSettings, iGameDataHandler)
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_game_data(iGameData.at(i));
    if (iGameData.at(i).back().Active) mActivePlayer = i;
  }
  display_score(mPlayerBox[mActivePlayer]->get_remaining_points());
}

CX01MainWindow::~CX01MainWindow()
{
  delete mUi;
  if (mDartBoard) delete mDartBoard;
  for (auto box : mPlayerBox)
  {
    delete box;
  }
}

void CX01MainWindow::add_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox.push_back(new CX01GroupBox(this, mSettings, i, mDartBoard));
    mPlayerBox[i]->setAttribute(Qt::WA_DeleteOnClose);
    mPlayerBox[i]->set_inactive();

    if ((i % 2) == 0) mUi->gridForLeftHandPlayers->addWidget(mPlayerBox[i]);
    else              mUi->gridForRightHandPlayers->addWidget(mPlayerBox[i]);
  }
}

void CX01MainWindow::connect_main_window_slots()
{
  connect(mUi->submitButton, &QPushButton::clicked, this, &CX01MainWindow::submit_button_clicked_slot);
  connect(mUi->undoButton, &QPushButton::clicked, this, &CX01MainWindow::undo_button_clicked_slot);
}

void CX01MainWindow::closeEvent(QCloseEvent * iEvent)
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

void CX01MainWindow::set_active_player(uint32_t iPlayer)
{
  mActivePlayer = iPlayer;
}

void CX01MainWindow::save_current_game()
{
  QVector<QVector<CX01Class::CPlayerData>> gameDataVec;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    gameDataVec.append(mPlayerBox[i]->get_game_data());
  }
  auto gameData = CGameDataHandler::SGameData(mTimeStamp.toString(), game_finished(), mSettings, mWinningPlayer, gameDataVec);
  mGameDataHandler.save_game_to_db(gameData);
}

bool CX01MainWindow::game_finished() const
{
  bool finished = true;
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    finished = finished && mPlayerBox[i]->is_finished();
  }

  return finished;
}

void CX01MainWindow::start_new_game_with_same_settings()
{
  mActivePlayer = mWinningPlayer = 0;
  clear_group_box_widgets();
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
  display_score(mPlayerBox[mActivePlayer]->get_remaining_points());
  erase_dart1();
  erase_dart2();
  erase_dart3();
  mDartBoard->unset_finished();
  mTimeStamp = QDateTime::currentDateTime();
}

void CX01MainWindow::clear_group_box_widgets()
{
  QLayoutItem * item;
  while ((item = mUi->gridForLeftHandPlayers->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }
  while ((item = mUi->gridForRightHandPlayers->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }
  mPlayerBox = {};
}

void CX01MainWindow::inactivate_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_inactive();
  }
}

void CX01MainWindow::handle_update_default()
{
  update_active_player();
  inactivate_all_players();
  mPlayerBox[mActivePlayer]->set_active();
}

void CX01MainWindow::handle_update_leg()
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

void CX01MainWindow::handle_update_set()
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
      mPlayerBox[mActivePlayer]->set_set_begin();
      mPlayerBox[mActivePlayer]->set_leg_begin();
      break;
    }
  }

  for (auto & box: mPlayerBox)
  {
    box->reset_legs();
    box->set_lcd_legs();
  }
}

void CX01MainWindow::update_players(const EUpdateType iType)
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
  mDartBoard->init_dartboard(mPlayerBox[mActivePlayer]->get_remaining_points());
}

void CX01MainWindow::reset_scores_of_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->reset();
    mPlayerBox[i]->display_finishes(mPlayerBox[i]->get_remaining_points(), 3);
  }
}

void CX01MainWindow::set_global_finished()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_finished();
  }
  mDartBoard->set_finished();
}

void CX01MainWindow::handle_game_won(uint32_t iPlayerNumber)
{
  set_global_finished();
  create_snapshots_of_all_players();
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

void CX01MainWindow::unset_leg_begin_for_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->unset_leg_begin();
  }
}

void CX01MainWindow::unset_set_begin_for_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->unset_set_begin();
  }
}

void CX01MainWindow::inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted)
{
  inactivate_all_players();
  set_active_player(iPlayer);

  if (!iLegStarted)
  {
    unset_leg_begin_for_all_players();
    mPlayerBox[iPlayer]->set_leg_begin();
  }

  if (!iSetStarted)
  {
    unset_leg_begin_for_all_players();
    unset_set_begin_for_all_players();
    mPlayerBox[iPlayer]->set_leg_begin();
    mPlayerBox[iPlayer]->set_set_begin();
  }
}

void CX01MainWindow::create_snapshots_of_all_players()
{
  for (uint32_t i = 0; i < mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->create_snapshot();
  }
}

void CX01MainWindow::submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts)
{
  mPlayerBox[mActivePlayer]->submit_score(iScore, iNumberOfDarts, iCheckoutAttempts, iDarts);
}

void CX01MainWindow::submit_button_clicked_slot()
{
  mDartBoard->submit_score();
}

void CX01MainWindow::undo_button_clicked_slot()
{
  mDartBoard->perform_undo();
}

void CX01MainWindow::display_score(uint32_t iScore)
{
  mUi->currentRemainingLCD->display(static_cast<int>(iScore));
}

void CX01MainWindow::display_dart1(uint32_t iVal)
{
  mUi->dart1LCD->display(static_cast<int>(iVal));
}

void CX01MainWindow::display_dart2(uint32_t iVal)
{
  mUi->dart2LCD->display(static_cast<int>(iVal));
}

void CX01MainWindow::display_dart3(uint32_t iVal)
{
  mUi->dart3LCD->display(static_cast<int>(iVal));
}

void CX01MainWindow::erase_dart1()
{
  mUi->dart1LCD->display("--");
}

void CX01MainWindow::erase_dart2()
{
  mUi->dart2LCD->display("--");
}

void CX01MainWindow::erase_dart3()
{
  mUi->dart3LCD->display("--");
}

void CX01MainWindow::update_finishes(uint32_t iScore, uint32_t iNumberOfDarts)
{
  mPlayerBox[mActivePlayer]->display_finishes(iScore, iNumberOfDarts);
}

void CX01MainWindow::set_focus_to_submit_button()
{
  mUi->submitButton->setFocus();
}
