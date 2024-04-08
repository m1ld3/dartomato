#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include "x01_class.h"
#include "cricket_class.h"

class CGameDataHandler
{
public:

  struct SGameData
  {
    SGameData(const QString iTimeStamp,
              const bool iFinished,
              const CSettings & iSettings,
              const uint32_t iWinnerIdx,
              QVector<QVector<CX01Class::CPlayerData>> iGameDataX01 = {},
              QVector<QVector<CCricketClass::CPlayerData>> iGameDataCricket = {}
              )
      : TimeStamp(iTimeStamp)
      , Finished(iFinished)
      , Settings(iSettings)
      , WinnerIdx(iWinnerIdx)
      , GameDataX01(iGameDataX01)
      , GameDataCricket(iGameDataCricket)
    {}

    SGameData() = default;

    QString TimeStamp = "";
    bool Finished = false;
    CSettings Settings = CSettings();
    uint32_t WinnerIdx = 0;
    QVector<QVector<CX01Class::CPlayerData>> GameDataX01 = {};
    QVector<QVector<CCricketClass::CPlayerData>> GameDataCricket = {};
  };

  struct SStatsData
  {
    SStatsData(const QString iPlayerName,
              QVector<CX01Class::CPlayerData> iGameDataX01 = {},
              QVector<CCricketClass::CPlayerData> iGameDataCricket = {}
              )
      : PlayerName(iPlayerName)
      , GameDataX01(iGameDataX01)
      , GameDataCricket(iGameDataCricket)
    {}

    SStatsData() = default;

    QString PlayerName = "";
    QVector<CX01Class::CPlayerData> GameDataX01 = {};
    QVector<CCricketClass::CPlayerData> GameDataCricket = {};
  };

  CGameDataHandler();
  ~CGameDataHandler() = default;
  bool add_new_player(const QString & iPlayerName);
  QStringList get_player_names() const;
  bool save_game_to_db(const SGameData & iGameData);
  QVector<SGameData> get_game_data(bool iAscending = false);
  QVector<SStatsData> get_stats_data();
  bool delete_game_from_db(const QString & iTimeStamp);
  bool delete_player_from_db(const QString & iPlayerName);

private:

  template<typename TPlayerData>
  void fill_game_data_array(const QVector<TPlayerData> & iGameHistory, QJsonArray & oGameDataArray);
  template<typename TPlayerData>
  void get_player_data(QVector<TPlayerData> & oGameHistory, const QJsonArray & iGameDataArray);
  template<typename T>
  void fill_vec(const T & iData, QJsonObject & oGameDataObject, const QString & iKey);
  template<typename T>
  void extract_vec(T & oData, QJsonObject & iGameDataObject, const QString & iKey);
  bool create_connection();
  bool create_players_table();
  bool create_games_tables();
  bool player_exists(const QString & iPlayerName) const;
  int get_player_id(const QString & iPlayerName) const;
  QString get_player_name_from_id(int iPlayerId) const;

  const QString mFileName = "game_data.sqlite";
};

#endif  // CGAMEDATAHANDLER_H
