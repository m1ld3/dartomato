#ifndef ALLTIME_STATS_DIALOG_H
#define ALLTIME_STATS_DIALOG_H

#include <QDialog>
#include "game_data_handler.h"

namespace Ui
{
  class CAllTimeStatsDialog;
}

class CAllTimeStatsDialog : public QDialog
{
  Q_OBJECT

public:

  explicit CAllTimeStatsDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent = nullptr);
  ~CAllTimeStatsDialog();

private slots:

private:

  void setup_drop_down_menu();

  Ui::CAllTimeStatsDialog * mUi;
  CGameDataHandler & mGameDataHandler;
  QVector<CGameDataHandler::SGameData> mGameData;
};

#endif  // ALLTIME_STATS_DIALOG_H
