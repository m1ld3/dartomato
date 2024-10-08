#include "alltime_stats_dialog.h"
#include "ui_alltime_stats_dialog.h"
#include <QMenu>
#include <QPointer>
#include <QValueAxis>

CAllTimeStatsDialog::CAllTimeStatsDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CAllTimeStatsDialog)
  , mGameDataHandler(iGameDataHandler)
  , mGameData(mGameDataHandler.get_game_data(true))
  , mPlayerList(mGameDataHandler.get_player_names())
{
  mUi->setupUi(this);
  mUi->graphicsView->setChart(mChart);
  mUi->graphicsView->setRenderHint(QPainter::Antialiasing);
  setup_drop_down_menu();
  prepare_plot_data();
  mStatsModel = new CAlltimePlayerStatsModel(mGameDataHandler, {}, mUi->comboBoxGame->currentIndex() == 1, this);
  mUi->tableView->setModel(mStatsModel);
  mUi->tableView->resizeColumnsToContents();
  connect(mUi->comboBoxGame, &QComboBox::currentIndexChanged, this, &CAllTimeStatsDialog::game_selected_slot);
  update_stats_combobox(mUi->comboBoxGame->currentIndex() == 1);
  connect(mUi->comboBoxStats, &QComboBox::currentIndexChanged, this, &CAllTimeStatsDialog::stat_selected_slot);
}

CAllTimeStatsDialog::~CAllTimeStatsDialog()
{
  delete mUi;
  delete mStatsModel;
}

void CAllTimeStatsDialog::setup_drop_down_menu()
{
  QPointer<QMenu> menu = new QMenu(this);

  for (auto & player : mPlayerList)
  {
    QPointer<QAction> action = new QAction(player, menu);
    action->setCheckable(true);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, &CAllTimeStatsDialog::player_selected_slot);
  }

  mUi->pushButtonSelectPlayers->setMenu(menu);
}

void CAllTimeStatsDialog::prepare_plot_data()
{
  mPlotData.resize(mPlayerList.size());

  uint32_t gameIdx = 0;
  for (const auto & game : mGameData)
  {
    if (game.Settings.Game == EGame::GAME_CRICKET)
    {
      for (uint32_t idx = 0; idx < mPlayerList.size(); idx++)
      {
        auto localIdx = game.Settings.PlayersList.indexOf(mPlayerList.at(idx));
        if (localIdx != -1)
        {
          const CCricketClass::CPlayerData & cricketGame = game.GameDataCricket.at(localIdx).back();
          mPlotData[idx].HitsPerRound->append(gameIdx, cricketGame.HitsPerRound);
          mPlotData[idx].HitsPerRound->setName(mPlayerList.at(idx));
          mPlotData[idx].DartsPerLegAvgCricket->append(gameIdx, (cricketGame.ScoringHistory.size()) ? static_cast<double>(cricketGame.TotalDarts) / cricketGame.ScoringHistory.size() : 0.0);
          mPlotData[idx].DartsPerLegAvgCricket->setName(mPlayerList.at(idx));
        }
      }
    }
    else
    {
      for (uint32_t idx = 0; idx < mPlayerList.size(); idx++)
      {
        auto localIdx = game.Settings.PlayersList.indexOf(mPlayerList.at(idx));
        if (localIdx != -1)
        {
          const CX01Class::CPlayerData & x01Game = game.GameDataX01.at(localIdx).back();
          mPlotData[idx].Avg3Dart->append(gameIdx, x01Game.Avg3Dart);
          mPlotData[idx].Avg3Dart->setName(mPlayerList.at(idx));
          mPlotData[idx].First9Avg->append(gameIdx, x01Game.First9Avg);
          mPlotData[idx].First9Avg->setName(mPlayerList.at(idx));
          mPlotData[idx].CheckoutRate->append(gameIdx, x01Game.CheckoutRate);
          mPlotData[idx].CheckoutRate->setName(mPlayerList.at(idx));
          mPlotData[idx].DartsPerLegAvgX01->append(gameIdx, (x01Game.AllScoresOfAllLegs.size()) ? static_cast<double>(x01Game.TotalDarts) / x01Game.AllScoresOfAllLegs.size() : 0.0);
          mPlotData[idx].DartsPerLegAvgX01->setName(mPlayerList.at(idx));
        }
      }
    }
    gameIdx++;
  }
}

