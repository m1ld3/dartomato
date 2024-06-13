#include "groupbox_x01.h"
#include "ui_groupbox_x01.h"
#include "stats_window_x01.h"
#include "QPixmap"
#include <sstream>
#include <iomanip>
#ifndef TESTING
#include <QMessageBox>
#endif
#include <QString>
#include <algorithm>
#include <x01_mainwindow.h>
#include <checkout_lists.h>
#include <string>
#include <QPointer>
#include "player_active_button.h"

#ifndef TESTING
CX01GroupBox::CX01GroupBox(QWidget * iParent, const CSettings iSettings,
                           uint32_t iPlayerNumber, CDartBoard * iDartBoard)
  : QGroupBox(iParent)
  , mUi(new Ui::CX01GroupBox)
  , mPlayer(iPlayerNumber, iSettings)
  , mDartBoard(iDartBoard)
  , mSettings(iSettings)
  , mPlayerName(mSettings.PlayersList.at(iPlayerNumber))
  , mPlayerNumber(iPlayerNumber)
  , mRemainingPoints(static_cast<uint32_t>(mSettings.Game))
  , mGameWindow(static_cast<CX01MainWindow*>(iParent))
  , mScoreSound(this)
  , mHistory({mPlayer.create_snapshot()})
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(3);
  mUi->lcdNumber->display(static_cast<int>(mSettings.Game));
  mUi->labelPlayerName->setText(mPlayerName);
  display_stats_and_finishes();
  connect_slots();
}

CX01GroupBox::~CX01GroupBox()
{
  delete mUi;
}

void CX01GroupBox::connect_slots()
{
  connect(mUi->labelPic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CX01GroupBox::player_active_button_pressed_slot);
  connect(mUi->pushButtonUndo, &QPushButton::clicked, this, &CX01GroupBox::push_button_undo_clicked_slot);
  connect(mUi->pushButtonStats, &QPushButton::clicked, this, &CX01GroupBox::push_button_stats_clicked_slot);
}

#else
CX01GroupBox::CX01GroupBox(IMainWindow * iMainWindow, const CSettings iSettings,
                           uint32_t iPlayerNumber, CDartBoard * iDartBoard)
  : mPlayer(iPlayerNumber, iSettings)
  , mDartBoard(iDartBoard)
  , mSettings(iSettings)
  , mPlayerNumber(iPlayerNumber)
  , mRemainingPoints(static_cast<uint32_t>(mSettings.Game))
  , mGameWindow(iMainWindow)
  , mHistory({mPlayer.create_snapshot()})
{}
#endif

