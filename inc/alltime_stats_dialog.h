#ifndef ALLTIME_STATS_DIALOG_H
#define ALLTIME_STATS_DIALOG_H

#include <QDialog>
#include "game_data_handler.h"
#include "alltime_player_stats_model.h"
#include <QLineSeries>
#include <QChartView>
#include <QPointer>

namespace Ui
{
  class CAllTimeStatsDialog;
}

class CAllTimeStatsDialog : public QDialog
{
  Q_OBJECT

public:

  explicit CAllTimeStatsDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent = nullptr);
  ~CAllTimeStatsDialog() override;

private slots:

  void player_selected_slot();
  void game_selected_slot() const;
  void stat_selected_slot() const;

private:

  struct SPlotData
  {
    QPointer<QLineSeries> Avg3Dart = new QLineSeries();
    QPointer<QLineSeries> First9Avg = new QLineSeries();
    QPointer<QLineSeries> DartsPerLegAvgX01 = new QLineSeries();
    QPointer<QLineSeries> DartsPerLegAvgCricket = new QLineSeries();
    QPointer<QLineSeries> CheckoutRate = new QLineSeries();
    QPointer<QLineSeries> HitsPerRound = new QLineSeries();
  };

  struct SPlotRange
  {
    int MinX = INT_MAX;
    int MaxX = 0;
    float MinY = 100000;
    float MaxY = 0;
  };

  void setup_drop_down_menu();
  void prepare_plot_data();
  void update_stats_combobox(bool iIsCricket) const;
  void plot_data() const;
  static void get_min_max(SPlotRange & oPlotRange, const QLineSeries * iSeries);

  Ui::CAllTimeStatsDialog * mUi;
  CGameDataHandler & mGameDataHandler;
  QVector<CGameDataHandler::SGameData> mGameData;
  CAlltimePlayerStatsModel * mStatsModel = nullptr;
  QVector<SPlotData> mPlotData = {};
  QStringList mSelectedPlayers = {};
  QStringList mPlayerList = {};
  QPointer<QChart> mChart = new QChart;
};

#endif  // ALLTIME_STATS_DIALOG_H
