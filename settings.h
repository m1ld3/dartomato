#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <stdint.h>

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

struct CSettings
{
  CSettings(EGame iGame, uint32_t iPlayers, uint32_t iSets, uint32_t iLegs,
            bool iSingleIn, bool iSingleOut, bool iDoubleIn, bool iDoubleOut,
            bool iMasterIn, bool iMasterOut, bool iCutThroat, bool iOffensive)
  : mGame(iGame)
  , mNumberOfPlayers(iPlayers)
  , mSets(iSets)
  , mLegs(iLegs)
  , mSingleIn(iSingleIn)
  , mSingleOut(iSingleOut)
  , mDoubleIn(iDoubleIn)
  , mDoubleOut(iDoubleOut)
  , mMasterIn(iMasterIn)
  , mMasterOut(iMasterOut)
  , mCutThroat(iCutThroat)
  , mOffensive(iOffensive)
  {}

  CSettings(){}

  EGame mGame = EGame::GAME_301;
  uint32_t mNumberOfPlayers = 1;
  uint32_t mSets = 1;
  uint32_t mLegs = 1;
  bool mSingleIn = true;
  bool mSingleOut = false;
  bool mDoubleIn = false;
  bool mDoubleOut = true;
  bool mMasterIn = false;
  bool mMasterOut = false;
  bool mCutThroat = false;
  bool mOffensive = false;
};

#endif // CSETTINGS_H
