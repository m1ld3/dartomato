#pragma once

#include <QDateTime>
#include <QFile>
#include "settings.h"

class CMainWindowIf
{
public:

  virtual void display_dart1(uint32_t iVal) = 0;
  virtual void display_dart2(uint32_t iVal) = 0;
  virtual void display_dart3(uint32_t iVal) = 0;
  virtual void erase_dart1() = 0;
  virtual void erase_dart2() = 0;
  virtual void erase_dart3() = 0;
  virtual void submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  virtual void update_finishes(uint32_t iScore, uint32_t iNumberOfDarts) = 0;
  virtual void set_focus_to_submit_button() = 0;
  virtual void update_players(const EUpdateType iType) = 0;
  virtual void reset_scores_of_all_players() = 0;
  virtual void inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted) = 0;
  virtual void create_snapshots_of_all_players() = 0;
  virtual void handle_game_won(uint32_t iPlayerNumber) = 0;

  bool is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const;
  bool is_score_bigger(uint32_t iScore) const;
  bool is_score_smaller(uint32_t iScore) const;
  void increase_slot_score(const ECricketSlots iSlot, uint32_t iPoints);
  QVector<uint32_t> compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer);
  void set_scores();
  void update_extra_points_labels();
  void update_darts(uint32_t iPlayer);

protected:

  virtual void update_active_player()
  {
    mActivePlayer = (mActivePlayer + 1) % mNumberOfPlayers;
  }
  virtual void save_unfinished_game_file()
  {
    QFile file(OpenGamePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream stream(&file);
      stream << mTimeStamp.toString();
      file.close();
    }
  }

  virtual void set_active_player(uint32_t iPlayer) { mActivePlayer = iPlayer; }
  virtual void add_players() = 0;
  virtual void connect_main_window_slots() = 0;
  virtual void update_player_default() = 0;
  virtual void update_player_leg() = 0;
  virtual void update_player_set() = 0;
  virtual void save_current_game() = 0;
  virtual bool game_finished() const = 0;
  virtual void start_new_game_with_same_settings() = 0;
  virtual void clear_group_box_widgets() = 0;
  virtual void set_global_finished() = 0;

  virtual void handle_update_default() = 0;
  virtual void handle_update_leg() = 0;
  virtual void handle_update_set() = 0;
  virtual void inactivate_all_players() = 0;
  virtual void unset_leg_begin_for_all_players() = 0;
  virtual void unset_set_begin_for_all_players() = 0;

  uint32_t mActivePlayer = 0;
  const uint32_t mNumberOfPlayers = 1;
  QDateTime mTimeStamp;
};
