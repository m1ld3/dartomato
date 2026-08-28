#include "stats_window_x01.h"

#include <utility>
#include "leg_stats_x01_model.h"
#include "leg_scores_x01_model.h"
#include "global_game_stats_x01_model.h"
#include "global_score_stats_x01_model.h"
#include "global_segment_stats_x01_model.h"
#include "ui_stats_window_x01.h"

#ifndef TESTING
CStatsWindowX01::CStatsWindowX01(CX01Class::CPlayerData  iPlayerData, QWidget * iParent)
 : QDialog(iParent)
 , mUi(new Ui::CStatsWindowX01)
 , mPlayerData(std::move(iPlayerData))
{
  mUi->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  count_scores();
  CStatsWindowX01::calculate_segment_counts();
  compute_dart_count_and_checkouts();
  QObject::connect(mUi->legSelector, &QComboBox::currentIndexChanged, this, &CStatsWindowX01::update_leg_history_slot);
  CStatsWindowX01::init_leg_selector();
  CStatsWindowX01::setup_table_views();
}
#else
CStatsWindowX01::CStatsWindowX01(const CX01Class::CPlayerData iPlayerData, QWidget * iParent)
  : mPlayerData(iPlayerData)
{}
#endif

CStatsWindowX01::~CStatsWindowX01()
{
#ifndef TESTING
  delete mUi;
#endif
  delete mLegStatsModel;
  delete mLegScoresModel;
  delete mGlobalGameStatsModel;
  delete mGlobalScoreStatsModel;
  delete mGlobalSegmentStatsModel;
}

