#ifndef X01MAINWINDOW_H
#define X01MAINWINDOW_H

#include <QMainWindow>
#include "groupbox_player.h"
#include <QSoundEffect>
#include "dartboard.h"

namespace Ui
{
  class CX01MainWindow;
}

class CX01MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit CX01MainWindow(QWidget * iParent = nullptr);
  explicit CX01MainWindow(QWidget * iParent = nullptr, uint32_t iNumberOfPlayers = 1, uint32_t iGame = GAME_501, uint32_t iSets = 1, uint32_t iLegs = 1,
                          bool iSingleIn = true, bool iSingleOut = false, bool iDoubleIn = false, bool iDoubleOut = false,
                          bool iMasterIn = false, bool iMasterOut = false, bool iOffensive = false);
  ~CX01MainWindow() override;
  void update_player();
  void closeEvent(QCloseEvent * iEvent) override;
  void set_active_player(uint32_t iPlayer);

private slots:

  void signal_update_player(QString iType);
  void signal_reset_scores();
  void signal_game_won(uint32_t iPlayerNumber);
  void signal_inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void signal_update_history();
  void submit_button_pressed_slot(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, QVector<QString> iDarts);
  void on_submit_button_clicked();
  void on_undo_button_clicked();
  void display_score_slot(uint32_t iScore);
  void display_dart1_slot(uint32_t iVal);
  void display_dart2_Slot(uint32_t iVal);
  void display_dart3_slot(uint32_t iVal);
  void erase_dart1_slot();
  void erase_dart2_slot();
  void erase_dart3_slot();
  void update_finishes_slot(uint32_t iScore, uint32_t iNumberOfDarts);
  void set_focus_to_submit_button_slot();

private:

  Ui::CX01MainWindow * mUi;
  CX01GroupBox * mX01GroupBox;
  uint32_t mNumberOfPlayers, mGame, mSets, mLegs, mActivePlayer;
  bool mSingleIn, mSingleOut, mDoubleIn, mDoubleOut, mMasterIn, mMasterOut, mOffensive;
  QVector<CX01GroupBox*> mPlayerBox;
  QVector<CPlayerClass*> mPlayer;
  CDartBoard * mDartBoard;
};

#endif  // X01MAINWINDOW_H