void CX01GroupBox::set_active()
{
  mActive = true;
#ifndef TESTING
  mUi->labelPic->setPixmap(mPixMapHand.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#endif
}

void CX01GroupBox::set_inactive()
{
  mActive = false;
  mUi->labelPic->clear();
}

void CX01GroupBox::reset()
{
  mRemainingPoints = static_cast<uint32_t>(mSettings.Game);
  mUi->lcdNumber->display(static_cast<int>(mRemainingPoints));
  mPlayer.reset_score();
}

void CX01GroupBox::set_finished()
{
  mFinished = true;
}

void CX01GroupBox::unset_finished()
{
  mFinished = false;
  mDartBoard->unset_finished();
}

void CX01GroupBox::display_stats_and_finishes()
{
#ifndef TESTING
  QString avg1dart = QString::number(mPlayer.get_avg1dart(), 'f', 2);
  QString avg3dart = QString::number(mPlayer.get_avg3dart(), 'f', 2);
  QString checkout = QString::number(mPlayer.get_checkout(), 'f', 2) + "%";
  mUi->label1DartAvgInput->setText(avg1dart);
  mUi->label3DartAvgInput->setText(avg3dart);
  mUi->labelCheckoutInput->setText(checkout);
  display_finishes(mRemainingPoints, 3);
#endif
}

void CX01GroupBox::play_score_sound()
{
#ifndef TESTING
  mScoreSound.play();
#endif
}

void CX01GroupBox::handle_game_shot(uint32_t iCheckoutAttempts)
{
  bool newSet = false;
  mPlayer.update_checkout(iCheckoutAttempts, 1);
  newSet = mPlayer.increment_won_legs_and_check_if_set_won();
  play_score_sound();
  reset_scores_of_all_players();
  CX01GroupBox::mLegAlreadyStarted = false;

  if (newSet)
  {
    update_players(EUpdateType::SET);
    CX01GroupBox::mSetAlreadyStarted = false;
  }
  else
  {
    update_players(EUpdateType::LEG);
  }
  set_lcd_legs_and_sets();
  display_stats_and_finishes();

  if (mPlayer.has_won_game())
  {
    mGameWindow->handle_game_won(mPlayerNumber);
  }
  else
  {
    create_snapshots_of_all_players();
  }
}

void CX01GroupBox::handle_default_score(uint32_t iCheckoutAttempts)
{
  mPlayer.update_checkout(iCheckoutAttempts, 0);
  play_score_sound();
#ifndef TESTING
  mUi->lcdNumber->display(static_cast<int>(mRemainingPoints));
#endif
  update_players(EUpdateType::DEFAULT);
}

void CX01GroupBox::submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts)
{
  mCurrentScore = iScore;
  prepare_score_sound();
  CX01GroupBox::mLegAlreadyStarted = true;
  CX01GroupBox::mSetAlreadyStarted = true;
  mRemainingPoints = mPlayer.set_score(mCurrentScore);
  mPlayer.set_darts(iDarts);
  mPlayer.compute_averages(iNumberOfDarts);

  if (mRemainingPoints == 0)
  {
    handle_game_shot(iCheckoutAttempts);
  }
  else
  {
    handle_default_score(iCheckoutAttempts);
    create_snapshot();
    display_stats_and_finishes();
  }
}

void CX01GroupBox::create_snapshot()
{
  auto snap = mPlayer.create_snapshot();
  snap.Active = mActive;
  snap.Finished = mFinished;
  mHistory.push_back(snap);
}

void CX01GroupBox::set_game_data(QVector<CX01Class::CPlayerData> iGameData)
{
  mHistory = iGameData;
  mActive = iGameData.back().Active;
  mPlayer.restore_state(mHistory.back());
  mRemainingPoints = mPlayer.get_remaining();
#ifndef TESTING
  mUi->lcdNumber->display(static_cast<int>(mRemainingPoints));
#endif
  if (mActive) mDartBoard->init_dartboard(mRemainingPoints);
  set_lcd_legs_and_sets();
  display_stats_and_finishes();
}

void CX01GroupBox::create_snapshots_of_all_players()
{
  mGameWindow->create_snapshots_of_all_players();
}

void CX01GroupBox::player_active_button_pressed_slot()
{
#ifndef TESTING
  if (!mActive)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
      inactivate_players(mPlayer.get_player_number(), CX01GroupBox::mLegAlreadyStarted, CX01GroupBox::mSetAlreadyStarted);
      set_active();
      mDartBoard->init_dartboard(mRemainingPoints);
    }
  }
#endif
}

void CX01GroupBox::set_set_begin()
{
  mSetBegin = true;
}

void CX01GroupBox::unset_set_begin()
{
  mSetBegin = false;
}

