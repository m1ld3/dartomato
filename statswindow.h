#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QString>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QToolTip>
#include <QtCharts/QValueAxis>
#include <QtCharts/QHorizontalBarSeries>
#include "chart.h"
#include "chartview.h"
#include "playerclass.h"

class CCallout;

namespace Ui
{
  class CStatsWindow;
}

class CStatsWindow : public QDialog
{
  Q_OBJECT

public:

  enum class EScoreCountsIdx
  {
      PLUS_0   = 0
    , PLUS_20  = 1
    , PLUS_40  = 2
    , PLUS_60  = 3
    , PLUS_80  = 4
    , PLUS_100 = 5
    , PLUS_120 = 6
    , PLUS_140 = 7
    , PLUS_160 = 8
    , THE_180  = 9
  };

  explicit CStatsWindow(QWidget * iParent = nullptr, CX01Class * iPlayer = nullptr);
  ~CStatsWindow() override;

private slots:

  void update_leg_history(int iIndex);
  void scores_scrollbar_changed(int iValue);
  void y_axis_changed(const QString & iMinCat, const QString & iMaxCat);

private:

  void set_label_1dart_avg(double iAvg);
  void set_label_3dart_avg(double iAvg);
  void set_label_checkout(double iCheckout);
  void set_label_leg_1dart_avg(double iAvg);
  void set_label_leg_3dart_avg(double iAvg);
  void set_label_best_leg(uint32_t iNumberOfDarts);
  void set_label_worst_leg(uint32_t iNumberOfDarts);
  void set_label_leg_avg(double iAvg);
  void set_label_checkout_attempts(QString iAttempts);
  void set_label_highest_checkout(uint32_t iCheckout);
  void set_label_180s(uint32_t iCount);
  void set_label_160s(uint32_t iCount);
  void set_label_140s(uint32_t iCount);
  void set_label_120s(uint32_t iCount);
  void set_label_100s(uint32_t iCount);
  void set_label_80s(uint32_t iCount);
  void set_label_60s(uint32_t iCount);
  void set_label_40s(uint32_t iCount);
  void set_label_20s(uint32_t iCount);
  void set_label_0s(uint32_t iCount);
  void set_text(QString iText);
  void clear_text();
  void init_leg_selector(uint32_t iNumberOfLegs);
  void create_scores_chart();
  void create_darts_chart();
  QPointer<CChart> create_chart(const std::map<QString, uint32_t> & iData, const QString & iTitle, const QString & iSeriesName, QStringList & oCategories);
  std::map<uint32_t, uint32_t> calculate_score_counts();
  QVector<QString> process_thrown_darts();
  std::map<QString, uint32_t> calculate_dart_counts(const QVector<QString> & iThrownDarts);
  void connect_hover_signals(QPointer<QBarSet> & iBarSet);
  void set_stats_labels();
  void display_current_leg_scores();
  void compute_dart_count_and_checkouts_of_won_legs();
  double compute_average(QVector<uint32_t> iScoresOfLeg);
  void display_best_and_worst_leg_dart_count();
  void display_highest_checkout();

private:

  Ui::CStatsWindow * mUi;
  CX01Class * mPlayer;
  std::array<uint32_t, 10> mScoreCounts = {};
  QVector<uint32_t> mDartCountOfWonLegs = {};
  QVector<uint32_t> mAllCheckouts = {};
};

#endif  // STATSWINDOW_H
