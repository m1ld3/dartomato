#include "stats_window_x01.h"
#include "leg_stats_x01_model.h"
#include "leg_scores_x01_model.h"
#include "global_game_stats_x01_model.h"
#include "global_score_stats_x01_model.h"
#include "global_segment_stats_x01_model.h"
#include "ui_stats_window_x01.h"

CStatsWindowX01::CStatsWindowX01(const CX01Class::CPlayerData iPlayerData, QWidget * iParent)
 : QDialog(iParent)
 , mUi(new Ui::CStatsWindowX01)
 , mPlayerData(iPlayerData)
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

CStatsWindowX01::~CStatsWindowX01()
{
  delete mUi;
  delete mLegStatsModel;
  delete mLegScoresModel;
  delete mGlobalGameStatsModel;
  delete mGlobalScoreStatsModel;
  delete mGlobalSegmentStatsModel;
}

void CStatsWindowX01::setup_table_views()
{
  mGlobalGameStatsData.Avg3Dart = mPlayerData.Avg3Dart;
  mGlobalGameStatsData.Avg1Dart = mPlayerData.Avg1Dart;
  mGlobalGameStatsData.First9Avg = mPlayerData.First9Avg;
  mGlobalGameStatsData.LegsWon = mPlayerData.TotalLegsWon;
  mGlobalGameStatsData.NumLegs = mPlayerData.ScoresOfCurrentLeg.size() > 0 ? mPlayerData.AllScoresOfAllLegs.size() + 1 : mPlayerData.AllScoresOfAllLegs.size();
  mGlobalGameStatsData.CheckoutAttempts = mPlayerData.CheckoutAttempts;
  mGlobalGameStatsData.CheckoutHits = mPlayerData.CheckoutHits;;
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
}

void CStatsWindowX01::init_leg_selector()
{
  uint32_t numberOfLegs = mPlayerData.ScoresOfCurrentLeg.size() > 0 ? mPlayerData.AllScoresOfAllLegs.size() + 1 : mPlayerData.AllScoresOfAllLegs.size();
  if (numberOfLegs == 0)
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
    mUi->legSelector->setCurrentIndex(numberOfLegs - 1);
  }
}

void CStatsWindowX01::update_leg_history(int iIndex)
{
  QVector<QVector<uint32_t>> totalScores = mPlayerData.AllScoresOfAllLegs;
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ThrownDartsOfAllLegs;
  if (mPlayerData.ScoresOfCurrentLeg.size()) totalScores.append(mPlayerData.ScoresOfCurrentLeg);
  if (mPlayerData.ThrownDartsOfCurrentLeg.size()) totalDarts.append(mPlayerData.ThrownDartsOfCurrentLeg);

  if (totalScores.size() >= iIndex + 1 && totalDarts.size() >= iIndex + 1)
  {
    uint32_t numberOfDarts = (totalDarts.at(iIndex).size() - 1) * 3 + totalDarts.at(iIndex).back().size();
    mLegStatsData.Avg1Dart = std::accumulate(totalScores.at(iIndex).begin(), totalScores.at(iIndex).end(), 0.0) / numberOfDarts;
    mLegStatsData.Avg3Dart = 3 * mLegStatsData.Avg1Dart;
    compute_first9_leg_average(totalScores.at(iIndex));
    if (!mLegScoresModel)
    {
      mLegScoresModel = new CLegScoresX01Model(totalScores.at(iIndex), totalDarts.at(iIndex), this);
      mUi->tableViewLegScores->setModel(mLegScoresModel);
      mUi->tableViewLegScores->setColumnWidth(0, 25);
      mUi->tableViewLegScores->setColumnWidth(1, 40);
      mUi->tableViewLegScores->setColumnWidth(2, 100);
    }
    else
    {
      mLegScoresModel->update(totalScores.at(iIndex), totalDarts.at(iIndex));
    }
  }

  if (mDartCountOfWonLegs.size())
  {
    mLegStatsData.AvgLegDartCount = compute_average(mDartCountOfWonLegs);
    mLegStatsData.BestWonLegDartCount = *std::min_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
    mLegStatsData.WorstWonLegDartCount = *std::max_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
  }
  mLegStatsData.DartCountOfCurrentLeg = compute_dart_count_of_indexed_leg(iIndex);

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
}

