#ifndef CRICKET_MAINWINDOW_H
#define CRICKET_MAINWINDOW_H

#include <QMainWindow>
#include "cricket_mainwindow_if.h"
#include "mainwindow_if.h"
#include "groupbox_cricket.h"
#include <QSoundEffect>
#include <QGridLayout>
#include "game_data_handler.h"

class CCricketGroupBox;

namespace Ui
{
  class CCricketMainWindow;
}

class CCricketMainWindow : public QMainWindow, public ICricketMainWindow, public IMainWindow
{
public:

  explicit CCricketMainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler);
  explicit CCricketMainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const CGameDataHandler::SGameData);
  ~CCricketMainWindow() override;
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

  // CCricketMainWindowIf
  bool is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const override;
  bool is_score_bigger(uint32_t iScore) const override;
  bool is_score_smaller(uint32_t iScore) const override;
  void increase_slot_score(const ECricketSlots iSlot, uint32_t iPoints) override;
  QVector<uint32_t> compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer) override;
  void set_scores() override;
  void update_extra_points_labels() override;
  void update_darts(uint32_t iPlayer) override;

private:

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

  Ui::CCricketMainWindow * mUi;
  uint32_t mWinningPlayer = 0;
  QVector<CCricketGroupBox*> mPlayerBox;
  const CSettings mSettings;
  CGameDataHandler & mGameDataHandler;
};

#endif  // CRICKET_MAINWINDOW_H
