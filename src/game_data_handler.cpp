#include "game_data_handler.h"
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

CGameDataHandler::CGameDataHandler()
{
  create_connection();
  create_players_table();
  create_game_modes_table();
  create_games_table();
}

bool CGameDataHandler::create_connection()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(mFileName);

  if (!db.open())
  {
    qWarning() << "Error: Unable to open database!" << db.lastError();
    return false;
  }

  return true;
}

bool CGameDataHandler::add_new_player(const QString & iPlayerName)
{  
  if (!create_players_table()) return false;

  if (player_exists(iPlayerName))
  {
    qWarning() << "Player name already exists!";
  }

  QSqlQuery query;
  query.prepare("INSERT INTO players (name) VALUES (:name)");
  query.bindValue(":name", iPlayerName);

  if (!query.exec())
  {
    qWarning() << "Error: Unable to insert player name." << query.lastError();
    return false;
  }

  return true;
}

bool CGameDataHandler::create_players_table()
{
  QSqlQuery query;

  if (!query.exec("CREATE TABLE IF NOT EXISTS players (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)"))
  {
    qWarning() << "Error: Unable to create players table" << query.lastError();
    return false;
  }

  return true;
}

bool CGameDataHandler::create_game_modes_table()
{
  QSqlQuery query;
  const QString createQuery = "CREATE TABLE IF NOT EXISTS game_modes (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)";
  const QString insertQuery = "INSERT OR IGNORE INTO game_modes (name) VALUES ('301'), ('501'), ('701'), ('Cricket')";

  if (!query.exec(createQuery))
  {
    qWarning() << "Error creating game modes table: " << query.lastError();
    return false;
  }

  if (!query.exec(insertQuery))
  {
    qWarning() << "Error inserting into game modes table: " << query.lastError();
    return false;
  }
  return true;
}

bool CGameDataHandler::create_games_table()
{
  QSqlQuery query;

  if (!query.exec("CREATE TABLE IF NOT EXISTS games (id INTEGER PRIMARY KEY AUTOINCREMENT, player_id INTEGER, game_mode_id INTEGER, game_data TEXT, FOREIGN KEY (player_id) REFERENCES players(id), FOREIGN KEY (game_mode_id) REFERENCES game_modes(id))"))
  {
    qWarning() << "Error: Unable to create games table" << query.lastError();
    return false;
  }

  return true;
}

bool CGameDataHandler::player_exists(const QString & iPlayerName) const
{
  QSqlQuery query;
  query.prepare("SELECT COUNT(*) FROM players WHERE name = :name");
  query.bindValue(":name", iPlayerName);

  if (!query.exec())
  {
    qWarning() << "Error in player_exists()" << query.lastError();
    return false;
  }

  query.next();
  int playerCount = query.value(0).toInt();

  return playerCount > 0;
}

QStringList CGameDataHandler::get_player_names() const
{
  QStringList playerNames;
  QSqlDatabase db = QSqlDatabase::database();

  if (!db.tables().contains("players"))
  {
    return playerNames;
  }

  QSqlQuery query("SELECT name FROM players");

  if (!query.exec())
  {
    qWarning() << "Error retrieving player names" << query.lastError();
    return playerNames;
  }

  while (query.next())
  {
    QString playerName = query.value(0).toString();
    playerNames.append(playerName);
  }

  return playerNames;
}

int CGameDataHandler::get_player_id(const QString & iPlayerName) const
{
  QSqlQuery query;
  query.prepare("SELECT id FROM players WHERE name = :name");
  query.bindValue(":name", iPlayerName);

  if (!query.exec() || !query.next())
  {
    qWarning() << "Error retrieving player ID" << query.lastError();
    return -1;
  }

  return query.value(0).toInt();
}

int CGameDataHandler::get_game_mode_id(const QString & iGameMode) const
{
  QSqlQuery query;
  query.prepare("SELECT id FROM game_modes WHERE name = :name");
  query.bindValue(":name", iGameMode);

  if (!query.exec())
  {
    qWarning() << "Error executing query:" << query.lastError();
    return -1;
  }

  if (!query.next())
  {
    qWarning() << "No matching game mode found for name:" << iGameMode;
    return -1;
  }

  return query.value(0).toInt();
}

bool CGameDataHandler::save_game_to_db_x01(bool iFinished, EGame iGame, const QString & iPlayerName, const std::vector<CX01Class::CPlayerData> & iGameHistory)
{

  const QString game = QString::number(static_cast<int>(iGame));

  QJsonObject gameFinishedObject;
  gameFinishedObject["finished"] = iFinished;
  QJsonArray gameDataArray;
  gameDataArray.append(gameFinishedObject);
  fill_game_data_array(iGameHistory, gameDataArray, iFinished);

  int playerId = get_player_id(iPlayerName);
  int gameModeId = get_game_mode_id(game);
  if (playerId == -1 || gameModeId == -1)
  {
    qWarning() << "Error: Invalid player or game mode";
    return false;
  }

  QSqlQuery query;
  query.prepare("INSERT INTO games (player_id, game_mode_id, game_data) VALUES (:player_id, :game_mode_id, :game_data)");
  query.bindValue(":player_id", playerId);
  query.bindValue(":game_mode_id", gameModeId);
  QByteArray jsonData = QJsonDocument(gameDataArray).toJson(QJsonDocument::Compact);
  query.bindValue(":game_data", QString(jsonData));

  if (!query.exec())
  {
    qWarning() << "Error: Unable to insert game entry" << query.lastError();
    return false;
  }

  return true;
}

