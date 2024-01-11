#include "xo1mainwindow.h"
#include "ui_xo1mainwindow.h"
#include "groupbox_player.h"
#include "playerclass.h"
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>


CX01MainWindow::CX01MainWindow(QWidget * iParent, const CSettings & ipSettings)
  : QMainWindow(iParent)
  , mUi(new Ui::CX01MainWindow)
  , mDartBoard(nullptr)
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  QString text = QString::number(static_cast<uint32_t>(mpSettings.mGame));
  QWidget::setWindowTitle(text);
  mDartBoard = new CDartBoardX01(mUi->graphicsViewDartBoard, this, mpSettings);
  mDartBoard->init_dartboard(static_cast<uint32_t>(mpSettings.mGame));
  connect_main_window_slots();
  add_players();
  mPlayerBox[mActivePlayer]->set_set_begin();
  mPlayerBox[mActivePlayer]->set_leg_begin();
  mPlayerBox[mActivePlayer]->set_active();
  display_score(mPlayerBox[mActivePlayer]->get_remaining());
  erase_dart1();
  erase_dart2();
  erase_dart3();
  mUi->submitButton->setAutoDefault(true);
}

CX01MainWindow::~CX01MainWindow()
{
  delete mUi;
  if (mDartBoard) delete mDartBoard;
  for (auto &player : mPlayer)
  {
    delete player;
  }
  for (auto &box : mPlayerBox)
  {
    delete box;
  }
}

void CX01MainWindow::add_players()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayer.push_back(new CX01Class(this, i+1, mpSettings));
    mPlayerBox.push_back(new CX01GroupBox(this, mpSettings, i+1, mPlayer[i], mDartBoard));
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
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Quit game",
                                                             tr("Are you sure you want to quit the game?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                             QMessageBox::No);
  if (resBtn != QMessageBox::Yes) iEvent->ignore();
  else                            iEvent->accept();
}

void CX01MainWindow::set_active_player(uint32_t iPlayer)
{
  mActivePlayer = iPlayer;
}

void CX01MainWindow::update_active_player()
{
  mActivePlayer = (mActivePlayer + 1) % mpSettings.mNumberOfPlayers;
}

void CX01MainWindow::update_player_default()
{
  update_active_player();

  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_inactive();
  }
  mPlayerBox[mActivePlayer]->set_active();
}

void CX01MainWindow::update_player_leg()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mPlayerBox[i]->has_begun_leg())
    {
      mPlayerBox[i]->unset_leg_begin();
      mActivePlayer = i;
      update_active_player();

      for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
      {
        mPlayerBox[i]->set_inactive();
      }
      mPlayerBox[mActivePlayer]->set_active();
      mPlayerBox[mActivePlayer]->set_leg_begin();
      break;
    }
  }
}

void CX01MainWindow::update_player_set()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    if (mPlayerBox[i]->has_begun_set())
    {
      mPlayerBox[i]->unset_set_begin();
      mActivePlayer = i;
      update_active_player();

      for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
      {
        mPlayerBox[i]->set_inactive();
        mPlayerBox[i]->unset_leg_begin();
      }
      mPlayerBox[mActivePlayer]->set_active();
      mPlayerBox[mActivePlayer]->set_set_begin();
      mPlayerBox[mActivePlayer]->set_leg_begin();
      break;
    }
  }

  for (auto &box: mPlayerBox)
  {
    box->reset_legs();
    box->set_lcd_legs();
  }
}

void CX01MainWindow::update_players(const EUpdateType iType)
{
  if (iType == EUpdateType::DEFAULT)
  {
    update_player_default();
  }
  else if (iType == EUpdateType::LEG)
  {
    update_player_leg();
  }
  else if (iType == EUpdateType::SET)
  {
    update_player_set();
  }
  mDartBoard->init_dartboard(mPlayerBox[mActivePlayer]->get_remaining());
}

void CX01MainWindow::reset_scores_of_all_players()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->reset();
    mPlayerBox[i]->display_finishes(mPlayerBox[i]->get_remaining(), 3);
  }
}

void CX01MainWindow::handle_game_won(uint32_t iPlayerNumber)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_finished();
  }
  mDartBoard->set_finished();
  QString name = mPlayerBox[iPlayerNumber]->get_player_number();
  QString text = name + " has won the game. Congratulations!. ";
  QMessageBox::about(this,"Game finished", text);
}

void CX01MainWindow::inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted)
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->set_inactive();
  }
  set_active_player(iPlayer);
  if (!iLegStarted)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      mPlayerBox[i]->unset_leg_begin();
    }
    mPlayerBox[iPlayer]->set_leg_begin();
  }
  if (!iSetStarted)
  {
    for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
    {
      mPlayerBox[i]->unset_set_begin();
      mPlayerBox[i]->unset_leg_begin();
    }
    mPlayerBox[iPlayer]->set_leg_begin();
    mPlayerBox[iPlayer]->set_set_begin();
  }
}

void CX01MainWindow::update_history_of_all_players()
{
  for (uint32_t i = 0; i < mpSettings.mNumberOfPlayers; i++)
  {
    mPlayerBox[i]->update_history();
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
