#ifndef CRICKET_MAINWINDOW_H
#define CRICKET_MAINWINDOW_H

#include <QMainWindow>
#include "groupbox_cricket.h"
#ifndef USE_TTS
#include <QSoundEffect>
#endif
#include <QGridLayout>
#include "game_data_handler.h"
#include <QDateTime>

class CCricketGroupBox;

namespace Ui
{
  class CCricketMainWindow;
}

class CCricketMainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit CCricketMainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler);
  explicit CCricketMainWindow(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const QVector<QVector<CCricketClass::CPlayerData>> iGameData);
  ~CCricketMainWindow() override;
  void closeEvent(QCloseEvent * iEvent) override;
  bool is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const;
  bool is_score_bigger(uint32_t iScore) const;
  bool is_score_smaller(uint32_t iScore) const;
  void increase_slot_score(const ECricketSlots iSlot, uint32_t iPoints);
  QVector<uint32_t> compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer);
  void set_scores();
  void update_extra_points_labels();
  void update_darts(uint32_t iPlayer);
  void update_players(const EUpdateType iType);
  void reset_scores_of_all_players();
  void handle_game_won(uint32_t iPlayerNumber);
  void inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void create_snapshots_of_all_players();

private:

  void update_active_player();
  void handle_update_default();
  void handle_update_leg();
  void handle_update_set();
  void inactivate_all_players();
  void unset_leg_begin_for_all_players();
  void unset_set_begin_for_all_players();
  void set_active_player(uint32_t iPlayer);
  void add_players();
  void save_current_game();
  bool game_finished() const;
  void start_new_game_with_same_settings();
  void clear_group_box_widgets();
  void save_unfinished_game_file();
  void set_global_finished();

  Ui::CCricketMainWindow * mUi;
  uint32_t mActivePlayer = 0;
  uint32_t mWinningPlayer = 0;
  QVector<CCricketGroupBox*> mPlayerBox;
  const CSettings mSettings;
  CGameDataHandler & mGameDataHandler;
  const uint32_t mNumberOfPlayers = 1;
  QDateTime mTimeStamp;
};

#endif  // CRICKET_MAINWINDOW_H
