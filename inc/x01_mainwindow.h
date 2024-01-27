#ifndef X01MAINWINDOW_H
#define X01MAINWINDOW_H

#include <QMainWindow>
#include "groupbox_x01.h"
#ifndef USE_TTS
#include <QSoundEffect>
#endif
#include "dartboard_x01.h"
#include "settings.h"
#include "game_data_handler.h"

namespace Ui
{
  class CX01MainWindow;
}

class CX01MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit CX01MainWindow(QWidget * iParent, const CSettings & iSettings, CGameDataHandler & iGameDataHandler);
  ~CX01MainWindow() override;
  void closeEvent(QCloseEvent * iEvent) override;
  void handle_game_won(uint32_t iPlayerNumber);
  void display_score(uint32_t iScore);
  void display_dart1(uint32_t iVal);
  void display_dart2(uint32_t iVal);
  void display_dart3(uint32_t iVal);
  void erase_dart1();
  void erase_dart2();
  void erase_dart3();
  void submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  void update_finishes(uint32_t iScore, uint32_t iNumberOfDarts);
  void set_focus_to_submit_button();
  void update_players(const EUpdateType iType);
  void reset_scores_of_all_players();
  void inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void update_history_of_all_players();
  void create_snapshots_of_all_players();

private slots:

  void submit_button_clicked_slot();
  void undo_button_clicked_slot();

private:

  void update_active_player();
  void add_players();
  void connect_main_window_slots();
  void update_player_default();
  void update_player_leg();
  void update_player_set();
  void set_active_player(uint32_t iPlayer);
  void save_current_game();

  Ui::CX01MainWindow * mUi;
  CX01GroupBox * mX01GroupBox;
  uint32_t mActivePlayer = 0;
  QVector<CX01GroupBox*> mPlayerBox;
  CDartBoardX01 * mDartBoard;
  const CSettings & mSettings;
  CGameDataHandler & mGameDataHandler;
  const uint32_t mNumberOfPlayers = 1;
};

#endif  // X01MAINWINDOW_H