void CStatsWindowX01::setup_table_views()
{
#ifndef TESTING
  mGlobalGameStatsModel = new CGlobalGameStatsX01Model(mGlobalGameStatsData, this);
  mGlobalScoreStatsModel = new CGlobalScoreStatsX01Model(mScoreCounts, this);
  mGlobalSegmentStatsModel = new CGlobalSegmentStatsX01Model(mSegmentCounts, this);
  mUi->tableViewGlobalGameStats->setModel(mGlobalGameStatsModel);
  mUi->tableViewGlobalScoreStats->setModel(mGlobalScoreStatsModel);
  mUi->tableViewGlobalSegmentStats->setModel(mGlobalSegmentStatsModel);
  mUi->tableViewGlobalGameStats->setColumnWidth(0, 125);
  mUi->tableViewGlobalGameStats->setColumnWidth(1, 125);
  mUi->tableViewGlobalGameStats->setColumnWidth(2, 125);
  mUi->tableViewGlobalGameStats->setColumnWidth(3, 125);
  mUi->tableViewGlobalScoreStats->setColumnWidth(0, 50);
  mUi->tableViewGlobalScoreStats->setColumnWidth(1, 75);
  mUi->tableViewGlobalScoreStats->setColumnWidth(2, 50);
  mUi->tableViewGlobalScoreStats->setColumnWidth(3, 75);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(0, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(1, 75);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(2, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(3, 75);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(4, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(5, 75);

  mUi->tableViewGlobalGameStats->resizeRowsToContents();
  mUi->tableViewGlobalSegmentStats->resizeRowsToContents();
  mUi->tableViewGlobalScoreStats->resizeRowsToContents();
#endif
}

void CStatsWindowX01::init_leg_selector()
{
  if (const uint32_t numberOfLegs = !mPlayerData.ScoresOfCurrentLeg.empty() ? mPlayerData.AllScoresOfAllLegs.size() + 1 : mPlayerData.AllScoresOfAllLegs.size(); numberOfLegs == 0)
  {
    mUi->legSelector->addItem("1");
    mUi->legSelector->setCurrentIndex(0);
  }
  else
  {
    for (uint32_t i = 1; i < numberOfLegs + 1; i++)
    {
      mUi->legSelector->addItem(QString::number(i));
    }
    mUi->legSelector->setCurrentIndex(static_cast<int>(numberOfLegs) - 1);
  }
}

void CStatsWindowX01::update_leg_scores_table_view(const QVector<uint32_t> & iScores, const QVector<QVector<QString>> & iDarts)
{
#ifndef TESTING
  if (!mLegScoresModel)
  {
    mLegScoresModel = new CLegScoresX01Model(iScores, iDarts, this);
    mUi->tableViewLegScores->setModel(mLegScoresModel);
    mUi->tableViewLegScores->setColumnWidth(0, 25);
    mUi->tableViewLegScores->setColumnWidth(1, 40);
    mUi->tableViewLegScores->setColumnWidth(2, 100);
  }
  else
  {
    mLegScoresModel->update(iScores, iDarts);
  }
  mUi->tableViewLegScores->resizeRowsToContents();
#endif
}

void CStatsWindowX01::update_leg_stats_table_view()
{
#ifndef TESTING
  if (!mLegStatsModel)
  {
    mLegStatsModel = new CLegStatsX01Model(mLegStatsData, this);
    mUi->tableViewLegStats->setModel(mLegStatsModel);
    mUi->tableViewLegStats->setColumnWidth(0, 150);
    mUi->tableViewLegStats->setColumnWidth(1, 50);
  }
  else
  {
    mLegStatsModel->update(mLegStatsData);
  }
  mUi->tableViewLegStats->resizeRowsToContents();
#endif
}

void CStatsWindowX01::update_leg_history(const int iIndex)
{
  QVector<QVector<uint32_t>> totalScores = mPlayerData.AllScoresOfAllLegs;
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ThrownDartsOfAllLegs;
  if (!mPlayerData.ScoresOfCurrentLeg.empty()) totalScores.append(mPlayerData.ScoresOfCurrentLeg);
  if (!mPlayerData.ThrownDartsOfCurrentLeg.empty()) totalDarts.append(mPlayerData.ThrownDartsOfCurrentLeg);

  if (totalScores.size() >= iIndex + 1 && totalDarts.size() >= iIndex + 1)
  {
    compute_dart_averages(totalScores.at(iIndex), totalDarts.at(iIndex));
    compute_first9_leg_average(totalScores.at(iIndex));
    update_leg_scores_table_view(totalScores.at(iIndex), totalDarts.at(iIndex));
  }

  mLegStatsData.DartCountOfCurrentLeg = compute_dart_count_of_indexed_leg(iIndex);
  update_leg_stats_table_view();
}

void CStatsWindowX01::compute_first9_leg_average(const QVector<uint32_t> & iScores)
{
  uint32_t points = 0;
  int idx = 0;
  for (const auto score : iScores)
  {
    if (idx < 3) points += score;
    idx++;
  }

  mLegStatsData.First9Avg = static_cast<double>(points) / 3;
}

void CStatsWindowX01::compute_dart_averages(const QVector<uint32_t> & iScores, const QVector<QVector<QString> > & iDarts)
{
  uint32_t numberOfDarts = (iDarts.size() - 1) * 3 + iDarts.back().size();
  mLegStatsData.Avg1Dart = std::accumulate(iScores.begin(), iScores.end(), 0.0) / numberOfDarts;
  mLegStatsData.Avg3Dart = 3 * mLegStatsData.Avg1Dart;
}


void CStatsWindowX01::count_scores()
{
  std::map<uint32_t, uint32_t> scoreCounts = calculate_score_counts();

  if (!mPlayerData.AllScoresFlat.empty())
  {
    for (auto & [score, count] : scoreCounts)
    {
      if (score <  20)                     mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0))   += count;
      if (score >= 20  && score < 40)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_20))  += count;
      if (score >= 40  && score < 60)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_40))  += count;
      if (score >= 60  && score < 80)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_60))  += count;
      if (score >= 80  && score < 100) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_80))  += count;
      if (score >= 100 && score < 120) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_100)) += count;
      if (score >= 120 && score < 140) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_120)) += count;
      if (score >= 140 && score < 160) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_140)) += count;
      if (score >= 160 && score < 180) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_160)) += count;
      if (score == 180)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_180))  += count;
      if (score == 140)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_140))  += count;
      if (score == 120)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_120))  += count;
      if (score == 100)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_100))  += count;
      if (score ==  85)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_85))   += count;
      if (score ==  81)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_81))   += count;
      if (score ==  60)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_60))   += count;
      if (score ==  45)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_45))   += count;
      if (score ==  41)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_41))   += count;
      if (score ==  30)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_30))   += count;
      if (score ==  26)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_26))   += count;
    }
  }
}

