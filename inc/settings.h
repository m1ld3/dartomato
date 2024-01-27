#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <stdint.h>
#include <QStringList>

//#define USE_TTS

enum class EGame
{
    GAME_301 = 301
  , GAME_501 = 501
  , GAME_701 = 701
  , GAME_CRICKET = 0
};

enum class EUpdateType
{
    DEFAULT = 0
  , LEG     = 1
  , SET     = 2
};

enum class ECricketSlots
{
    SLOT_15  = 0
  , SLOT_16  = 1
  , SLOT_17  = 2
  , SLOT_18  = 3
  , SLOT_19  = 4
  , SLOT_20  = 5
  , SLOT_25  = 6
  , SLOT_MAX = 7
};

struct CSettings
{
  CSettings(EGame iGame, QStringList iPlayers, uint32_t iSets, uint32_t iLegs,
            bool iSingleIn, bool iSingleOut, bool iDoubleIn, bool iDoubleOut,
            bool iMasterIn, bool iMasterOut, bool iCutThroat)
  : Game(iGame)
  , PlayersList(iPlayers)
  , Sets(iSets)
  , Legs(iLegs)
  , SingleIn(iSingleIn)
  , SingleOut(iSingleOut)
  , DoubleIn(iDoubleIn)
  , DoubleOut(iDoubleOut)
  , MasterIn(iMasterIn)
  , MasterOut(iMasterOut)
  , CutThroat(iCutThroat)
  {}

  CSettings(){}

  EGame Game = EGame::GAME_301;
  QStringList PlayersList = {};
  uint32_t Sets = 1;
  uint32_t Legs = 1;
  bool SingleIn = true;
  bool SingleOut = false;
  bool DoubleIn = false;
  bool DoubleOut = true;
  bool MasterIn = false;
  bool MasterOut = false;
  bool CutThroat = false;
};

#endif // CSETTINGS_H
