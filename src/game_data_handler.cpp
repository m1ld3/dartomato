#include "game_data_handler.h"
#include <QFile>
#include <QJsonArray>

CGameDataHandler::CGameDataHandler(QObject * iParent)
  : QObject(iParent)
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
    mGameData = std::make_unique<QJsonDocument>(QJsonDocument(QJsonDocument::fromJson(jsonData)));
  }
  else if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QJsonObject root;
    root["players"] = QJsonArray();
    mGameData = std::make_unique<QJsonDocument>(root);
    file.write(mGameData->toJson());
    file.close();
  }
  else
  {
    qWarning() << "Failed to initialize JSON file.";
  }

  if (!mGameData->isObject())
  {
    qWarning() << "Invalid JSON document";
    return false;
  }

  return true;
}

QJsonArray CGameDataHandler::read_players_array() const
{
  if (mGameData && mGameData->isObject())
  {
    QJsonObject root = mGameData->object();
    if (root.contains("players") && root["players"].isArray()) return root["players"].toArray();
  }
  return QJsonArray();
}

bool CGameDataHandler::add_new_players(const QVector<QString> & iPlayers)
{
  QJsonObject root = mGameData->object();
  QJsonArray existingPlayersArray = root["players"].toArray();
  QJsonArray newPlayersArray = QJsonArray::fromStringList(iPlayers);

  for (const auto & newPlayer : newPlayersArray)
  {
    if (!existingPlayersArray.contains(newPlayer))
    {
      existingPlayersArray.append(newPlayer);
    }
  }

  root["players"] = existingPlayersArray;

  QFile saveFile("game_data.json");
  if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    saveFile.write(QJsonDocument(root).toJson());
    saveFile.close();
    return true;
  }
  else
  {
    qWarning() << "Could not save the updated data.";
    return false;
  }
}
