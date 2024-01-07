#include "statswindow.h"
#include "ui_statswindow.h"
#include <qscrollbar.h>

CStatsWindow::CStatsWindow(QWidget * iParent, CX01Class * iPlayer)
  : QDialog(iParent)
  , mUi(new Ui::CStatsWindow)
  , mPlayer(iPlayer)
{
  mUi->setupUi(this);
  mUi->scoresChartView->setMouseTracking(true);
  mUi->dartsChartView->setMouseTracking(true);
  setAttribute(Qt::WA_DeleteOnClose);
  mUi->scoresChartView->setRubberBand( QChartView::VerticalRubberBand);
  mUi->dartsChartView->setRubberBand( QChartView::VerticalRubberBand);

  create_scores_chart();
  create_darts_chart();

  connect(mUi->scrollBarScores, SIGNAL(valueChanged(int)), this, SLOT(scores_scrollbar_changed(int)));
  connect(mUi->scoresChartView->chart()->axisY(), SIGNAL(rangeChanged(const QString&, const QString&)), this, SLOT(y_axis_changed(const QString&, const QString&)));

  set_stats_labels();
  QObject::connect(mUi->legSelector, &QComboBox::currentIndexChanged, this, &CStatsWindow::update_leg_history);
  display_current_leg_scores();
  compute_dart_count_and_checkouts_of_won_legs();
  display_best_and_worst_leg_dart_count();

//  QObject::connect(mUi->tabWidget, &QTabWidget::currentChanged, this, [this](int index)
//  {
//    if (index == 0) update_leg_history(mUi->legSelector->currentIndex());
//  });
}

CStatsWindow::~CStatsWindow()
{
  delete mUi;
}

void CStatsWindow::set_label_1dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_1dartInput->setText(avgstr);
}

void CStatsWindow::set_label_3dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_3dartInput->setText(avgstr);
}

void CStatsWindow::set_label_checkout(double iCheckout)
{
  QString checkstr = QString::number(iCheckout, 'f', 2) + "%";
  mUi->label_checkoutInput->setText(checkstr);
}

void CStatsWindow::set_label_leg_1dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_legavg1_input->setText(avgstr);
}

void CStatsWindow::set_label_leg_3dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_legavg3_input->setText(avgstr);
}

void CStatsWindow::set_label_best_leg(uint32_t iNumberOfDarts)
{
  mUi->label_best_leg_input->setText(QString::number(iNumberOfDarts));
}

void CStatsWindow::set_label_worst_leg(uint32_t iNumberOfDarts)
{
  mUi->label_worst_leg_input->setText(QString::number(iNumberOfDarts));
}

void CStatsWindow::set_label_leg_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_leg_avg_input->setText(avgstr);
}

void CStatsWindow::set_label_checkout_attempts(QString iAttempts)
{
  mUi->label_checkout_attempts_input->setText(iAttempts);
}

void CStatsWindow::set_label_highest_checkout(uint32_t iCheckout)
{
  mUi->label_highest_checkout_input->setText(QString::number(iCheckout));
}

