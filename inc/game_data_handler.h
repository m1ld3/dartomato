#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "x01_class.h"

class CGameDataHandler
{
public:

  CGameDataHandler();
  ~CGameDataHandler() = default;
  bool initialize_game_data_file();
  QJsonArray read_players_array() const;
  bool add_new_player(const QString & iPlayerName);
  QStringList get_player_names() const;
  bool save_game_to_file_x01(bool iFinished, EGame iGame, const QString & iPlayerName, const std::vector<CX01Class::CPlayerData> & iGameHistory);

private:

  bool write_to_file();
  bool load_file();
  void fill_game_data_array(const std::vector<CX01Class::CPlayerData> & iGameHistory, QJsonArray & iGameDataArray);
  void add_game_data_to_existing_mode(QJsonObject & iPlayerObject, QJsonArray & iGameDataArray, const QString & iGame);
  void create_mode_with_game_data(QJsonObject & iPlayerObject, QJsonArray & iGameDataArray, const QString & iGame);
  void fill_integer_array(const QVector<uint32_t> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_integer_array_of_arrays(const QVector<QVector<uint32_t>> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_string_array_of_arrays(const QVector<QVector<QString>> & iData, QJsonObject & iGameDataObject, const QString & iKey);
  void fill_string_array_of_array_of_arrays(const QVector<QVector<QVector<QString>>> & iData, QJsonObject & iGameDataObject, const QString & iKey);

  QJsonDocument mGameDocument;
  const QString mFileName = "game_data.json";
};

#endif  // CGAMEDATAHANDLER_H
