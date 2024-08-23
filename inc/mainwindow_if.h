#pragma once

#include <QDateTime>
#include <QFile>
#include "settings.h"
#include "game_data_handler.h"

class IMainWindow
{
public:

  IMainWindow(uint32_t iNumberOfPlayers)
    : mNumberOfPlayers(iNumberOfPlayers)
    , mTimeStamp(QDateTime::currentDateTime())
  {}

#ifdef TESTING
  IMainWindow()
    : mNumberOfPlayers(0)
  {}
#endif

  virtual ~IMainWindow() = default;

  static IMainWindow * create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler);
  static IMainWindow * create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const CGameDataHandler::SGameData iGameData);

  virtual void update_players(const EUpdateType iType) = 0;
  virtual void reset_scores_of_all_players() = 0;
  virtual void activate_player_inactivate_other_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted) = 0;
  virtual void create_snapshots_of_all_players() = 0;
  virtual void handle_game_won(uint32_t iPlayerNumber) = 0;

  virtual void setAttribute(Qt::WidgetAttribute iAttribute, bool iOn = true) = 0;
  virtual void show() = 0;

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
  const uint32_t mNumberOfPlayers;
  QDateTime mTimeStamp;
};

//Q_DECLARE_INTERFACE(IMainWindow, "com.IMainWindow")