void CX01GroupBox::set_leg_begin()
{
  mLegBegin = true;
#ifndef TESTING
  mUi->labelLegBegin->setPixmap(mPixMapDot.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
#endif
}

void CX01GroupBox::unset_leg_begin()
{
  mLegBegin = false;
  mUi->labelLegBegin->clear();
}

bool CX01GroupBox::has_begun_leg() const
{
  return mLegBegin;
}

bool CX01GroupBox::has_begun_set() const
{
  return mSetBegin;
}

void CX01GroupBox::reset_legs()
{
  mPlayer.reset_legs();
}

const QMap<uint32_t, QVector<QString>> & CX01GroupBox::get_checkout_map(uint32_t iNumberOfDarts)
{
  if (mSettings.OutMode == EX01OutMode::SINGLE_OUT)
  {
    if (iNumberOfDarts == 1)      return singleOutSingleDartCheckoutList;
    else if (iNumberOfDarts == 2) return singleOutTwoDartCheckoutList;
    else                          return singleOutThreeDartCheckoutList;
  }
  else if (mSettings.OutMode == EX01OutMode::DOUBLE_OUT)
  {
    if (iNumberOfDarts == 1)      return doubleOutSingleDartCheckoutList;
    else if (iNumberOfDarts == 2) return doubleOutTwoDartCheckoutList;
    else                          return doubleOutThreeDartCheckoutList;
  }
  else
  {
    if (iNumberOfDarts == 1)      return masterOutSingleDartCheckoutList;
    else if (iNumberOfDarts == 2) return masterOutTwoDartCheckoutList;
    else                          return masterOutThreeDartCheckoutList;
  }
}

void CX01GroupBox::prepare_score_sound()
{
  std::stringstream ss;
  ss << std::setw(3) << std::setfill('0') << mCurrentScore;
  std::string digits = ss.str();
  std::string strpath = "qrc:/resources/sounds/" + digits + ".wav";
  mSoundPath = QString::fromStdString(strpath);
#ifndef TESTING
  mScoreSound.setSource(mSoundPath);
#endif
}

void CX01GroupBox::display_finishes(uint32_t iRemaining, uint32_t iNumberOfDarts)
{
  mUi->textBrowser->clear();
  mUi->textBrowser->setText("Checkouts:");
  if (iNumberOfDarts == 0) return;

  for (uint32_t darts = 1; darts <= iNumberOfDarts; darts++)
  {
    const QMap<uint32_t, QVector<QString>> checkoutMap = get_checkout_map(darts);

    if (checkoutMap.find(iRemaining) != checkoutMap.end())
    {
      QVector<QString> checkouts = checkoutMap.find(iRemaining).value();
      for (auto & checkout : checkouts)
      {
        mUi->textBrowser->append(checkout);
      }
    }
  }

  QTextCursor cursor = mUi->textBrowser->textCursor();
  cursor.setPosition(0);
  mUi->textBrowser->setTextCursor(cursor);
}

void CX01GroupBox::set_lcd_legs()
{
  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer.get_legs()));
}

uint32_t CX01GroupBox::get_remaining_points() const
{
  return mPlayer.get_remaining();
}

void CX01GroupBox::set_lcd_legs_and_sets()
{
#ifndef TESTING
  mUi->lcdNumberLegs->display(static_cast<int>(mPlayer.get_legs()));
  mUi->lcdNumberSets->display(static_cast<int>(mPlayer.get_sets()));
#endif
}

void CX01GroupBox::perform_undo()
{
  if (mHistory.size() > 1)
  {
    mHistory.pop_back();
    mPlayer.restore_state(mHistory.back());
  }
  mRemainingPoints = mPlayer.get_remaining();
#ifndef TESTING
  mUi->lcdNumber->display(static_cast<int>(mRemainingPoints));
#endif
  if (mActive) mDartBoard->init_dartboard(mRemainingPoints);

  set_lcd_legs_and_sets();
  display_stats_and_finishes();

  if (mFinished)
  {
    unset_finished();
  }
}

void CX01GroupBox::update_players(const EUpdateType iType)
{
  mGameWindow->update_players(iType);
}

void CX01GroupBox::reset_scores_of_all_players()
{
  mGameWindow->reset_scores_of_all_players();
}

void CX01GroupBox::inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted)
{
  mGameWindow->inactivate_players(iPlayer, iLegStarted, iSetStarted);
}

void CX01GroupBox::push_button_stats_clicked_slot()
{
#ifndef TESTING
  auto stats = IStatsWindow::create(mHistory.back(), this);
  stats->setAttribute(Qt::WA_DeleteOnClose);
  stats->setModal(true);
  stats->show();
#endif
}

void CX01GroupBox::push_button_undo_clicked_slot()
{
#ifndef TESTING
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Undo",
                                                             tr("Are you sure you want to undo your last score?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
  if (resBtn == QMessageBox::Yes) perform_undo();
#endif
}

bool CX01GroupBox::mLegAlreadyStarted = false;
bool CX01GroupBox::mSetAlreadyStarted = false;