void CGameDataHandler::fill_game_data_array(const std::vector<CX01Class::CPlayerData> & iGameHistory, QJsonArray & iGameDataArray, bool iFinished)
{
  std::vector<CX01Class::CPlayerData> gameHistory;
  if (iFinished) gameHistory.push_back(*std::prev(iGameHistory.end()));
  else gameHistory = iGameHistory;
  for (auto & data : gameHistory)
  {
    QJsonObject gameDataObject;
    gameDataObject["SetsWon"] = static_cast<int>(data.SetsWon);
    gameDataObject["LegsWonPerSet"] = static_cast<int>(data.LegsWonPerSet);
    gameDataObject["TotalLegsWon"] = static_cast<int>(data.TotalLegsWon);
    gameDataObject["RemainingPoints"] = static_cast<int>(data.RemainingPoints);
    gameDataObject["CheckoutAttempts"] = static_cast<int>(data.CheckoutAttempts);
    gameDataObject["CheckoutHits"] = static_cast<int>(data.CheckoutHits);
    gameDataObject["TotalDarts"] = static_cast<int>(data.TotalDarts);
    gameDataObject["Avg1Dart"] = static_cast<int>(data.Avg1Dart);
    gameDataObject["Avg3Dart"] = static_cast<int>(data.Avg3Dart);
    gameDataObject["CheckoutRate"] = static_cast<int>(data.CheckoutRate);

    fill_integer_array(data.ScoresOfCurrentLeg, gameDataObject, "ScoresOfCurrentLeg");
    fill_integer_array_of_arrays(data.AllScoresOfAllLegs, gameDataObject, "AllScoresOfAllLegs");
    fill_integer_array(data.AllScoresFlat, gameDataObject, "AllScoresFlat");
    fill_string_array_of_arrays(data.ThrownDartsOfCurrentLeg, gameDataObject, "ThrownDartsOfCurrentLeg");
    fill_string_array_of_arrays(data.ThrownDartsOfAllLegsFlat, gameDataObject, "ThrownDartsOfAllLegsFlat");
    fill_string_array_of_array_of_arrays(data.ThrownDartsOfAllLegs, gameDataObject, "ThrownDartsOfAllLegs");
    fill_integer_array(data.RemainingPointsOfCurrentLeg, gameDataObject, "RemainingPointsOfCurrentLeg");
    fill_integer_array_of_arrays(data.RemainingPointsOfAllLegs, gameDataObject, "RemainingPointsOfAllLegs");

    iGameDataArray.append(gameDataObject);
  }
}

void CGameDataHandler::fill_integer_array(const QVector<uint32_t> & iData, QJsonObject & iGameDataObject, const QString & iKey)
{
  QJsonArray tempArray;
  for (const auto & val : iData) tempArray.append(static_cast<int>(val));
  iGameDataObject[iKey] = tempArray;
}

void CGameDataHandler::fill_integer_array_of_arrays(const QVector<QVector<uint32_t>> & iData, QJsonObject & iGameDataObject, const QString & iKey)
{
  QJsonArray outerArray;
  for (const auto & innerVec : iData)
  {
    QJsonArray innerArray;
    for (const auto & val : innerVec) innerArray.append(static_cast<int>(val));
    outerArray.append(innerArray);
  }
  iGameDataObject[iKey] = outerArray;
}

void CGameDataHandler::fill_string_array_of_arrays(const QVector<QVector<QString>> & iData, QJsonObject & iGameDataObject, const QString & iKey)
{
  QJsonArray outerArray;
  for (const auto & innerVec : iData)
  {
    QJsonArray innerArray;
    for (const auto & val : innerVec) innerArray.append(val);
    outerArray.append(innerArray);
  }
  iGameDataObject[iKey] = outerArray;
}

void CGameDataHandler::fill_string_array_of_array_of_arrays(const QVector<QVector<QVector<QString>>> & iData, QJsonObject & iGameDataObject, const QString & iKey)
{
  QJsonArray outerArray;
  for (const auto & midVec : iData)
  {
    QJsonArray midArray;
    for (const auto & innerVec : midVec)
    {
      QJsonArray innerArray;
      for (const auto & val : innerVec) innerArray.append(val);
      midArray.append(innerArray);
    }
    outerArray.append(midArray);
  }
  iGameDataObject[iKey] = outerArray;
}
