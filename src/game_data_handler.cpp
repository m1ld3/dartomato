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
  create_games_tables();
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

bool CGameDataHandler::create_games_tables()
{
  QSqlQuery query;

  if (!query.exec("CREATE TABLE IF NOT EXISTS games (id INTEGER PRIMARY KEY AUTOINCREMENT, player_id INTEGER, winner_id INTEGER, game_mode INTEGER, time_stamp TEXT, finished INTEGER, best_of_legs INTEGER, best_of_sets INTEGER, in_mode INTEGER, out_mode INTEGER, cutthroat INTEGER, game_data TEXT, FOREIGN KEY (player_id) REFERENCES players(id), FOREIGN KEY (winner_id) REFERENCES players(id))"))
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

QString CGameDataHandler::get_player_name_from_id(int iPlayerId) const
{
  QSqlQuery query;
  query.prepare("SELECT name FROM players WHERE id = :playerId");
  query.bindValue(":playerId", iPlayerId);

  if (query.exec() && query.next())
  {
    return query.value("name").toString();
  }
  else
  {
    qWarning() << "Error: Unable to retrieve player name for player id" << iPlayerId << query.lastError();
    return "";
  }
}

template<>
void CGameDataHandler::fill_game_data_array(const QVector<CX01Class::CPlayerData> & iGameHistory, QJsonArray & oGameDataArray)
{
  for (auto & data : iGameHistory)
  {
    QJsonObject gameDataObject;
    gameDataObject["SetsWon"] = static_cast<int>(data.SetsWon);
    gameDataObject["LegsWonPerSet"] = static_cast<int>(data.LegsWonPerSet);
    gameDataObject["TotalLegsWon"] = static_cast<int>(data.TotalLegsWon);
    gameDataObject["RemainingPoints"] = static_cast<int>(data.RemainingPoints);
    gameDataObject["CheckoutAttempts"] = static_cast<int>(data.CheckoutAttempts);
    gameDataObject["CheckoutHits"] = static_cast<int>(data.CheckoutHits);
    gameDataObject["TotalDarts"] = static_cast<int>(data.TotalDarts);
    gameDataObject["Avg1Dart"] = data.Avg1Dart;
    gameDataObject["Avg3Dart"] = data.Avg3Dart;
    gameDataObject["CheckoutRate"] = data.CheckoutRate;

    fill_vec(data.ScoresOfCurrentLeg, gameDataObject, "ScoresOfCurrentLeg");
    fill_vec(data.AllScoresOfAllLegs, gameDataObject, "AllScoresOfAllLegs");
    fill_vec(data.AllScoresFlat, gameDataObject, "AllScoresFlat");
    fill_vec(data.ThrownDartsOfCurrentLeg, gameDataObject, "ThrownDartsOfCurrentLeg");
    fill_vec(data.ThrownDartsOfAllLegsFlat, gameDataObject, "ThrownDartsOfAllLegsFlat");
    fill_vec(data.ThrownDartsOfAllLegs, gameDataObject, "ThrownDartsOfAllLegs");
    fill_vec(data.RemainingPointsOfCurrentLeg, gameDataObject, "RemainingPointsOfCurrentLeg");
    fill_vec(data.RemainingPointsOfAllLegs, gameDataObject, "RemainingPointsOfAllLegs");

    oGameDataArray.append(gameDataObject);
  }
}

template<>
void CGameDataHandler::fill_game_data_array(const QVector<CCricketClass::CPlayerData> & iGameHistory, QJsonArray & oGameDataArray)
{
  for (auto & data : iGameHistory)
  {
    QJsonObject gameDataObject;
    gameDataObject["SetsWon"] = static_cast<int>(data.SetsWon);
    gameDataObject["LegsWonPerSet"] = static_cast<int>(data.LegsWonPerSet);
    gameDataObject["TotalLegsWon"] = static_cast<int>(data.TotalLegsWon);
    gameDataObject["TotalDarts"] = static_cast<int>(data.TotalDarts);
    gameDataObject["Score"] = static_cast<int>(data.Score);
    gameDataObject["TotalHits"] = static_cast<int>(data.TotalHits);
    gameDataObject["TotalDarts"] = data.HitsPerRound;

    fill_vec(data.ScoresOfCurrentLeg, gameDataObject, "ScoresOfCurrentLeg");
    fill_vec(data.ScoringHistory, gameDataObject, "ScoringHistory");
    fill_vec(data.SlotArray, gameDataObject, "SlotArray");
    fill_vec(data.ExtraPointsArray, gameDataObject, "ExtraPointsArray");

    oGameDataArray.append(gameDataObject);
  }
}

template<>
void CGameDataHandler::get_player_data(QVector<CX01Class::CPlayerData> & oGameHistory, const QJsonArray & iGameDataArray)
{
  for (const auto & jsonObject : iGameDataArray)
  {
    if (!jsonObject.isObject()) continue;

    QJsonObject gameDataObject = jsonObject.toObject();
    CX01Class::CPlayerData playerData;

    playerData.SetsWon = gameDataObject["SetsWon"].toInt();
    playerData.LegsWonPerSet = gameDataObject["LegsWonPerSet"].toInt();
    playerData.TotalLegsWon = gameDataObject["TotalLegsWon"].toInt();
    playerData.RemainingPoints = gameDataObject["RemainingPoints"].toInt();
    playerData.CheckoutAttempts = gameDataObject["CheckoutAttempts"].toInt();
    playerData.CheckoutHits = gameDataObject["CheckoutHits"].toInt();
    playerData.TotalDarts = gameDataObject["TotalDarts"].toInt();
    playerData.Avg1Dart = gameDataObject["Avg1Dart"].toDouble();
    playerData.Avg3Dart = gameDataObject["Avg3Dart"].toDouble();
    playerData.CheckoutRate = gameDataObject["CheckoutRate"].toDouble();

    extract_vec(playerData.ScoresOfCurrentLeg, gameDataObject, "ScoresOfCurrentLeg");
    extract_vec(playerData.AllScoresOfAllLegs, gameDataObject, "AllScoresOfAllLegs");
    extract_vec(playerData.AllScoresFlat, gameDataObject, "AllScoresFlat");
    extract_vec(playerData.ThrownDartsOfCurrentLeg, gameDataObject, "ThrownDartsOfCurrentLeg");
    extract_vec(playerData.ThrownDartsOfAllLegsFlat, gameDataObject, "ThrownDartsOfAllLegsFlat");
    extract_vec(playerData.ThrownDartsOfAllLegs, gameDataObject, "ThrownDartsOfAllLegs");
    extract_vec(playerData.RemainingPointsOfCurrentLeg, gameDataObject, "RemainingPointsOfCurrentLeg");
    extract_vec(playerData.RemainingPointsOfAllLegs, gameDataObject, "RemainingPointsOfAllLegs");

    oGameHistory.append(playerData);
  }
}

template<>
void CGameDataHandler::get_player_data(QVector<CCricketClass::CPlayerData> & oGameHistory, const QJsonArray & iGameDataArray)
{
  for (const auto & jsonObject : iGameDataArray)
  {
    if (!jsonObject.isObject()) continue;

    QJsonObject gameDataObject = jsonObject.toObject();
    CCricketClass::CPlayerData playerData;

    playerData.SetsWon = gameDataObject["SetsWon"].toInt();
    playerData.LegsWonPerSet = gameDataObject["LegsWonPerSet"].toInt();
    playerData.TotalLegsWon = gameDataObject["TotalLegsWon"].toInt();
    playerData.TotalDarts = gameDataObject["TotalDarts"].toInt();
    playerData.Score = gameDataObject["Score"].toInt();
    playerData.TotalHits = gameDataObject["TotalHits"].toInt();
    playerData.TotalDarts = gameDataObject["TotalDarts"].toInt();

    extract_vec(playerData.ScoresOfCurrentLeg, gameDataObject, "ScoresOfCurrentLeg");
    extract_vec(playerData.ScoringHistory, gameDataObject, "ScoringHistory");
    extract_vec(playerData.SlotArray, gameDataObject, "SlotArray");
    extract_vec(playerData.ExtraPointsArray, gameDataObject, "ExtraPointsArray");

    oGameHistory.append(playerData);
  }
}

bool CGameDataHandler::save_game_to_db(const SGameData & iGameData)
{
  for (uint32_t i = 0; i < iGameData.Settings.PlayersList.size(); i++)
  {
    QJsonArray gameDataArray;
    if (iGameData.GameDataX01.size() > 0) fill_game_data_array(iGameData.GameDataX01.at(i), gameDataArray);
    else fill_game_data_array(iGameData.GameDataCricket.at(i), gameDataArray);
    int playerId = get_player_id(iGameData.Settings.PlayersList.at(i));

    if (playerId == -1)
    {
      qWarning() << "Error: Invalid player or game mode";
      return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO games (player_id, winner_id, game_mode, time_stamp, finished, best_of_legs, best_of_sets, in_mode, out_mode, cutthroat, game_data) VALUES (:player_id, :winner_id, :game_mode, :time_stamp, :finished, :best_of_legs, :best_of_sets, :in_mode, :out_mode, :cutthroat, :game_data)");
    query.bindValue(":player_id", playerId);
    query.bindValue(":winner_id", iGameData.WinnerIdx);
    query.bindValue(":game_mode", static_cast<int>(iGameData.Settings.Game));
    query.bindValue(":time_stamp", iGameData.TimeStamp);
    query.bindValue(":finished", iGameData.Finished);
    query.bindValue(":best_of_legs", iGameData.Settings.Legs);
    query.bindValue(":best_of_sets", iGameData.Settings.Sets);
    query.bindValue(":in_mode", static_cast<int>(iGameData.Settings.InMode));
    query.bindValue(":out_mode", static_cast<int>(iGameData.Settings.OutMode));
    query.bindValue(":cutthroat", iGameData.Settings.CutThroat);
    QByteArray jsonData = QJsonDocument(gameDataArray).toJson(QJsonDocument::Compact);
    query.bindValue(":game_data", QString(jsonData));

    if (!query.exec())
    {
      qWarning() << "Error: Unable to insert game entry" << query.lastError();
      return false;
    }
  }

  return true;
}

QVector<CGameDataHandler::SGameData> CGameDataHandler::get_game_data()
{
  QSqlDatabase db = QSqlDatabase::database();
  QVector<SGameData> gameData;
  QVector<QString> timeStamps;

  QSqlQuery timeStampQuery("SELECT DISTINCT time_stamp FROM games", db);
  while (timeStampQuery.next())
  {
    timeStamps.append(timeStampQuery.value("time_stamp").toString());
  }

  for (const auto & timeStamp : timeStamps)
  {
    QVector<QString> playerNamesArray = {};
    QSqlQuery selectQuery(QString("SELECT * FROM games WHERE time_stamp='%1'").arg(timeStamp));
    QVector<QVector<CX01Class::CPlayerData>> x01Data {};
    QVector<QVector<CCricketClass::CPlayerData>> cricketData {};
    auto gameMode = EGame::GAME_501;
    auto inMode = EX01InMode::SINGLE_IN;
    auto outMode = EX01OutMode::DOUBLE_OUT;
    bool finished = false;
    bool cutthroat = false;
    int sets = 1;
    int legs = 1;
    bool first = true;
    uint32_t winnerId = 0;

    while (selectQuery.next())
    {
      QVector<CX01Class::CPlayerData> singleX01Data {};
      QVector<CCricketClass::CPlayerData> singleCricketData {};

      if (first)
      {
        gameMode = static_cast<EGame>(selectQuery.value("game_mode").toInt());
        finished = selectQuery.value("finished").toBool();
        legs = selectQuery.value("best_of_legs").toInt();
        sets = selectQuery.value("best_of_sets").toInt();
        inMode = static_cast<EX01InMode>(selectQuery.value("in_mode").toInt());
        outMode = static_cast<EX01OutMode>(selectQuery.value("out_mode").toInt());
        cutthroat = selectQuery.value("cutthroat").toBool();
        winnerId = selectQuery.value("winner_id").toInt();
        first = false;
      }

      playerNamesArray.append(get_player_name_from_id(selectQuery.value("player_id").toInt()));
      QString gameDataString = selectQuery.value("game_data").toString();
      QJsonDocument jsonDoc = QJsonDocument::fromJson(gameDataString.toUtf8());
      QJsonArray gameDataArray = jsonDoc.array();

      if (gameMode == EGame::GAME_CRICKET) get_player_data(singleCricketData, gameDataArray);
      else get_player_data(singleX01Data, gameDataArray);
      x01Data.append(singleX01Data);
      cricketData.append(singleCricketData);
    }
    auto settings = CSettings(gameMode, playerNamesArray, sets, legs, inMode, outMode, cutthroat);
    auto singleGameData = SGameData(timeStamp, finished, settings, winnerId, x01Data, cricketData);
    gameData.append(singleGameData);
  }

  return gameData;
}

template<typename T>
void CGameDataHandler::fill_vec(const T & iData, QJsonObject & oGameDataObject, const QString & iKey)
{
  QJsonArray tempArray;
  if constexpr (std::is_same_v<T, QVector<QVector<uint32_t>>> ||
                std::is_same_v<T, QVector<QVector<QString>>> ||
                std::is_same_v<T, QVector<QVector<QVector<QString>>>>)
  {
    for (const auto & val : iData)
    {
      QJsonObject innerObject;
      fill_vec(val, innerObject, iKey);
      tempArray.append(innerObject);
    }
  }
  else
  {
    for (const auto & val : iData)
    {
      if constexpr (std::is_same_v<decltype(val), const uint32_t &>)
      {
        tempArray.append(static_cast<int>(val));
      }
      else
      {
        tempArray.append(val);
      }
    }
  }

  oGameDataObject[iKey] = tempArray;
}

template void CGameDataHandler::fill_vec<QVector<uint32_t>>(const QVector<uint32_t> & iData, QJsonObject & iGameDataObject, const QString & iKey);
template void CGameDataHandler::fill_vec<QVector<QVector<QVector<QString>>>>(const QVector<QVector<QVector<QString>>> & iData, QJsonObject & iGameDataObject, const QString & iKey);

template<typename T>
void CGameDataHandler::extract_vec(T & oData, QJsonObject & iGameDataObject, const QString & iKey)
{
  const QJsonValue jsonValue = iGameDataObject.value(iKey);
  if (jsonValue.isArray())
  {
    const QJsonArray jsonArray = jsonValue.toArray();
    for (const QJsonValue & data : jsonArray)
    {
      if constexpr (std::is_same_v<T, QVector<QVector<uint32_t>>> ||
                    std::is_same_v<T, QVector<QVector<QString>>> ||
                    std::is_same_v<T, QVector<QVector<QVector<QString>>>>)
      {
        typename T::value_type innerData;
        QJsonObject innerObject = data.toObject();
        extract_vec(innerData, innerObject, iKey);
        oData.append(innerData);
      }
      else
      {
        if constexpr (std::is_same_v<T, QVector<uint32_t>>)
        {
          oData.append(data.toInt());
        }
        else
        {
          oData.append(data.toString());
        }
      }
    }
  }
}

template void CGameDataHandler::extract_vec<QVector<uint32_t>>(QVector<uint32_t> & oData, QJsonObject & iGameDataObject, const QString & iKey);
template void CGameDataHandler::extract_vec<QVector<QVector<QVector<QString>>>>(QVector<QVector<QVector<QString>>> & oData, QJsonObject & iGameDataObject, const QString & iKey);
