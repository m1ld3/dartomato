#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include <utility>

#include "x01_class.h"
#include "cricket_class.h"

class QJsonArray;
class QJsonObject;

class CGameDataHandler
{
  friend class CGameDataHandlerTest;

public:

  struct SGameData
  {
    SGameData(QString  iTimeStamp,
              const bool iFinished,
              CSettings  iSettings,
              const uint32_t iWinnerIdx,
              QVector<QVector<CX01Class::CPlayerData>> iGameDataX01 = {},
              QVector<QVector<CCricketClass::CPlayerData>> iGameDataCricket = {}
              )
      : TimeStamp(std::move(iTimeStamp))
      , Finished(iFinished)
      , Settings(std::move(iSettings))
      , WinnerIdx(iWinnerIdx)
      , GameDataX01(std::move(iGameDataX01))
      , GameDataCricket(std::move(iGameDataCricket))
    {}

    SGameData() = default;

    bool operator==(const SGameData & iOther) const
    {
      return TimeStamp == iOther.TimeStamp &&
             Finished == iOther.Finished &&
             Settings == iOther.Settings &&
             WinnerIdx == iOther.WinnerIdx &&
             GameDataX01 == iOther.GameDataX01 &&
             GameDataCricket == iOther.GameDataCricket;
    }

    QString TimeStamp = "";
    bool Finished = false;
    CSettings Settings = CSettings();
    uint32_t WinnerIdx = 0;
    QVector<QVector<CX01Class::CPlayerData>> GameDataX01 = {};
    QVector<QVector<CCricketClass::CPlayerData>> GameDataCricket = {};
  };

  struct SStatsData
  {
    SStatsData(QString  iPlayerName,
              QVector<CX01Class::CPlayerData> iGameDataX01 = {},
              QVector<CCricketClass::CPlayerData> iGameDataCricket = {}
              )
      : PlayerName(std::move(iPlayerName))
      , GameDataX01(std::move(iGameDataX01))
      , GameDataCricket(std::move(iGameDataCricket))
    {}

    SStatsData() = default;

    bool operator==(const SStatsData & iOther) const
    {
      return PlayerName == iOther.PlayerName &&
             GameDataX01 == iOther.GameDataX01 &&
             GameDataCricket == iOther.GameDataCricket;
    }

    QString PlayerName = "";
    QVector<CX01Class::CPlayerData> GameDataX01 = {};
    QVector<CCricketClass::CPlayerData> GameDataCricket = {};
  };

  CGameDataHandler();
  ~CGameDataHandler() = default;
  static bool add_new_player(const QString & iPlayerName);
  [[nodiscard]] static QStringList get_player_names();
  bool save_game_to_db(const SGameData & iGameData);
  QVector<SGameData> get_game_data(bool iAscending = false);
  QVector<SStatsData> get_stats_data();
  static bool delete_game_from_db(const QString & iTimeStamp);
  static bool delete_player_from_db(const QString & iPlayerName);

private:

  template<typename TPlayerData>
  void fill_game_data_array(const QVector<TPlayerData> & iGameHistory, QJsonArray & oGameDataArray);
  template<typename TPlayerData>
  void get_player_data(QVector<TPlayerData> & oGameHistory, const QJsonArray & iGameDataArray);
  template<typename T>
  void fill_vec(const T & iData, QJsonObject & oGameDataObject, const QString & iKey);
  template<typename T>
  void extract_vec(T & oData, QJsonObject & iGameDataObject, const QString & iKey);
  static bool create_connection();
  static bool create_players_table();
  static bool create_games_tables();
  [[nodiscard]] static bool player_exists(const QString & iPlayerName);
  [[nodiscard]] static int get_player_id(const QString & iPlayerName) ;
  [[nodiscard]] static QString get_player_name_from_id(int iPlayerId) ;

  static const QString mFileName;
};

#endif  // CGAMEDATAHANDLER_H
