#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <stdint.h>
#include <QStringList>
#include <map>

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

enum class EX01InMode
{
    SINGLE_IN = 0
  , DOUBLE_IN = 1
  , MASTER_IN = 2
};

enum class EX01OutMode
{
    SINGLE_OUT = 0
  , DOUBLE_OUT = 1
  , MASTER_OUT = 2
};

static std::map<EX01InMode, QString> MapX01InMode2Str
{
  {EX01InMode::SINGLE_IN, "Single In"},
  {EX01InMode::DOUBLE_IN, "Double In"},
  {EX01InMode::MASTER_IN, "Master In"},
};

static std::map<EX01OutMode, QString> MapX01OutMode2Str
{
  {EX01OutMode::SINGLE_OUT, "Single Out"},
  {EX01OutMode::DOUBLE_OUT, "Double Out"},
  {EX01OutMode::MASTER_OUT, "Master Out"},
};

struct CSettings
{
  CSettings(EGame iGame, QStringList iPlayers, uint32_t iSets, uint32_t iLegs,
            EX01InMode iInMode, EX01OutMode iOutMode, bool iCutThroat)
  : Game(iGame)
  , PlayersList(iPlayers)
  , Sets(iSets)
  , Legs(iLegs)
  , InMode(iInMode)
  , OutMode(iOutMode)
  , CutThroat(iCutThroat)
  {}

  CSettings() = default;

  bool operator==(const CSettings & iOther) const
  {
    return Game == iOther.Game &&
           PlayersList == iOther.PlayersList &&
           Sets == iOther.Sets &&
           Legs == iOther.Legs &&
           InMode == iOther.InMode &&
           OutMode == iOther.OutMode &&
           CutThroat == iOther.CutThroat;
  }

  EGame Game = EGame::GAME_301;
  QStringList PlayersList = {};
  uint32_t Sets = 1;
  uint32_t Legs = 1;
  EX01InMode InMode = EX01InMode::SINGLE_IN;
  EX01OutMode OutMode = EX01OutMode::DOUBLE_OUT;
  bool CutThroat = false;
};

static const QString OpenGamePath = "open_game.txt";

#endif  // CSETTINGS_H