void CStatsWindow::set_label_180s(uint32_t iCount)
{
  mUi->label_180_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_160s(uint32_t iCount)
{
  mUi->label_160p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_140s(uint32_t iCount)
{
  mUi->label_140p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_120s(uint32_t iCount)
{
  mUi->label_120p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_100s(uint32_t iCount)
{
  mUi->label_100p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_80s(uint32_t iCount)
{
  mUi->label_80p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_60s(uint32_t iCount)
{
  mUi->label_60p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_40s(uint32_t iCount)
{
  mUi->label_40p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_20s(uint32_t iCount)
{
  mUi->label_20p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_0s(uint32_t iCount)
{
  mUi->label_0p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_text(QString iText)
{
  mUi->textBrowser->append(iText);
}

void CStatsWindow::clear_text()
{
  mUi->textBrowser->clear();
}

void CStatsWindow::init_leg_selector(uint32_t iNumberOfLegs)
{
  if (iNumberOfLegs == 0)
  {
    mUi->legSelector->addItem("1");
    mUi->legSelector->setCurrentIndex(0);
  }
  else
  {
    for (uint32_t i = 1; i < iNumberOfLegs + 1; i++)
    {
      mUi->legSelector->addItem(QString::number(i));
    }
    mUi->legSelector->setCurrentIndex(iNumberOfLegs - 1);
    }
}

void CStatsWindow::update_leg_history(int iIndex)
{
  clear_text();
  set_label_leg_1dart_avg(0.0);
  set_label_leg_3dart_avg(0.0);
  QVector<uint32_t> legscores = mPlayer->get_scores_of_current_leg();
  QVector<QVector<uint32_t>> totalscores = mPlayer->get_all_scores_of_all_legs();

  if (legscores.size()) totalscores.append(legscores);
  if (totalscores.size() >= iIndex + 1)
  {
    for (int i = 0; i < totalscores.at(iIndex).size(); i++)
    {
      QString line = QString::number(i+1) + ": " + QString::number(totalscores.at(iIndex)[i]);
      set_text(line);
    }
    QVector<QVector<QString>> legdarts = mPlayer->get_thrown_darts_of_current_leg();
    QVector<QVector<QVector<QString>>> totaldarts = mPlayer->get_thrown_darts_of_all_legs();
    if (legdarts.size()) totaldarts.append(legdarts);
    uint32_t numberOfDarts = (totaldarts.at(iIndex).size() - 1) * 3 + totaldarts.at(iIndex).back().size();
    double avg1dart = std::accumulate(totalscores.at(iIndex).begin(), totalscores.at(iIndex).end(), 0.0)/numberOfDarts;
    double avg3dart = 3 * avg1dart;
    set_label_leg_1dart_avg(avg1dart);
    set_label_leg_3dart_avg(avg3dart);
    }
}

void CStatsWindow::scores_scrollbar_changed(int iValue)
{
//  auto yAxis = static_cast<QBarCategoryAxis*>(mUi->scoresChartView->chart()->axisY());
//  auto categories = yAxis->categories();
//  if (yAxis->count() > 7)
//  {

//  }
//  if (qAbs(yAxis->count()/2 - +iValue) > 1)
//  {
//    ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
//    ui->plot->replot();
//    }
}

void CStatsWindow::y_axis_changed(const QString & iMinCat, const QString & iMaxCat)
{
  printf(iMinCat.toStdString().c_str());
  printf(iMaxCat.toStdString().c_str());
  display_highest_checkout();
}

void CStatsWindow::create_scores_chart()
{
  std::map<uint32_t, uint32_t> scoreCounts = calculate_score_counts();
  QVector<uint32_t> allScores = mPlayer->get_total_scores_flat();
  std::map<QString, uint32_t> scoreCountsStr = {};

  if (allScores.size() > 0)
  {
    std::map<uint32_t, uint32_t>::iterator it;
    for (it = scoreCounts.begin(); it != scoreCounts.end(); it++)
    {
      if (it->first < 20)                      mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0))   += it->second;
      if (it->first >= 20 && it->first < 40)   mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_20))  += it->second;
      if (it->first >= 40 && it->first < 60)   mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_40))  += it->second;
      if (it->first >= 60 && it->first < 80)   mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_60))  += it->second;
      if (it->first >= 80 && it->first < 100)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_80))  += it->second;
      if (it->first >= 100 && it->first < 120) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_100)) += it->second;
      if (it->first >= 120 && it->first < 140) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_120)) += it->second;
      if (it->first >= 140 && it->first < 160) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_140)) += it->second;
      if (it->first >= 160 && it->first < 180) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_160)) += it->second;
      if (it->first == 180)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_180))  += it->second;
      scoreCountsStr.insert({QString::number(it->first), it->second});
    }
  }

  QStringList categories;
  QPointer<CChart> chart = create_chart(scoreCountsStr, "Scoring Statistics", "Scores", categories);

  mUi->scoresChartView->setChart(chart);
  mUi->scoresChartView->setVerticalScrollBar(mUi->scrollBarScores);

//  QObject::connect(mScrollBarScores, &QScrollBar::valueChanged, this, [this, chart, categories](int value)
//  {
//    int totalCategories = categories.count();
//    int visibleCategories = mUi->scoresChartView->height() / totalCategories;
//    chart->axes(Qt::Vertical).back()->setRange(value, value + visibleCategories);
//  });
}

