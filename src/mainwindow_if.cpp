#include "mainwindow_if.h"
#include "x01_mainwindow.h"
#include "cricket_mainwindow.h"

IMainWindow * IMainWindow::create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler)
{
  if (iSettings.Game == EGame::GAME_CRICKET)
  {
    return new CCricketMainWindow(iParent, iSettings, iGameDataHandler);
  }
  else
  {
    return new CX01MainWindow(iParent, iSettings, iGameDataHandler);
  }
}

IMainWindow * IMainWindow::create(QWidget * iParent, const CSettings iSettings, CGameDataHandler & iGameDataHandler, const CGameDataHandler::SGameData iGameData)
{
  if (iSettings.Game == EGame::GAME_CRICKET)
  {
    return new CCricketMainWindow(iParent, iSettings, iGameDataHandler, iGameData);
  }
  else
  {
    return new CX01MainWindow(iParent, iSettings, iGameDataHandler, iGameData);
  }
}
