#include "stats_window.h"
#include "ui_stats_window.h"
#include "leg_stats_model.h"
#include "leg_scores_model.h"
#include "global_game_stats_model.h"

CStatsWindow::CStatsWindow(const CX01Class::CPlayerData & iPlayerData, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CStatsWindow)
  , mPlayerData(iPlayerData)
{
  mUi->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  count_scores();
  calculate_segment_counts();
  set_stats_labels();
  compute_dart_count_and_checkouts();
  mUi->tableViewLegStats->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewLegScores->setSelectionMode(QAbstractItemView::NoSelection);
  QObject::connect(mUi->legSelector, &QComboBox::currentIndexChanged, this, &CStatsWindow::update_leg_history);
  display_current_leg_stats();
  mGlobalGameStatsData.Avg3Dart = mPlayerData.Avg3Dart;
  mGlobalGameStatsData.Avg1Dart = mPlayerData.Avg1Dart;
  mGlobalGameStatsData.CheckoutAttempts = mPlayerData.CheckoutAttempts;
  mGlobalGameStatsData.CheckoutHits = mPlayerData.CheckoutHits;;
  mGlobalStatsModel = new CGlobalGameStatsModel(mGlobalGameStatsData, this);
  mUi->tableViewGlobalStats->setModel(mGlobalStatsModel);
  mUi->tableViewGlobalStats->setSelectionMode(QAbstractItemView::NoSelection);
  mUi->tableViewGlobalStats->setColumnWidth(0, 125);
  mUi->tableViewGlobalStats->setColumnWidth(1, 125);
  mUi->tableViewGlobalStats->setColumnWidth(2, 125);
  mUi->tableViewGlobalStats->setColumnWidth(3, 125);
}

CStatsWindow::~CStatsWindow()
{
  delete mUi;
  delete mLegStatsModel;
  delete mLegScoresModel;
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
  QVector<QVector<uint32_t>> totalScores = mPlayerData.AllScoresOfAllLegs;
  QVector<QVector<QVector<QString>>> totalDarts = mPlayerData.ThrownDartsOfAllLegs;
  if (mPlayerData.ScoresOfCurrentLeg.size()) totalScores.append(mPlayerData.ScoresOfCurrentLeg);
  if (mPlayerData.ThrownDartsOfCurrentLeg.size()) totalDarts.append(mPlayerData.ThrownDartsOfCurrentLeg);

  if (totalScores.size() >= iIndex + 1 && totalDarts.size() >= iIndex + 1)
  {
    uint32_t numberOfDarts = (totalDarts.at(iIndex).size() - 1) * 3 + totalDarts.at(iIndex).back().size();
    mLegStatsData.Avg1Dart = std::accumulate(totalScores.at(iIndex).begin(), totalScores.at(iIndex).end(), 0.0) / numberOfDarts;
    mLegStatsData.Avg3Dart = 3 * mLegStatsData.Avg1Dart;
    if (!mLegScoresModel)
    {
      mLegScoresModel = new CLegScoresModel(totalScores.at(iIndex), totalDarts.at(iIndex), this);
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
    mLegStatsModel = new CLegStatsModel(mLegStatsData, this);
    mUi->tableViewLegStats->setModel(mLegStatsModel);
    mUi->tableViewLegStats->setColumnWidth(0, 150);
    mUi->tableViewLegStats->setColumnWidth(1, 50);
  }
  else
  {
    mLegStatsModel->update(mLegStatsData);
  }
}

void CStatsWindow::count_scores()
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

std::map<uint32_t, uint32_t> CStatsWindow::calculate_score_counts()
{
  std::map<uint32_t, uint32_t> scoreCounts;
  for (auto & score : mPlayerData.AllScoresFlat) ++scoreCounts[score];

  return scoreCounts;
}

void CStatsWindow::calculate_segment_counts()
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
        if (idx >= 17) mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_TRIPLES)) += 1;
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

void CStatsWindow::set_stats_labels()
{
  mUi->label0pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0))));
  mUi->label20pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_20))));
  mUi->label40pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_40))));
  mUi->label60pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_60))));
  mUi->label80pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_80))));
  mUi->label100pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_100))));
  mUi->label120pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_120))));
  mUi->label140pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_140))));
  mUi->label160pInput->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_160))));
  mUi->label180Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_180))));
  mUi->label140Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_140))));
  mUi->label120Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_120))));
  mUi->label100Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_100))));
  mUi->label85Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_85))));
  mUi->label81Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_81))));
  mUi->label60Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_60))));
  mUi->label45Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_45))));
  mUi->label41Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_41))));
  mUi->label30Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_30))));
  mUi->label26Input->setText(QString::number(mScoreCounts.at(static_cast<int>(EScoreCountsIdx::THE_26))));

  mUi->label25Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_25))));
  mUi->label20Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_20))));
  mUi->label19Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_19))));
  mUi->label18Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_18))));
  mUi->label17Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_17))));
  mUi->label16Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_16))));
  mUi->label15Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_15))));
  mUi->label14Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_14))));
  mUi->label13Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_13))));
  mUi->label12Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_12))));
  mUi->label11Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_11))));
  mUi->label10Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_10))));
  mUi->label9Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_9))));
  mUi->label8Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_8))));
  mUi->label7Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_7))));
  mUi->label6Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_6))));
  mUi->label5Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_5))));
  mUi->label4Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_4))));
  mUi->label3Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_3))));
  mUi->label2Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_2))));
  mUi->label1Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_1))));
  mUi->label0Input->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_0))));
  mUi->labelHighTriplesInput->setText(QString::number(mSegmentCounts.at(static_cast<int>(EDartCountsIdx::SEG_TRIPLES))));
}

void CStatsWindow::display_current_leg_stats()
{
  uint32_t numberOfLegs = mPlayerData.ScoresOfCurrentLeg.size() > 0 ? mPlayerData.AllScoresOfAllLegs.size() + 1 : mPlayerData.AllScoresOfAllLegs.size();
  init_leg_selector(numberOfLegs);
}

void CStatsWindow::compute_dart_count_and_checkouts()
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

double CStatsWindow::compute_average(QVector<uint32_t> iScoresOfLeg)
{
  double avg;
  double n = static_cast<double>(iScoresOfLeg.size());
  if (n > 0) avg = std::accumulate(iScoresOfLeg.begin(), iScoresOfLeg.end(), 0.0) / n;
  else avg = 0.0;
  return avg;
}

uint32_t CStatsWindow::compute_dart_count_of_indexed_leg(uint32_t iIndex)
{
  QVector<QVector<QString>> dartsOfIndexedLeg;
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayerData.ThrownDartsOfAllLegs;
  if (mPlayerData.ThrownDartsOfCurrentLeg.size()) dartsOfAllLegs.append(mPlayerData.ThrownDartsOfCurrentLeg);
  if (dartsOfAllLegs.size()) dartsOfIndexedLeg = dartsOfAllLegs.at(iIndex);
  if (dartsOfIndexedLeg.size()) return (dartsOfIndexedLeg.size() - 1) * 3 + dartsOfIndexedLeg.last().size();
  return 0;
}
