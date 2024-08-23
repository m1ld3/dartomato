#ifndef X01MAINWINDOW_H
#define X01MAINWINDOW_H

#include <QMainWindow>
#include "x01_mainwindow_if.h"
#include "mainwindow_if.h"
#include "groupbox_x01.h"
#include <QSoundEffect>
#include "dartboard_x01.h"
#include "settings.h"
#include "game_data_handler.h"

namespace Ui
{
  class CX01MainWindow;
}

class CX01MainWindow : public QMainWindow, public IX01MainWindow, public IMainWindow
{
  Q_OBJECT

public:

  explicit CX01MainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler);
  explicit CX01MainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const CGameDataHandler::SGameData iGameData);
  ~CX01MainWindow() override;
#ifdef TESTING
  CX01MainWindow(QWidget * iParent = nullptr)
    : QMainWindow(iParent)
  {}
#endif
  void closeEvent(QCloseEvent * iEvent) override;
  void setAttribute(Qt::WidgetAttribute iAttribute, bool iOn = true) override
  {
    QMainWindow::setAttribute(iAttribute, iOn);
  }
  void show() override
  {
    QMainWindow::show();
  }

  // CMainWindowIf
  void update_players(const EUpdateType iType) override;
  void reset_scores_of_all_players() override;
  void activate_player_inactivate_other_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted) override;
  void create_snapshots_of_all_players() override;
  void handle_game_won(uint32_t iPlayerNumber) override;

  // CX01MainWindowIf
  void display_dart1(uint32_t iVal) override;
  void display_dart2(uint32_t iVal) override;
  void display_dart3(uint32_t iVal) override;
  void erase_dart1() override;
  void erase_dart2() override;
  void erase_dart3() override;
  void submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts) override;
  void update_finishes(uint32_t iScore, uint32_t iNumberOfDarts) override;
  void set_focus_to_submit_button() override;
  void display_score(uint32_t iScore) override;

private slots:

  void submit_button_clicked_slot() override;
  void undo_button_clicked_slot() override;

private:

  // CMainWindowIf
  void set_active_player(uint32_t iPlayer) override;
  void add_players() override;
  void save_current_game() override;
  bool game_finished() const override;
  void start_new_game_with_same_settings() override;
  void clear_group_box_widgets() override;
  void set_global_finished() override;
  void handle_update_default() override;
  void handle_update_leg() override;
  void handle_update_set() override;
  void inactivate_all_players() override;
  void unset_leg_begin_for_all_players() override;
  void unset_set_begin_for_all_players() override;

  // CX01MainWindowIf
  void connect_main_window_slots() override;

  Ui::CX01MainWindow * mUi;
  uint32_t mWinningPlayer = 0;
  QVector<CX01GroupBox*> mPlayerBox;
  CDartBoard * mDartBoard;
  const CSettings mSettings;
  CGameDataHandler mGameDataHandler;
};

#endif  // X01MAINWINDOW_H
