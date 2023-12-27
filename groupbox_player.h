#ifndef GROUPBOX_PLAYER_H
#define GROUPBOX_PLAYER_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "dartboard.h"
#include "playerclass.h"
#include <QPixmap>
#include <QSoundEffect>
#include "statswindow.h"

class CX01MainWindow;


constexpr uint32_t GAME_301 = 301;
constexpr uint32_t GAME_501 = 501;
constexpr uint32_t GAME_701 = 701;

namespace Ui
{
  class CX01GroupBox;
}

class CX01GroupBox : public QGroupBox
{
  Q_OBJECT

public:

  explicit CX01GroupBox(QWidget * iParent = nullptr, uint32_t iGame = GAME_501, uint32_t iPlayerNumber = 1, uint32_t iSets = 1, uint32_t iLegs = 1,
                        bool iSingleIn = true, bool iSingleOut = false, bool iDoubleIn = false, bool iDoubleOut = false,
                        bool iMasterIn = false, bool iMasterOut = false, bool iOffensive = false, CPlayerClass * iPlayer = nullptr, CDartBoard * iDartboard = nullptr);
  ~CX01GroupBox() override;
  void set_active();
  void set_inactive();
  void reset();
  void set_finished();
  void unset_finished();
  void close_score_input();
  QString get_player_number() const;
  void set_set_begin();
  void unset_set_begin();
  void set_leg_begin();
  void unset_leg_begin();
  bool has_begun_leg() const;
  bool has_begun_set() const;
  void update_history();
  void reset_legs();
  void display_finishes(uint32_t iRemaining, uint32_t iNumberOfDarts);
  void play_fail_sounds();
  void set_lcd_legs();
  void play_offensive_sounds();
  uint32_t get_remaining() const;
  void submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, QVector<QString> && iDarts);
  double compute_average(QVector<uint32_t> iScoresOfLeg);

signals:

  void signal_update_player(QString iType);
  void signal_reset_scores();
  void signal_inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void signal_update_history();

public slots:

  void on_push_button_name_clicked();
  void ok_button_clicked(QString && iName);
  void signal_player_active_button_pressed();
  void perform_undo();
  void slot_update_leg_history(uint32_t iIndex, CStatsWindow * iStats);

private slots:

  void on_push_button_stats_clicked();
  void on_push_button_undo_clicked();

private:

  Ui::CX01GroupBox * mUi;
  QString mPlayerName;
  CDialogNameInput * mDialogNameInput;
  CPlayerClass * mPlayer;
  CDartBoard * mDartBoard;
  uint32_t mStartVal, mPlayerNumber, mSets, mLegs, mRemaining, mCurrentScore;
  bool mSingleIn, mSingleOut, mDoubleIn, mDoubleOut, mMasterIn, mMasterOut, mOffensive, mActive, mFinished, mSetBegin, mLegBegin;
  QPixmap mPixMap = QPixmap(":/resources/img/darts.svg");
  static bool mLegStarted;
  static bool mSetStarted;
  CX01MainWindow * mGameWindow;
  QSoundEffect mSexy69;
  QSoundEffect mAnotherone;
  QSoundEffect mSound1;
  QSoundEffect mSound2;
  QSoundEffect mSound3;
  QSoundEffect mSound4;
  QSoundEffect mSound5;
  QSoundEffect mSound6;
  QSoundEffect mSound7;
  QSoundEffect mSound8;
  QSoundEffect mSound9;
  QSoundEffect mSound10;
  QSoundEffect mSound11;
  QSoundEffect mScoreSound;
};

#endif  // GROUPBOX_PLAYER_H
