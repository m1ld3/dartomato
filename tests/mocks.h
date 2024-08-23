#pragma once

#include <gmock/gmock.h>
#include "x01_mainwindow_if.h"
#include "cricket_mainwindow_if.h"
#include "mainwindow_if.h"
#include "dartboard_base.h"

class CX01MainWindowMock : public IX01MainWindow, public IMainWindow
{
public:

  CX01MainWindowMock()
  {}

  MOCK_METHOD(void, display_dart1, (uint32_t), (override));
  MOCK_METHOD(void, display_dart2, (uint32_t), (override));
  MOCK_METHOD(void, display_dart3, (uint32_t), (override));
  MOCK_METHOD(void, erase_dart1, (), (override));
  MOCK_METHOD(void, erase_dart2, (), (override));
  MOCK_METHOD(void, erase_dart3, (), (override));
  MOCK_METHOD(void, submit_score_to_player, (uint32_t, uint32_t, uint32_t, const QVector<QString>&), (override));
  MOCK_METHOD(void, update_finishes, (uint32_t, uint32_t), (override));
  MOCK_METHOD(void, set_focus_to_submit_button, (), (override));
  MOCK_METHOD(void, display_score, (uint32_t), (override));
  MOCK_METHOD(void, connect_main_window_slots, (), (override));
  MOCK_METHOD(void, submit_button_clicked_slot, (), (override));
  MOCK_METHOD(void, undo_button_clicked_slot, (), (override));

  MOCK_METHOD(void, update_players, (const EUpdateType), (override));
  MOCK_METHOD(void, reset_scores_of_all_players, (), (override));
  MOCK_METHOD(void, activate_player_inactivate_other_players, (uint32_t, bool, bool), (override));
  MOCK_METHOD(void, create_snapshots_of_all_players, (), (override));
  MOCK_METHOD(void, handle_game_won, (uint32_t), (override));
  MOCK_METHOD(void, setAttribute, (Qt::WidgetAttribute, bool), (override));
  MOCK_METHOD(void, show, (), (override));
  MOCK_METHOD(void, set_active_player, (uint32_t), (override));
  MOCK_METHOD(void, add_players, (), (override));
  MOCK_METHOD(void, save_current_game, (), (override));
  MOCK_METHOD(bool, game_finished, (), (const, override));
  MOCK_METHOD(void, start_new_game_with_same_settings, (), (override));
  MOCK_METHOD(void, clear_group_box_widgets, (), (override));
  MOCK_METHOD(void, set_global_finished, (), (override));
  MOCK_METHOD(void, handle_update_default, (), (override));
  MOCK_METHOD(void, handle_update_leg, (), (override));
  MOCK_METHOD(void, handle_update_set, (), (override));
  MOCK_METHOD(void, inactivate_all_players, (), (override));
  MOCK_METHOD(void, unset_leg_begin_for_all_players, (), (override));
  MOCK_METHOD(void, unset_set_begin_for_all_players, (), (override));
};

class CCricketMainWindowMock : public ICricketMainWindow, public IMainWindow
{
public:

  CCricketMainWindowMock()
  {}

  MOCK_METHOD(bool, is_slot_free, (const ECricketSlots, uint32_t), (const, override));
  MOCK_METHOD(bool, is_score_bigger, (uint32_t), (const, override));
  MOCK_METHOD(bool, is_score_smaller, (uint32_t), (const, override));
  MOCK_METHOD(void, increase_slot_score, (const ECricketSlots, uint32_t), (override));
  MOCK_METHOD(QVector<uint32_t>, compute_extra_points, (const ECricketSlots, uint32_t, uint32_t), (override));
  MOCK_METHOD(void, set_scores, (), (override));
  MOCK_METHOD(void, update_extra_points_labels, (), (override));
  MOCK_METHOD(void, update_darts, (uint32_t), (override));

  MOCK_METHOD(void, update_players, (const EUpdateType), (override));
  MOCK_METHOD(void, reset_scores_of_all_players, (), (override));
  MOCK_METHOD(void, activate_player_inactivate_other_players, (uint32_t, bool, bool), (override));
  MOCK_METHOD(void, create_snapshots_of_all_players, (), (override));
  MOCK_METHOD(void, handle_game_won, (uint32_t), (override));
  MOCK_METHOD(void, setAttribute, (Qt::WidgetAttribute, bool), (override));
  MOCK_METHOD(void, show, (), (override));
  MOCK_METHOD(void, set_active_player, (uint32_t), (override));
  MOCK_METHOD(void, add_players, (), (override));
  MOCK_METHOD(void, save_current_game, (), (override));
  MOCK_METHOD(bool, game_finished, (), (const, override));
  MOCK_METHOD(void, start_new_game_with_same_settings, (), (override));
  MOCK_METHOD(void, clear_group_box_widgets, (), (override));
  MOCK_METHOD(void, set_global_finished, (), (override));
  MOCK_METHOD(void, handle_update_default, (), (override));
  MOCK_METHOD(void, handle_update_leg, (), (override));
  MOCK_METHOD(void, handle_update_set, (), (override));
  MOCK_METHOD(void, inactivate_all_players, (), (override));
  MOCK_METHOD(void, unset_leg_begin_for_all_players, (), (override));
  MOCK_METHOD(void, unset_set_begin_for_all_players, (), (override));
};

class CDartBoardMock : public CDartBoard
{
  friend class CDartBoardX01Test;

public:

  CDartBoardMock()
    : CDartBoard(CSettings())
  {}

  MOCK_METHOD(void, handle_segment_pressed_event, (uint32_t, QChar), (override));
  MOCK_METHOD(void, set_finished, (), (override));
  MOCK_METHOD(void, unset_finished, (), (override));
  MOCK_METHOD(void, perform_undo, (), (override));
  MOCK_METHOD(void, submit_score, (), (override));
  MOCK_METHOD(void, init_dartboard, (uint32_t), (override));
  MOCK_METHOD(void, play_game_shot_sound, (), (override));
};