void CStatsWindowX01::compute_first9_leg_average(const QVector<uint32_t> &iScores)
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

void CStatsWindowX01::count_scores()
{
  std::map<uint32_t, uint32_t> scoreCounts = calculate_score_counts();

  if (mPlayerData.AllScoresFlat.size() > 0)
  {
    std::map<uint32_t, uint32_t>::iterator it;
    for (it = scoreCounts.begin(); it != scoreCounts.end(); it++)
    {
      if (it->first <  20)                     mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0))   += it->second;
      if (it->first >= 20  && it->first < 40)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_20))  += it->second;
      if (it->first >= 40  && it->first < 60)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_40))  += it->second;
      if (it->first >= 60  && it->first < 80)  mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_60))  += it->second;
      if (it->first >= 80  && it->first < 100) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_80))  += it->second;
      if (it->first >= 100 && it->first < 120) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_100)) += it->second;
      if (it->first >= 120 && it->first < 140) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_120)) += it->second;
      if (it->first >= 140 && it->first < 160) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_140)) += it->second;
      if (it->first >= 160 && it->first < 180) mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_160)) += it->second;
      if (it->first == 180)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_180))  += it->second;
      if (it->first == 140)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_140))  += it->second;
      if (it->first == 120)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_120))  += it->second;
      if (it->first == 100)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_100))  += it->second;
      if (it->first ==  85)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_85))   += it->second;
      if (it->first ==  81)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_81))   += it->second;
      if (it->first ==  60)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_60))   += it->second;
      if (it->first ==  45)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_45))   += it->second;
      if (it->first ==  41)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_41))   += it->second;
      if (it->first ==  30)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_30))   += it->second;
      if (it->first ==  26)                    mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_26))   += it->second;
    }
  }
}

std::map<uint32_t, uint32_t> CStatsWindowX01::calculate_score_counts()
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
      int idx = 0;
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
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ThrownDartsOfAllLegs;
  if (mPlayerData.ThrownDartsOfCurrentLeg.size()) dartsOfAllLegs.append(mPlayerData.ThrownDartsOfCurrentLeg);
  QVector<QVector<uint32_t>> remainingPointsOfAllLegs = mPlayerData.RemainingPointsOfAllLegs;
  if (mPlayerData.RemainingPointsOfCurrentLeg.size()) remainingPointsOfAllLegs.append(mPlayerData.RemainingPointsOfCurrentLeg);
  QVector<QVector<uint32_t>> allScoresOfAllLegs = mPlayerData.AllScoresOfAllLegs;
  if (mPlayerData.ScoresOfCurrentLeg.size()) allScoresOfAllLegs.append(mPlayerData.ScoresOfCurrentLeg);
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

  if (mAllCheckouts.size() > 0) mGlobalGameStatsData.HighestCheckout = *std::max_element(mAllCheckouts.begin(), mAllCheckouts.end());
}

double CStatsWindowX01::compute_average(QVector<uint32_t> iScoresOfLeg)
{
  double avg;
  double n = static_cast<double>(iScoresOfLeg.size());
  if (n > 0) avg = std::accumulate(iScoresOfLeg.begin(), iScoresOfLeg.end(), 0.0) / n;
  else avg = 0.0;
  return avg;
}

uint32_t CStatsWindowX01::compute_dart_count_of_indexed_leg(uint32_t iIndex)
{
  QVector<QVector<QString>> dartsOfIndexedLeg;
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ThrownDartsOfAllLegs;
  if (mPlayerData.ThrownDartsOfCurrentLeg.size()) dartsOfAllLegs.append(mPlayerData.ThrownDartsOfCurrentLeg);
  if (dartsOfAllLegs.size()) dartsOfIndexedLeg = dartsOfAllLegs.at(iIndex);
  if (dartsOfIndexedLeg.size()) return (dartsOfIndexedLeg.size() - 1) * 3 + dartsOfIndexedLeg.last().size();
  return 0;
}
