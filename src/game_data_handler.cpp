#include "game_data_handler.h"
#include <QFile>
#include <QJsonArray>

CGameDataHandler::CGameDataHandler()
{
  initialize_game_data_file();
}

bool CGameDataHandler::initialize_game_data_file()
{
  QFile file(mFileName);
  if (file.exists())
  {
    load_file();
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

bool CGameDataHandler::write_to_file()
{
  QFile saveFile(mFileName);
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

bool CGameDataHandler::add_new_player(const QString & iPlayerName)
{
  if (!load_file()) return false;
  QJsonObject newPlayer;
  newPlayer["name"] = iPlayerName;
  if (!mGameDocument["players"].isArray()) return false;
  QJsonObject root = mGameDocument.object();
  QJsonArray playersArray = root["players"].toArray();
  playersArray.append(newPlayer);
  root["players"] = playersArray;
  mGameDocument.setObject(root);
  return write_to_file();
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

bool CGameDataHandler::load_file()
{
  QFile file(mFileName);
  if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qWarning() << "Could not open the file: " << file.errorString();
    return false;
  }
  QByteArray jsonData = file.readAll();
  mGameDocument = QJsonDocument(QJsonDocument::fromJson(jsonData));
  file.close();
  return true;
}

bool CGameDataHandler::save_game_to_file_x01(bool iFinished, EGame iGame, const QString & iPlayerName, const std::vector<CX01Class::CPlayerData> & iGameHistory)
{
  load_file();
  QJsonArray playersArray = mGameDocument["players"].toArray();
  const QString game = QString::number(static_cast<int>(iGame));
  for (uint32_t i = 0; i < playersArray.size(); i++)
  {
    const QJsonValue & player = playersArray.at(i);
    QJsonObject playerObject = player.toObject();
    if (playerObject["name"].toString() == iPlayerName)
    {
      QJsonObject gameFinishedObject;
      gameFinishedObject["finished"] = iFinished;
      QJsonArray gameDataArray;
      gameDataArray.append(gameFinishedObject);

      fill_game_data_array(iGameHistory, gameDataArray);

      if (playerObject.contains("mode"))
      {
        add_game_data_to_existing_mode(playerObject, gameDataArray, game);
      }
      else
      {
        create_mode_with_game_data(playerObject, gameDataArray, game);
      }

      playersArray.replace(i, playerObject);
      break;
    }
  }

  QJsonObject root = mGameDocument.object();
  root["players"] = playersArray;
  mGameDocument.setObject(root);

  return write_to_file();
}

void CGameDataHandler::fill_game_data_array(const std::vector<CX01Class::CPlayerData> & iGameHistory, QJsonArray & iGameDataArray)
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

void CGameDataHandler::add_game_data_to_existing_mode(QJsonObject & iPlayerObject, QJsonArray & iGameDataArray, const QString & iGame)
{
  QJsonArray modeArray = iPlayerObject["mode"].toArray();
  bool modeExists = false;
  for (uint32_t i = 0; i < modeArray.size(); i++)
  {
    const QJsonValue & mode = modeArray.at(i);
    QJsonObject modeObject = mode.toObject();
    if (modeObject.contains(iGame))
    {
      QJsonArray gameArray = modeObject[iGame].toArray();
      gameArray.append(iGameDataArray);
      modeObject[iGame] = gameArray;
      modeArray.replace(i, modeObject);
      iPlayerObject["mode"] = modeArray;
      modeExists = true;
      break;
    }
  }
  if (!modeExists)
  {
    QJsonArray gameArray;
    gameArray.append(iGameDataArray);
    QJsonObject modeObject;
    modeObject[iGame] = gameArray;
    modeArray.append(modeObject);
    iPlayerObject["mode"] = modeArray;
  }
}

void CGameDataHandler::create_mode_with_game_data(QJsonObject & iPlayerObject, QJsonArray & iGameDataArray, const QString & iGame)
{
  QJsonArray gameArray;
  gameArray.append(iGameDataArray);
  QJsonObject modeObject;
  modeObject[iGame] = gameArray;
  QJsonArray modeArray;
  modeArray.append(modeObject);
  iPlayerObject["mode"] = modeArray;
}
