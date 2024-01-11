#include "groupbox_player.h"
#include "ui_groupbox_player.h"
#include "dialognameinput.h"
#include "statswindow.h"
#include "QPixmap"
#include <sstream>
#include <iomanip>
#include <QMessageBox>
#include <QString>
#include <algorithm>
#include <xo1mainwindow.h>
#include <checkoutlists.h>
#include <string>
#include <QPointer>

CX01GroupBox::CX01GroupBox(QWidget * iParent, const CSettings & ipSettings,
                           uint32_t iPlayerNumber, CX01Class * const iPlayer, CDartBoardX01 * iDartBoard)
  : QGroupBox(iParent)
  , mUi(new Ui::CX01GroupBox)
  , mPlayer(iPlayer)
  , mDartBoard(iDartBoard)
  , mpSettings(ipSettings)
  , mRemaining(static_cast<uint32_t>(mpSettings.mGame))
#ifndef USE_TTS
  , mScoreSound(this)
#endif
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(3);
  mUi->lcdNumber->display(static_cast<int>(mpSettings.mGame));
  QString text = "Player " + QString::number(iPlayerNumber);
  mUi->label_playername->setText(text);
  display_stats_and_finishes();
  mpGameWindow = static_cast<CX01MainWindow*>(iParent);
  connect_slots();
}

CX01GroupBox::~CX01GroupBox()
{
  delete mUi;
}

void CX01GroupBox::connect_slots()
{
  connect(mUi->label_pic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CX01GroupBox::player_active_button_pressed_slot);
  connect(mUi->pushButton_name, &QPushButton::clicked, this, &CX01GroupBox::push_button_name_clicked_slot);
  connect(mUi->pushButton_undo, &QPushButton::clicked, this, &CX01GroupBox::push_button_undo_clicked_slot);
  connect(mUi->pushButton_stats, &QPushButton::clicked, this, &CX01GroupBox::push_button_stats_clicked_slot);
}

void CX01GroupBox::set_active()
{
  mActive = true;
  mUi->label_pic->setPixmap(mPixMap.scaled(80, 80, Qt::KeepAspectRatio));
}

void CX01GroupBox::set_inactive()
{
  mActive = false;
  mUi->label_pic->clear();
}

void CX01GroupBox::reset()
{
  mRemaining = static_cast<uint32_t>(mpSettings.mGame);
  mUi->lcdNumber->display(static_cast<int>(mRemaining));
  mPlayer->reset_score();
}

void CX01GroupBox::set_finished()
{
  mFinished = true;
}

void CX01GroupBox::unset_finished()
{
  mFinished = false;
}

QString CX01GroupBox::get_player_number() const
{
  return mUi->label_playername->text();
}

void CX01GroupBox::push_button_name_clicked_slot()
{
  QPointer<CDialogNameInput> dn = new CDialogNameInput(this, mUi->label_playername->text());
  dn->setAttribute(Qt::WA_DeleteOnClose);
  connect(dn, &CDialogNameInput::signal_ok_button_clicked, this, &CX01GroupBox::ok_button_clicked_slot);
  dn->show();
}

void CX01GroupBox::ok_button_clicked_slot(const QString & iName)
{
  mPlayerName = iName;
  mUi->label_playername->setText(mPlayerName);
}

void CX01GroupBox::display_stats_and_finishes()
{
  QString avg1dart = QString::number(mPlayer->get_avg1dart(), 'f', 3);
  QString avg3dart = QString::number(mPlayer->get_avg3dart(), 'f', 3);
  QString checkout = QString::number(mPlayer->get_checkout(), 'f', 3) + "%";
  mUi->label_1dartInput->setText(avg1dart);
  mUi->label_3dartInput->setText(avg3dart);
  mUi->label_checkoutInput->setText(checkout);
  display_finishes(mRemaining, 3);
}

void CX01GroupBox::play_score_sound()
{
#ifdef USE_TTS
  auto * tts = new QTextToSpeech;
  QLocale locale(QLocale::English, QLocale::UnitedKingdom);
  tts->setLocale(locale);
  tts->setVolume(1.0);
  tts->setRate(0.1);
  tts->say(QString::number(mCurrentScore));
#else
  mScoreSound.play();
#endif
}

void CX01GroupBox::handle_game_shot(uint32_t iCheckoutAttempts)
{
  bool newSet = false;
  mPlayer->update_checkout(iCheckoutAttempts, 1);
  newSet = mPlayer->increment_won_legs_and_check_if_set_won();
  play_score_sound();
  update_history_of_all_players();
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
}

