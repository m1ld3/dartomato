#include "stats_window_cricket.h"
#include "leg_stats_cricket_model.h"
#include "leg_scores_cricket_model.h"
#include "global_game_stats_cricket_model.h"
#include "global_segment_stats_cricket_model.h"
#include "ui_stats_window_cricket.h"

CStatsWindowCricket::CStatsWindowCricket(const CCricketClass::CPlayerData iPlayerData, QWidget * iParent)
 : QDialog(iParent)
 ,  mUi(new Ui::CStatsWindowCricket)
 ,  mPlayerData(iPlayerData)
{
  mUi->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  CStatsWindowCricket::calculate_segment_counts();
  compute_dart_count_of_won_legs();
  QObject::connect(mUi->legSelector, &QComboBox::currentIndexChanged, this, &CStatsWindowCricket::update_leg_history_slot);
  CStatsWindowCricket::init_leg_selector();
  CStatsWindowCricket::setup_table_views();
}

CStatsWindowCricket::~CStatsWindowCricket()
{
  delete mUi;
  delete mLegStatsModel;
  delete mLegScoresModel;
  delete mGlobalGameStatsModel;
  delete mGlobalSegmentStatsModel;
}

void CStatsWindowCricket::setup_table_views()
{
  mGlobalGameStatsData.HitsPerRound = mPlayerData.HitsPerRound;
  mGlobalGameStatsData.TotalHits = mPlayerData.TotalHits;
  mGlobalGameStatsData.LegsWon = mPlayerData.TotalLegsWon;
  mGlobalGameStatsData.NumLegs = mPlayerData.LegWonVec.size() > 0 ? mPlayerData.LegWonHistory.size() + 1 : mPlayerData.LegWonHistory.size();
  mGlobalGameStatsModel = new CGlobalGameStatsCricketModel(mGlobalGameStatsData, this);
  mGlobalSegmentStatsModel = new CGlobalSegmentStatsCricketModel(mSegmentCounts, this);
  mUi->tableViewGlobalGameStats->setModel(mGlobalGameStatsModel);
  mUi->tableViewGlobalSegmentStats->setModel(mGlobalSegmentStatsModel);
  mUi->tableViewLegStats->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewLegScores->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewGlobalGameStats->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewGlobalSegmentStats->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewGlobalGameStats->setColumnWidth(0, 100);
  mUi->tableViewGlobalGameStats->setColumnWidth(1, 80);
  mUi->tableViewGlobalGameStats->setColumnWidth(2, 70);
  mUi->tableViewGlobalGameStats->setColumnWidth(3, 80);
  mUi->tableViewGlobalGameStats->setColumnWidth(4, 70);
  mUi->tableViewGlobalGameStats->setColumnWidth(5, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(0, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(1, 75);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(2, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(3, 75);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(4, 50);
  mUi->tableViewGlobalSegmentStats->setColumnWidth(5, 75);
}

void CStatsWindowCricket::init_leg_selector()
{
  uint32_t numberOfLegs = mPlayerData.ScoresOfCurrentLeg.size() > 0 ? mPlayerData.ScoringHistory.size() + 1 : mPlayerData.ScoringHistory.size();
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

void CStatsWindowCricket::update_leg_history(int iIndex)
{
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ScoringHistory;
  QVector<QVector<uint32_t>> totalHits = mPlayerData.HitsHistory;
  if (mPlayerData.ScoresOfCurrentLeg.size()) totalDarts.append(mPlayerData.ScoresOfCurrentLeg);
  if (mPlayerData.HitsOfCurrentLeg.size()) totalHits.append(mPlayerData.HitsOfCurrentLeg);

  if (totalDarts.size() >= iIndex + 1 && totalHits.size() >= iIndex + 1)
  {
    uint32_t numberOfDarts = (totalDarts.at(iIndex).size() - 1) * 3 + totalDarts.at(iIndex).back().size();
    uint32_t hits = std::accumulate(totalHits.at(iIndex).begin(), totalHits.at(iIndex).end(), 0.0);
    mLegStatsData.HitsPerRound = numberOfDarts > 0 ? 3 * static_cast<double>(hits) / static_cast<double>(numberOfDarts) : 0.0;

    if (!mLegScoresModel)
    {
      mLegScoresModel = new CLegScoresCricketModel(totalDarts.at(iIndex), this);
      mUi->tableViewLegScores->setModel(mLegScoresModel);
      mUi->tableViewLegScores->setColumnWidth(0, 25);
      mUi->tableViewLegScores->setColumnWidth(1, 100);
    }
    else
    {
      mLegScoresModel->update(totalDarts.at(iIndex));
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
    mLegStatsModel = new CLegStatsCricketModel(mLegStatsData, this);
    mUi->tableViewLegStats->setModel(mLegStatsModel);
    mUi->tableViewLegStats->setColumnWidth(0, 150);
    mUi->tableViewLegStats->setColumnWidth(1, 50);
  }
  else
  {
    mLegStatsModel->update(mLegStatsData);
  }
}

void CStatsWindowCricket::calculate_segment_counts()
{
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ScoringHistory;
  if (mPlayerData.ScoresOfCurrentLeg.size()) totalDarts.append(mPlayerData.ScoresOfCurrentLeg);
  QVector<QVector<QString>> totalDartsFlat = {};
  for (const auto & darts : totalDarts) totalDartsFlat.append(darts);

  for (const auto & darts : totalDartsFlat)
  {
    for (const auto & dart : darts)
    {
      if (dart == "") continue;
      int idx = dart.mid(1).toUInt() % 15;
      if (idx == 10 || dart.mid(1).toUInt() == 0) idx = 6;
      if (dart[0] == 'd')      mSegmentCounts.at(idx + 7) += 1;
      else if (dart[0] == 't') mSegmentCounts.at(idx + 14) += 1;
      else                     mSegmentCounts.at(idx) += 1;
    }
  }
}

void CStatsWindowCricket::compute_dart_count_of_won_legs()
{
  mDartCountOfWonLegs = {};

  for (uint32_t idx = 0; idx < mPlayerData.LegWonHistory.size(); idx++)
  {
    if (mPlayerData.LegWonHistory.at(idx).back() == true)
    {
      mDartCountOfWonLegs.append((mPlayerData.ScoringHistory.at(idx).size() - 1) * 3 + mPlayerData.ScoringHistory.at(idx).back().size());
    }
  }
}

double CStatsWindowCricket::compute_average(QVector<uint32_t> iScoresOfLeg)
{
  double avg;
  double n = static_cast<double>(iScoresOfLeg.size());
  if (n > 0) avg = std::accumulate(iScoresOfLeg.begin(), iScoresOfLeg.end(), 0.0) / n;
  else avg = 0.0;
  return avg;
}

uint32_t CStatsWindowCricket::compute_dart_count_of_indexed_leg(uint32_t iIndex)
{
  QVector<QVector<QString>> dartsOfIndexedLeg;
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ScoringHistory;
  if (mPlayerData.ScoresOfCurrentLeg.size()) dartsOfAllLegs.append(mPlayerData.ScoresOfCurrentLeg);
  if (dartsOfAllLegs.size()) dartsOfIndexedLeg = dartsOfAllLegs.at(iIndex);
  if (dartsOfIndexedLeg.size()) return (dartsOfIndexedLeg.size() - 1) * 3 + dartsOfIndexedLeg.last().size();
  return 0;
}
