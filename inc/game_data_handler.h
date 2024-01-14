#ifndef CGAMEDATAHANDLER_H
#define CGAMEDATAHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class CGameDataHandler : public QObject
{
  Q_OBJECT

public:

  CGameDataHandler(QObject * iParent = nullptr);
  ~CGameDataHandler() = default;
  bool initialize_game_data_file();
  QJsonArray read_players_array() const;
  bool add_new_players(const QVector<QString> & iPlayers);

private:

  std::unique_ptr<QJsonDocument> mGameData;

};

#endif // CGAMEDATAHANDLER_H
