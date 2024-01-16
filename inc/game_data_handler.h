#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class CGameDataHandler
{
public:

  CGameDataHandler();
  ~CGameDataHandler() = default;
  bool initialize_game_data_file();
  QJsonArray read_players_array() const;
  bool add_new_player(const QString & iPlayerName);
  QStringList get_player_names() const;

private:

  QJsonDocument mGameDocument;

};

#endif // CGAMEDATAHANDLER_H