void CAllTimeStatsDialog::update_stats_combobox(bool iIsCricket)
{
  if (iIsCricket)
  {
    mUi->comboBoxStats->clear();
    mUi->comboBoxStats->addItems({"Hits Per Round",
                                  "Darts Per Leg Average"
                                 });
  }
  else
  {
    mUi->comboBoxStats->clear();
    mUi->comboBoxStats->addItems({"3-Dart Average",
                                  "First-9-Average",
                                  "Checkout Rate",
                                  "Darts Per Leg Average"
                                 });
  }
}

void CAllTimeStatsDialog::get_min_max(SPlotRange & oPlotRange, const QLineSeries * const iSeries)
{
  for (auto & point : iSeries->points())
  {
    if (point.x() < oPlotRange.MinX) oPlotRange.MinX = point.x();
    if (point.x() > oPlotRange.MaxX) oPlotRange.MaxX = point.x();
    if (point.y() < oPlotRange.MinY) oPlotRange.MinY = point.y();
    if (point.y() > oPlotRange.MaxY) oPlotRange.MaxY = point.y();
  }
}

void CAllTimeStatsDialog::plot_data()
{
  for (auto * series : mChart->series()) mChart->removeSeries(series);
  SPlotRange range;
  for (uint32_t idx = 0; idx < mPlayerList.size(); idx++)
  {
    if (mSelectedPlayers.contains(mPlayerList.at(idx)))
    {
      if (mUi->comboBoxGame->currentIndex() == 1)
      {
        switch(mUi->comboBoxStats->currentIndex())
        {
        case 0:
          mChart->addSeries(mPlotData.at(idx).HitsPerRound);
          get_min_max(range, mPlotData.at(idx).HitsPerRound);
          break;
        case 1:
          mChart->addSeries(mPlotData.at(idx).DartsPerLegAvgCricket);
          get_min_max(range, mPlotData.at(idx).DartsPerLegAvgCricket);
          break;
        }
      }
      else
      {
        switch(mUi->comboBoxStats->currentIndex())
        {
        case 0:
          mChart->addSeries(mPlotData.at(idx).Avg3Dart);
          get_min_max(range, mPlotData.at(idx).Avg3Dart);
          break;
        case 1:
          mChart->addSeries(mPlotData.at(idx).First9Avg);
          get_min_max(range, mPlotData.at(idx).First9Avg);
          break;
        case 2:
          mChart->addSeries(mPlotData.at(idx).CheckoutRate);
          get_min_max(range, mPlotData.at(idx).CheckoutRate);
          break;
        case 3:
          mChart->addSeries(mPlotData.at(idx).DartsPerLegAvgX01);
          get_min_max(range, mPlotData.at(idx).DartsPerLegAvgX01);
          break;
        }
      }
    }
  }

  for (auto & axis : mChart->axes()) mChart->removeAxis(axis);
  auto axisX = new QValueAxis();
  auto axisY = new QValueAxis();
  axisX->setTitleText("Game");
  axisX->setLabelFormat("%d");

  if (range.MaxX < 5)
  {
    axisX->setRange(range.MinX, 5);
    axisX->setTickCount(5 - range.MinX + 1);
  }
  else
  {
    axisX->setRange(range.MinX, range.MaxX);
    axisX->setTickCount(range.MaxX - range.MinX + 1);
  }

  axisY->setRange(range.MinY - 1, range.MaxY);
  mChart->addAxis(axisX, Qt::AlignBottom);
  mChart->addAxis(axisY, Qt::AlignLeft);

  for (auto * series : mChart->series())
  {
    for (auto * axis : mChart->axes())
    {
      series->attachAxis(axis);
    }
  }
  mUi->graphicsView->resize(mChart->size().toSize());
}

void CAllTimeStatsDialog::player_selected_slot()
{
  mSelectedPlayers = {};
  for (auto * action : mUi->pushButtonSelectPlayers->menu()->actions())
  {
    if (action->isChecked()) mSelectedPlayers.append(action->text());
  }

  if (mUi->comboBoxGame->currentIndex() == 1)
  {
    mStatsModel->update_selected_players_cricket(mSelectedPlayers);
  }
  else
  {
    mStatsModel->update_selected_players_x01(mSelectedPlayers);
  }
  mUi->tableView->resizeColumnsToContents();
  plot_data();
}

void CAllTimeStatsDialog::game_selected_slot()
{
  bool isCricket = mUi->comboBoxGame->currentIndex() == 1;
  mStatsModel->update_selected_game(isCricket);
  mUi->tableView->resizeColumnsToContents();
  update_stats_combobox(isCricket);
  plot_data();
}

void CAllTimeStatsDialog::stat_selected_slot()
{
  plot_data();
}

