#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include "x01_class.h"
#include "cricket_class.h"

class CGameDataHandler
{
public:

  CGameDataHandler();
  ~CGameDataHandler() = default;
  bool add_new_player(const QString & iPlayerName);
  QStringList get_player_names() const;
  bool save_game_to_db_x01(const QString & iTimeStamp, const bool iFinished, const CSettings & iSettings, const QString & iPlayerName, const std::vector<CX01Class::CPlayerData> & iGameHistory);
  bool save_game_to_db_cricket(const QString & iTimeStamp, const bool iFinished, const CSettings & iSettings, const QString & iPlayerName, const std::vector<CCricketClass::CPlayerData> & iGameHistory);

private:

  void fill_game_data_array_x01(const std::vector<CX01Class::CPlayerData> & iGameHistory, QJsonArray & iGameDataArray);
  void fill_game_data_array_cricket(const std::vector<CCricketClass::CPlayerData> & iGameHistory, QJsonArray & iGameDataArray);
  void fill_integer_vec(const QVector<uint32_t> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_integer_vec_of_vecs(const QVector<QVector<uint32_t>> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_string_vec_of_vecs(const QVector<QVector<QString>> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_string_vec_of_vec_of_vecs(const QVector<QVector<QVector<QString>>> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  bool create_connection();
  bool create_players_table();
  bool create_game_modes_table();
  bool create_games_tables();
  bool player_exists(const QString & iPlayerName) const;
  int get_player_id(const QString & iPlayerName) const;
  int get_game_mode_id(const QString & iGameType) const;

  const QString mFileName = "game_data.sqlite";
};

#endif  // CGAMEDATAHANDLER_H
