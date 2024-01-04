#ifndef X01MAINWINDOW_H
#define X01MAINWINDOW_H

#include <QMainWindow>
#include "groupbox_player.h"
#include <QSoundEffect>
#include "dartboard.h"
#include "settings.h"

namespace Ui
{
  class CX01MainWindow;
}

class CX01MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit CX01MainWindow(QWidget * iParent, const CSettings & ipSettings);
  ~CX01MainWindow() override;
  void update_player();
  void closeEvent(QCloseEvent * iEvent) override;
  void set_active_player(uint32_t iPlayer);

private slots:

  void update_player_slot(const EUpdateType iType);
  void reset_scores_slot();
  void game_won_slot(uint32_t iPlayerNumber);
  void inactivate_players_slot(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void update_history_slot();
  void submit_button_pressed_slot(uint32_t iScore, uint32_t iNumberOfDarts,
                                  uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  void submit_button_clicked_slot();
  void undo_button_clicked_slot();
  void display_score_slot(uint32_t iScore);
  void display_dart1_slot(uint32_t iVal);
  void display_dart2_slot(uint32_t iVal);
  void display_dart3_slot(uint32_t iVal);
  void erase_dart1_slot();
  void erase_dart2_slot();
  void erase_dart3_slot();
  void update_finishes_slot(uint32_t iScore, uint32_t iNumberOfDarts);
  void set_focus_to_submit_button_slot();

private:

  void add_players();
  void connect_main_window_slots();

  Ui::CX01MainWindow * mUi;
  CX01GroupBox * mX01GroupBox;
  uint32_t mActivePlayer = 0;
  QVector<CX01GroupBox*> mPlayerBox;
  QVector<CX01Class*> mPlayer;
  CDartBoardX01 * mDartBoard;
  const CSettings & mpSettings;
  void update__player_default();
  void update_player_leg();
  void update_player_set();
};

#endif  // X01MAINWINDOW_H
