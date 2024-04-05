#ifndef ALLTIME_STATS_DIALOG_H
#define ALLTIME_STATS_DIALOG_H

#include <QDialog>
#include "game_data_handler.h"
#include "alltime_player_stats_model.h"

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

  void player_selected_slot();
  void game_selected_slot();

private:

//  class CAlltimeStatsChart
//  {

//  };

  void setup_drop_down_menu();
  void prepare_plot_data();
  void update_stats_combobox(bool isCricket);

  Ui::CAllTimeStatsDialog * mUi;
  CGameDataHandler & mGameDataHandler;
  QVector<CGameDataHandler::SGameData> mGameData;
  CAlltimePlayerStatsModel * mStatsModel = nullptr;
};

#endif  // ALLTIME_STATS_DIALOG_H
