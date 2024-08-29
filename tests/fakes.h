#pragma once

#include <stdint.h>
#include <QChar>
#include <QSoundEffect>
#include "settings.h"

class CDartBoardFake
{
public:

  virtual void handle_segment_pressed_event(uint32_t iVal, QChar iType) = 0;
  virtual void unset_finished() = 0;
  virtual void init_dartboard(uint32_t iScore) = 0;
  void play_game_shot_sound(){};

  ~CDartBoardFake() = default;

  CSettings mSettings;
  QSoundEffect mBustedSound;
  QSoundEffect mGameShotSound;
};

class CCricketInputFake
{
public:

  bool are_slots_full() { return true; };
};
