#pragma once
#include <stdint.h>
#include <QChar>
#include "settings.h"
#include <QSoundEffect>
#include "x01_mainwindow.h"

class CSoundEffectMock : public QSoundEffect
{
};

//class CX01MainWindowMock : public CX01MainWindow
//{

//};

class CDartBoardFake
{
public:

  virtual void handle_segment_pressed_event(uint32_t iVal, QChar & iType) = 0;
  void play_game_shot_sound(){};

  ~CDartBoardFake() = default;

  CSettings mSettings;
  CSoundEffectMock mBustedSound;
  CSoundEffectMock mGameShotSound;
};
