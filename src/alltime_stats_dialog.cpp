#include "alltime_stats_dialog.h"
#include "ui_alltime_stats_dialog.h"
#include <QMenu>
#include <QPointer>

CAllTimeStatsDialog::CAllTimeStatsDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CAllTimeStatsDialog)
  , mGameDataHandler(iGameDataHandler)
  , mGameData(mGameDataHandler.get_game_data())
{
  mUi->setupUi(this);
  setup_drop_down_menu();
  connect(mUi->comboBoxGame, &QComboBox::currentIndexChanged, this, &CAllTimeStatsDialog::game_selected_slot);
}

CAllTimeStatsDialog::~CAllTimeStatsDialog()
{
  delete mUi;
  delete mStatsModel;
}

void CAllTimeStatsDialog::setup_drop_down_menu()
{
  QPointer<QMenu> menu = new QMenu(this);
  QStringList playerList = mGameDataHandler.get_player_names();

  for (auto & player : playerList)
  {
    QPointer<QAction> action = new QAction(player, menu);
    action->setCheckable(true);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, &CAllTimeStatsDialog::player_selected_slot);
  }

  mUi->pushButtonSelectPlayers->setMenu(menu);
}

void CAllTimeStatsDialog::player_selected_slot()
{
  QStringList selectedPlayers;
  for (auto * action : mUi->pushButtonSelectPlayers->menu()->actions())
  {
    if (action->isChecked()) selectedPlayers.append(action->text());
  }
  if (!mStatsModel)
  {
    mStatsModel = new CAlltimePlayerStatsModel(mGameDataHandler, selectedPlayers, mUi->comboBoxGame->currentIndex() == 1, this);
    mUi->tableView->setModel(mStatsModel);
  }
  else if (mUi->comboBoxGame->currentIndex() == 1)
  {
    mStatsModel->update_selected_players_cricket(selectedPlayers);
  }
  else
  {
    mStatsModel->update_selected_players_x01(selectedPlayers);
  }
}

void CAllTimeStatsDialog::game_selected_slot()
{
  if (!mStatsModel)
  {
    mStatsModel = new CAlltimePlayerStatsModel(mGameDataHandler, {}, mUi->comboBoxGame->currentIndex() == 1, this);
    mUi->tableView->setModel(mStatsModel);
  }
  {
    mStatsModel->update_selected_game(mUi->comboBoxGame->currentIndex() == 1);
  }
}