void CStatsWindow::create_darts_chart()
{
  QVector<QString> thrownDarts = process_thrown_darts();
  const std::map<QString, uint32_t> dartCounts = calculate_dart_counts(thrownDarts);

  QStringList categories;
  QPointer<CChart> chart = create_chart(dartCounts, "Single Dart Statistics", "Single Darts", categories);

  mUi->dartsChartView->setChart(chart);
  mUi->dartsChartView->setVerticalScrollBar(mUi->scrollBarDarts);

//  QObject::connect(mScrollBarDarts, &QScrollBar::valueChanged, this, [this, chart, categories](int value)
//  {
//    int totalCategories = categories.count();
//    int visibleCategories = mUi->dartsChartView->height() / totalCategories;
//    chart->axes(Qt::Vertical).back()->setRange(value, value + visibleCategories);
//  });
}

QPointer<CChart> CStatsWindow::create_chart(const std::map<QString, uint32_t> & iData, const QString & iTitle, const QString & iSeriesName, QStringList & oCategories)
{
  QPointer<QBarSet> barSet = new QBarSet(iSeriesName);
  QPointer<QHorizontalBarSeries> series = new QHorizontalBarSeries();
  QPointer<CChart> chart = new CChart();
  chart->addSeries(series);
  chart->setTitle(iTitle);
  chart->setAnimationOptions(QChart::SeriesAnimations);

  if (iData.size() > 0)
  {
    series->append(barSet);
    std::map<QString, uint32_t>::const_iterator it;
    for (it = iData.begin(); it != iData.end(); it++)
    {
      barSet->append(it->second);
      oCategories.append(it->first);
    }

    QPointer<QValueAxis> axisX = new QValueAxis();
    std::map<QString, uint32_t>::const_iterator best =
        std::max_element(iData.begin(), iData.end(),
        [] (const std::pair<QString, uint32_t>& a, const std::pair<QString, uint32_t>& b)->bool{return a.second < b.second;});
    qreal max = static_cast<qreal>(best->second);
    axisX->setRange(0, max);
    axisX->setTickType(QValueAxis::TicksFixed);
    axisX->setTickCount(std::min(std::max(2, int(max) + 1), 10));
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignTop);
    series->attachAxis(axisX);

    QPointer<QBarCategoryAxis> axisY = new QBarCategoryAxis();
    if (oCategories.size()) axisY->setMin(oCategories.first());
    axisY->append(oCategories);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->setAnimationOptions(QChart::NoAnimation);
    connect_hover_signals(barSet);
  }
  return chart;
}

std::map<uint32_t, uint32_t> CStatsWindow::calculate_score_counts()
{
  QVector<uint32_t> allScores = mPlayer->get_total_scores_flat();
  std::map<uint32_t, uint32_t> scoreCounts;
  for (auto & score : allScores) ++scoreCounts[score];

  return scoreCounts;
}

QVector<QString> CStatsWindow::process_thrown_darts()
{
  QVector<QVector<QString>> thrownDarts = mPlayer->get_darts();
  QVector<QString> thrownDartsFlat;

  for (const auto & darts : thrownDarts)
  {
    for (const auto &dart : darts)
    {
      if (dart[0] == 'd')
      {
        thrownDartsFlat.append("D" + QString::number(dart.mid(1).toInt() / 2));
      }
      else if (dart[0] == 't')
      {
        thrownDartsFlat.append("T" + QString::number(dart.mid(1).toInt() / 3));
      }
      else
      {
        thrownDartsFlat.append("S" + dart.mid(1));
      }
    }
  }

  return thrownDartsFlat;
}

std::map<QString, uint32_t> CStatsWindow::calculate_dart_counts(const QVector<QString> & iThrownDarts)
{
  std::map<QString, uint32_t> dartCounts;
  for (auto & dart : iThrownDarts) ++dartCounts[dart];

  return dartCounts;
}