void CX01GroupBox::handle_default_score(uint32_t iCheckoutAttempts)
{
  mPlayer->update_checkout(iCheckoutAttempts, 0);
  play_score_sound();
  mUi->lcdNumber->display(static_cast<int>(mRemaining));
  update_players(EUpdateType::DEFAULT);
}

void CX01GroupBox::submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts)
{
  mCurrentScore = iScore;
#ifndef USE_TTS
  prepare_score_sound();
#endif
  CX01GroupBox::mLegAlreadyStarted = true;
  CX01GroupBox::mSetAlreadyStarted = true;
  mRemaining = mPlayer->set_score(mCurrentScore);
  mPlayer->set_darts(iDarts);
  mPlayer->compute_averages(iNumberOfDarts);

  if (mRemaining == 0) handle_game_shot(iCheckoutAttempts);
  else                 handle_default_score(iCheckoutAttempts);

  display_stats_and_finishes();
}

void CX01GroupBox::player_active_button_pressed_slot()
{
  if (!mActive)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
      inactivate_players(mPlayer->get_player_number(), CX01GroupBox::mLegAlreadyStarted, CX01GroupBox::mSetAlreadyStarted);
      set_active();
      mDartBoard->init_dartboard(mRemaining);
    }
  }
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
  mUi->label_pic->setStyleSheet("#label_pic { border: 5px solid red; }");
}

void CX01GroupBox::unset_leg_begin()
{
  mLegBegin = false;
  mUi->label_pic->setStyleSheet("");
}

bool CX01GroupBox::has_begun_leg() const
{
  return mLegBegin;
}

bool CX01GroupBox::has_begun_set() const
{
  return mSetBegin;
}

void CX01GroupBox::update_history()
{
  mPlayer->update_history();
}

void CX01GroupBox::reset_legs()
{
  mPlayer->reset_legs();
}

const QMap<uint32_t, QVector<QString>> & CX01GroupBox::get_checkout_map(uint32_t iNumberOfDarts)
{
  if (mpSettings.mSingleOut)
  {
    if (iNumberOfDarts == 1)      return singleOutSingleDartCheckoutList;
    else if (iNumberOfDarts == 2) return singleOutTwoDartCheckoutList;
    else                          return singleOutThreeDartCheckoutList;
  }
  else if (mpSettings.mDoubleOut)
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
#ifndef USE_TTS
  std::stringstream ss;
  ss << std::setw(3) << std::setfill('0') << mCurrentScore;
  std::string digits = ss.str();
  std::string strpath = "qrc:/resources/sounds/" + digits + ".wav";
  QString filepath = QString::fromStdString(strpath);
  mScoreSound.setSource(filepath);
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
  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
}

uint32_t CX01GroupBox::get_remaining() const
{
  return mPlayer->get_remaining();
}

void CX01GroupBox::set_lcd_legs_and_sets()
{
  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
}

void CX01GroupBox::perform_undo()
{
  mPlayer->undo();
  mRemaining = mPlayer->get_remaining();
  mUi->lcdNumber->display(static_cast<int>(mRemaining));

  if (mActive) mDartBoard->init_dartboard(mRemaining);

  set_lcd_legs_and_sets();
  display_stats_and_finishes();

  if (mFinished)
  {
    unset_finished();
    mDartBoard->unset_finished();
    }
}

void CX01GroupBox::update_players(const EUpdateType iType)
{
  mpGameWindow->update_players(iType);
}

void CX01GroupBox::reset_scores_of_all_players()
{
  mpGameWindow->reset_scores_of_all_players();
}

void CX01GroupBox::inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted)
{
  mpGameWindow->inactivate_players(iPlayer, iLegStarted, iSetStarted);
}

void CX01GroupBox::update_history_of_all_players()
{
  mpGameWindow->update_history_of_all_players();
}

void CX01GroupBox::push_button_stats_clicked_slot()
{
  QPointer<CStatsWindow> stats = new CStatsWindow(this, mPlayer);
  stats->setAttribute(Qt::WA_DeleteOnClose);
  stats->setModal(true);
  stats->show();
}

void CX01GroupBox::push_button_undo_clicked_slot()
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Undo",
                                                             tr("Are you sure you want to undo your last score?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
  if (resBtn == QMessageBox::Yes) perform_undo();
}

bool CX01GroupBox::mLegAlreadyStarted = false;
bool CX01GroupBox::mSetAlreadyStarted = false;
