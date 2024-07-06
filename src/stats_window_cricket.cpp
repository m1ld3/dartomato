#include "stats_window_cricket.h"
#include "leg_stats_cricket_model.h"
#include "leg_scores_cricket_model.h"
#include "global_game_stats_cricket_model.h"
#include "global_segment_stats_cricket_model.h"
#include "ui_stats_window_cricket.h"

#ifndef TESTING
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
  get_global_game_stats_data();
  CStatsWindowCricket::setup_table_views();
}
#else
CStatsWindowCricket::CStatsWindowCricket(const CCricketClass::CPlayerData iPlayerData, QWidget * iParent)
 :  mPlayerData(iPlayerData)
{}
#endif

CStatsWindowCricket::~CStatsWindowCricket()
{
#ifndef TESTING
  delete mUi;
#endif
  delete mLegStatsModel;
  delete mLegScoresModel;
  delete mGlobalGameStatsModel;
  delete mGlobalSegmentStatsModel;
}

void CStatsWindowCricket::get_global_game_stats_data()
{
  mGlobalGameStatsData.HitsPerRound = mPlayerData.HitsPerRound;
  mGlobalGameStatsData.TotalHits = mPlayerData.TotalHits;
  mGlobalGameStatsData.LegsWon = mPlayerData.TotalLegsWon;
  mGlobalGameStatsData.NumLegs = mPlayerData.LegWonVec.size() > 0 ? mPlayerData.LegWonHistory.size() + 1 : mPlayerData.LegWonHistory.size();
}

void CStatsWindowCricket::setup_table_views()
{
#ifndef TESTING
  mGlobalGameStatsModel = new CGlobalGameStatsCricketModel(mGlobalGameStatsData, this);
  mGlobalSegmentStatsModel = new CGlobalSegmentStatsCricketModel(mSegmentCounts, this);
  mUi->tableViewGlobalGameStats->setModel(mGlobalGameStatsModel);
  mUi->tableViewGlobalSegmentStats->setModel(mGlobalSegmentStatsModel);
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
#endif
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

void CStatsWindowCricket::compute_hits_per_round(const QVector<QVector<QString>> & iTotalDarts, const QVector<uint32_t> & iTotalHits)
{
  uint32_t numberOfDarts = (iTotalDarts.size() - 1) * 3 + iTotalDarts.back().size();
  uint32_t hits = std::accumulate(iTotalHits.begin(), iTotalHits.end(), 0.0);
  mLegStatsData.HitsPerRound = numberOfDarts > 0 ? 3 * static_cast<double>(hits) / static_cast<double>(numberOfDarts) : 0.0;
}

void CStatsWindowCricket::update_leg_scores_table_view(const QVector<QVector<QString>> & iTotalDarts)
{
#ifndef TESTING
  if (!mLegScoresModel)
  {
    mLegScoresModel = new CLegScoresCricketModel(iTotalDarts, this);
    mUi->tableViewLegScores->setModel(mLegScoresModel);
    mUi->tableViewLegScores->setColumnWidth(0, 25);
    mUi->tableViewLegScores->setColumnWidth(1, 100);
  }
  else
  {
    mLegScoresModel->update(iTotalDarts);
  }
#endif
}

void CStatsWindowCricket::update_leg_stats_table_view()
{
#ifndef TESTING
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
#endif
}

void CStatsWindowCricket::update_leg_history(int iIndex)
{
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ScoringHistory;
  QVector<QVector<uint32_t>> totalHits = mPlayerData.HitsHistory;
  if (mPlayerData.ScoresOfCurrentLeg.size()) totalDarts.append(mPlayerData.ScoresOfCurrentLeg);
  if (mPlayerData.HitsOfCurrentLeg.size()) totalHits.append(mPlayerData.HitsOfCurrentLeg);

  if (totalDarts.size() >= iIndex + 1 && totalHits.size() >= iIndex + 1)
  {
    compute_hits_per_round(totalDarts.at(iIndex), totalHits.at(iIndex));
    update_leg_scores_table_view(totalDarts.at(iIndex));
    mLegStatsData.AvgLegDartCount = static_cast<double>(mPlayerData.TotalDarts) / totalDarts.size();
  }

  mLegStatsData.DartCountOfCurrentLeg = compute_dart_count_of_indexed_leg(iIndex);
  update_leg_stats_table_view();
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
      if (idx == 10) idx = 6;
      if (dart.mid(1).toUInt() == 0) idx = 20;
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
  if (mDartCountOfWonLegs.size())
  {
    mLegStatsData.BestWonLegDartCount = *std::min_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
    mLegStatsData.WorstWonLegDartCount = *std::max_element(mDartCountOfWonLegs.begin(), mDartCountOfWonLegs.end());
  }
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
