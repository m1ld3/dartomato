#include "game_data_handler.h"
#include <QFile>
#include <QJsonArray>

CGameDataHandler::CGameDataHandler()
{
  initialize_game_data_file();
}

bool CGameDataHandler::initialize_game_data_file()
{
  QFile file("game_data.json");

  if (file.exists())
  {
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
      qWarning() << "Could not open the file: " << file.errorString();
      return false;
    }
    QByteArray jsonData = file.readAll();
    mGameDocument = QJsonDocument(QJsonDocument::fromJson(jsonData));
  }
  else if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QJsonObject root;
    root["players"] = QJsonArray();
    mGameDocument = QJsonDocument(root);
    file.write(mGameDocument.toJson());
    file.close();
  }
  else
  {
    qWarning() << "Failed to initialize JSON file.";
  }

  if (!mGameDocument.isObject())
  {
    qWarning() << "Invalid JSON document";
    return false;
  }

  return true;
}

QJsonArray CGameDataHandler::read_players_array() const
{
  if (mGameDocument.isObject())
  {
    QJsonObject root = mGameDocument.object();
    if (root.contains("players") && root["players"].isArray()) return root["players"].toArray();
  }
  return QJsonArray();
}

bool CGameDataHandler::add_new_player(const QString & iPlayerName)
{
  QJsonObject newPlayer;
  newPlayer["name"] = iPlayerName;
  if (!mGameDocument["players"].isArray()) return false;
  QJsonObject root = mGameDocument.object();
  QJsonArray playersArray = root["players"].toArray();
  playersArray.append(newPlayer);
  root["players"] = playersArray;
  mGameDocument.setObject(root);

  QFile saveFile("game_data.json");
  if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    saveFile.write(mGameDocument.toJson());
    saveFile.close();
    return true;
  }
  else
  {
    qWarning() << "Could not save the updated data.";
    return false;
  }
}

QStringList CGameDataHandler::get_player_names() const
{
  QStringList playerNames;
  if (!mGameDocument["players"].isArray()) return playerNames;

  QJsonArray playersArray = mGameDocument["players"].toArray();
  for (const QJsonValue & player : playersArray)
  {
    if (player.isObject() && player.toObject().contains("name") && player.toObject()["name"].isString())
    {
      playerNames.append(player.toObject()["name"].toString());
    }
  }

  return playerNames;
}
