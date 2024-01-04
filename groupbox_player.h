#ifndef GROUPBOX_PLAYER_H
#define GROUPBOX_PLAYER_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "dartboard.h"
#include "playerclass.h"
#include <QPixmap>
#include <QSoundEffect>
#include "statswindow.h"
#include "settings.h"

class CX01MainWindow;

namespace Ui
{
  class CX01GroupBox;
}

class CX01GroupBox : public QGroupBox
{
  Q_OBJECT

public:

  explicit CX01GroupBox(QWidget * iParent, const CSettings & ipSettings,
                        uint32_t iPlayerNumber = 1, CX01Class * iPlayer = nullptr,
                        CDartBoardX01 * iDartboard = nullptr);
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
  void set_lcd_legs();
  uint32_t get_remaining() const;
  void submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  double compute_average(QVector<uint32_t> iScoresOfLeg);
  void perform_undo();

signals:

  void signal_update_player(const EUpdateType iType);
  void signal_reset_scores();
  void signal_inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void signal_update_history();

public slots:

  void push_button_name_clicked_slot();
  void ok_button_clicked_slot(const QString & iName);
  void player_active_button_pressed_slot();
  void update_leg_history_slot(uint32_t iIndex, CStatsWindow * iStats);

private slots:

  void push_button_stats_clicked_slot();
  void push_button_undo_clicked_slot();

private:

  Ui::CX01GroupBox * mUi;
  QString mPlayerName;
  CDialogNameInput * mDialogNameInput;
  CX01Class * mPlayer;
  CDartBoardX01 * mDartBoard;
  uint32_t mPlayerNumber, mRemaining;
  uint32_t mCurrentScore = 0;
  bool mActive = false;
  bool mFinished = false;
  bool mSetBegin = false;
  bool mLegBegin = false;
  QPixmap mPixMap = QPixmap(":/resources/img/darts.svg");
  static bool mLegStarted;
  static bool mSetStarted;
  CX01MainWindow * mGameWindow;
  QSoundEffect mScoreSound;
  const CSettings & mpSettings;
};

#endif  // GROUPBOX_PLAYER_H