void CStatsWindow::connect_hover_signals(QPointer<QBarSet> &iBarSet)
{
  QObject::connect(iBarSet, &QBarSet::hovered, this, [iBarSet](bool status, int index)
  {
    QPoint p = QCursor::pos();
    if (status)
    {
      QToolTip::showText(p, QString::number(iBarSet->at(index)));
    }
  });
}

void CStatsWindow::set_stats_labels()
{
  set_label_1dart_avg(mPlayer->get_avg1dart());
  set_label_3dart_avg(mPlayer->get_avg3dart());
  set_label_checkout(mPlayer->get_checkout());
  set_label_checkout_attempts(mPlayer->get_checkout_attempts());
  set_label_0s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0)));
  set_label_20s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_20)));
  set_label_40s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_40)));
  set_label_60s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_60)));
  set_label_80s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_80)));
  set_label_100s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_100)));
  set_label_120s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_120)));
  set_label_140s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_140)));
  set_label_160s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_160)));
  set_label_180s(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_180)));
}

void CStatsWindow::display_current_leg_scores()
{
  QVector<uint32_t> currentLegScores = mPlayer->get_scores_of_current_leg();
  QVector<QVector<uint32_t>> allScoresOfAllLegs = mPlayer->get_all_scores_of_all_legs();
  uint32_t numberOfLegs = currentLegScores.size() > 0 ? allScoresOfAllLegs.size() + 1 : allScoresOfAllLegs.size();
  init_leg_selector(numberOfLegs);
}

void CStatsWindow::compute_dart_count_and_checkouts_of_won_legs()
{
  QVector<QVector<QString>> dartsOfCurrentLeg = mPlayer->get_thrown_darts_of_current_leg();
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayer->get_thrown_darts_of_all_legs();
  if (dartsOfCurrentLeg.size()) dartsOfAllLegs.append(dartsOfCurrentLeg);
  QVector<uint32_t> remainingPointsOfCurrentLeg = mPlayer->get_remaining_points_of_current_leg();
  QVector<QVector<uint32_t>> remainingPointsOfAllLegs = mPlayer->get_remaining_points_of_all_legs();
  if (remainingPointsOfCurrentLeg.size()) remainingPointsOfAllLegs.append(remainingPointsOfCurrentLeg);
  QVector<uint32_t> currentLegScores = mPlayer->get_scores_of_current_leg();
  QVector<QVector<uint32_t>> allScoresOfAllLegs = mPlayer->get_all_scores_of_all_legs();
  if (currentLegScores.size()) allScoresOfAllLegs.append(currentLegScores);
  mDartCountOfWonLegs = {};
  mAllCheckouts = {};
  for (uint32_t idx = 0; idx < remainingPointsOfAllLegs.size(); idx++)
  {
    if (remainingPointsOfAllLegs.at(idx).back() == 0)
    {
      mDartCountOfWonLegs.append((dartsOfAllLegs.at(idx).size() - 1) * 3 + dartsOfAllLegs.at(idx).back().size());
      mAllCheckouts.append(allScoresOfAllLegs.at(idx).back());
    }
  }
  set_label_leg_avg(compute_average(mDartCountOfWonLegs));
}

double CStatsWindow::compute_average(QVector<uint32_t> iScoresOfLeg)
{
  double avg;
  double n = static_cast<double>(iScoresOfLeg.size());
  if (n > 0) avg = std::accumulate(iScoresOfLeg.begin(), iScoresOfLeg.end(), 0.0)/n;
  else avg = 0.0;
  return avg;
}

void CStatsWindow::display_best_and_worst_leg_dart_count()
{
  uint32_t bestLeg = 0;
  uint32_t worstLeg = 0;
  if (mDartCountOfWonLegs.size() > 0)
  {
    bestLeg = *std::min_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
    worstLeg = *std::max_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
    set_label_best_leg(bestLeg);
    set_label_worst_leg(worstLeg);
    }
}

void CStatsWindow::display_highest_checkout()
{
  uint32_t highestCheckout = 0;
  if (mAllCheckouts.size() > 0) highestCheckout = *std::max_element(mAllCheckouts.begin(), mAllCheckouts.end());
  set_label_highest_checkout(highestCheckout);
}