std::map<uint32_t, uint32_t> CStatsWindowX01::calculate_score_counts() const
{
  std::map<uint32_t, uint32_t> scoreCounts;
  for (auto & score : mPlayerData.AllScoresFlat) ++scoreCounts[score];

  return scoreCounts;
}

void CStatsWindowX01::calculate_segment_counts()
{
  for (const auto & darts : mPlayerData.ThrownDartsOfAllLegsFlat)
  {
    for (const auto & dart : darts)
    {
      uint idx = 0;
      if (dart[0] == 'd')
      {
        idx = dart.mid(1).toUInt() / 2;
        if (idx == 25) idx = static_cast<int>(EDartCountsIdx::SEG_25);
      }
      else if (dart[0] == 't')
      {
        idx = dart.mid(1).toUInt() / 3;
        if (idx >= 15) mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_TRIPLES)) += 1;
      }
      else
      {
        idx = dart.mid(1).toUInt();
        if (idx == 25) idx = static_cast<int>(EDartCountsIdx::SEG_25);
      }
      mSegmentCounts.at(idx) += 1;
    }
  }
}

void CStatsWindowX01::compute_dart_count_and_checkouts()
{
  mGlobalGameStatsData.Avg3Dart = mPlayerData.Avg3Dart;
  mGlobalGameStatsData.Avg1Dart = mPlayerData.Avg1Dart;
  mGlobalGameStatsData.First9Avg = mPlayerData.First9Avg;
  mGlobalGameStatsData.LegsWon = mPlayerData.TotalLegsWon;
  mGlobalGameStatsData.NumLegs = !mPlayerData.ScoresOfCurrentLeg.empty() ? mPlayerData.AllScoresOfAllLegs.size() + 1 : mPlayerData.AllScoresOfAllLegs.size();
  mGlobalGameStatsData.CheckoutAttempts = mPlayerData.CheckoutAttempts;
  mGlobalGameStatsData.CheckoutHits = mPlayerData.CheckoutHits;  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ThrownDartsOfAllLegs;
  if (!mPlayerData.ThrownDartsOfCurrentLeg.empty()) dartsOfAllLegs.append(mPlayerData.ThrownDartsOfCurrentLeg);
  QVector<QVector<uint32_t>> remainingPointsOfAllLegs = mPlayerData.RemainingPointsOfAllLegs;
  if (!mPlayerData.RemainingPointsOfCurrentLeg.empty()) remainingPointsOfAllLegs.append(mPlayerData.RemainingPointsOfCurrentLeg);
  QVector<QVector<uint32_t>> allScoresOfAllLegs = mPlayerData.AllScoresOfAllLegs;
  if (!mPlayerData.ScoresOfCurrentLeg.empty()) allScoresOfAllLegs.append(mPlayerData.ScoresOfCurrentLeg);
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

  if (!mDartCountOfWonLegs.empty())
  {
    mLegStatsData.BestWonLegDartCount = *std::min_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
    mLegStatsData.WorstWonLegDartCount = *std::max_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
  }

  if (!dartsOfAllLegs.empty()) mLegStatsData.AvgLegDartCount = static_cast<double>(mPlayerData.TotalDarts) / static_cast<double>(dartsOfAllLegs.size());
  if (!mAllCheckouts.empty()) mGlobalGameStatsData.HighestCheckout = *std::max_element(mAllCheckouts.begin(), mAllCheckouts.end());
}

uint32_t CStatsWindowX01::compute_dart_count_of_indexed_leg(uint32_t iIndex)
{
  QVector<QVector<QString>> dartsOfIndexedLeg;
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ThrownDartsOfAllLegs;
  if (!mPlayerData.ThrownDartsOfCurrentLeg.empty()) dartsOfAllLegs.append(mPlayerData.ThrownDartsOfCurrentLeg);
  if (!dartsOfAllLegs.empty()) dartsOfIndexedLeg = dartsOfAllLegs.at(iIndex);
  if (!dartsOfIndexedLeg.empty()) return (dartsOfIndexedLeg.size() - 1) * 3 + dartsOfIndexedLeg.last().size();
  return 0;
}
