#include "mainwindow_if.h"
#include "x01_mainwindow.h"
#include "cricket_mainwindow.h"

std::unique_ptr<IMainWindow> IMainWindow::create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler)
{
  if (iSettings.Game == EGame::GAME_CRICKET)
  {
    return std::make_unique<CCricketMainWindow>(iParent, iSettings, iGameDataHandler);
  }
  else
  {
    return std::make_unique<CX01MainWindow>(iParent, iSettings, iGameDataHandler);
  }
}

std::unique_ptr<IMainWindow> IMainWindow::create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const CGameDataHandler::SGameData iGameData)
{
  if (iSettings.Game == EGame::GAME_CRICKET)
  {
    return std::make_unique<CCricketMainWindow>(iParent, iSettings, iGameDataHandler, iGameData);
  }
  else
  {
    return std::make_unique<CX01MainWindow>(iParent, iSettings, iGameDataHandler, iGameData);
  }
}
