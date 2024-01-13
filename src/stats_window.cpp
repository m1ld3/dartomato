#include "stats_window.h"
#include "ui_stats_window.h"

CStatsWindow::CStatsWindow(QWidget * iParent, CX01Class * iPlayer)
  : QDialog(iParent)
  , mUi(new Ui::CStatsWindow)
  , mPlayer(iPlayer)
{
  mUi->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  count_scores();
  calculate_segment_counts();
  set_stats_labels();
  QObject::connect(mUi->legSelector, &QComboBox::currentIndexChanged, this, &CStatsWindow::update_leg_history);
  display_current_leg_scores();
  compute_dart_count_and_checkouts();
  display_best_and_worst_leg_dart_count();
  mUi->labelWorstLegIcon->setPixmap(mPixMap.scaled(20, 20, Qt::KeepAspectRatio));
  mUi->labelBestLegIcon->setPixmap(mPixMap.scaled(20, 20, Qt::KeepAspectRatio));
  mUi->labelLegAvgIcon->setPixmap(mPixMap.scaled(20, 20, Qt::KeepAspectRatio));
  mUi->labelCurrentLegDartsIcon->setPixmap(mPixMap.scaled(20, 20, Qt::KeepAspectRatio));
}

CStatsWindow::~CStatsWindow()
{
  delete mUi;
}

void CStatsWindow::set_label_checkout(double iCheckout, const QString & iCheckoutAttempts)
{
  QString checkstr = QString::number(iCheckout, 'f', 2) + "%  (" + iCheckoutAttempts + ")";
  mUi->labelCheckoutInput->setText(checkstr);
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
  mUi->labelLegAvg1Input->setText(QString::number(0.0, 'f', 2));
  mUi->labelLegAvg3Input->setText(QString::number(0.0, 'f', 2));
  compute_dart_count_of_indexed_leg(iIndex);
  mUi->labelCurrentLegDartsInput->setText(QString::number(mDartCountOfCurrentLeg));
  QVector<uint32_t> legscores = mPlayer->get_scores_of_current_leg();
  QVector<QVector<uint32_t>> totalscores = mPlayer->get_all_scores_of_all_legs();

  if (legscores.size()) totalscores.append(legscores);
  if (totalscores.size() >= iIndex + 1)
  {
    for (int i = 0; i < totalscores.at(iIndex).size(); i++)
    {
      QString line = QString::number(i + 1) + ": " + QString::number(totalscores.at(iIndex)[i]);
      set_text(line);
    }

    QVector<QVector<QString>> legdarts = mPlayer->get_thrown_darts_of_current_leg();
    QVector<QVector<QVector<QString>>> totaldarts = mPlayer->get_thrown_darts_of_all_legs();
    if (legdarts.size()) totaldarts.append(legdarts);
    uint32_t numberOfDarts = (totaldarts.at(iIndex).size() - 1) * 3 + totaldarts.at(iIndex).back().size();
    double avg1dart = std::accumulate(totalscores.at(iIndex).begin(), totalscores.at(iIndex).end(), 0.0) / numberOfDarts;
    double avg3dart = 3 * avg1dart;
    mUi->labelLegAvg1Input->setText(QString::number(avg1dart, 'f', 2));
    mUi->labelLegAvg3Input->setText(QString::number(avg3dart, 'f', 2));
  }
}

void CStatsWindow::count_scores()
{
  std::map<uint32_t, uint32_t> scoreCounts = calculate_score_counts();
  QVector<uint32_t> allScores = mPlayer->get_total_scores_flat();

  if (allScores.size() > 0)
  {
    std::map<uint32_t, uint32_t>::iterator it;
    for (it = scoreCounts.begin(); it != scoreCounts.end(); it++)
    {
      if (it->first < 20)                      mScoreCounts.at(static_cast<int>(EScoreCountsIdx::PLUS_0))   += it->second;
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
  QVector<uint32_t> allScores = mPlayer->get_total_scores_flat();
  std::map<uint32_t, uint32_t> scoreCounts;
  for (auto & score : allScores) ++scoreCounts[score];

  return scoreCounts;
}

void CStatsWindow::calculate_segment_counts()
{
  QVector<QVector<QString>> thrownDarts = mPlayer->get_darts();

  for (const auto & darts : thrownDarts)
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
  mUi->label1DartAvgInput->setText(QString::number(mPlayer->get_avg1dart(), 'f', 2));
  mUi->label3DartAvgInput->setText(QString::number(mPlayer->get_avg3dart(), 'f', 2));
  set_label_checkout(mPlayer->get_checkout(), mPlayer->get_checkout_attempts());
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

void CStatsWindow::display_current_leg_scores()
{
  QVector<uint32_t> currentLegScores = mPlayer->get_scores_of_current_leg();
  QVector<QVector<uint32_t>> allScoresOfAllLegs = mPlayer->get_all_scores_of_all_legs();
  uint32_t numberOfLegs = currentLegScores.size() > 0 ? allScoresOfAllLegs.size() + 1 : allScoresOfAllLegs.size();
  init_leg_selector(numberOfLegs);
}

void CStatsWindow::compute_dart_count_and_checkouts()
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
  mUi->labelLegAvgInput->setText(QString::number(compute_average(mDartCountOfWonLegs), 'f', 2));
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
    mUi->labelBestLegInput->setText(QString::number(bestLeg));
    mUi->labelWorstLegInput->setText(QString::number(worstLeg));
  }
}

void CStatsWindow::display_highest_checkout()
{
  uint32_t highestCheckout = 0;
  if (mAllCheckouts.size() > 0) highestCheckout = *std::max_element(mAllCheckouts.begin(), mAllCheckouts.end());
  mUi->labelHighestCheckoutInput->setText(QString::number(highestCheckout));
}

void CStatsWindow::compute_dart_count_of_indexed_leg(uint32_t iIndex)
{
  QVector<QVector<QString>> dartsOfCurrentLeg = mPlayer->get_thrown_darts_of_current_leg();
  QVector<QVector<QString>> dartsOfIndexedLeg;
  QVector<QVector<QVector<QString>>> dartsOfAllLegs = mPlayer->get_thrown_darts_of_all_legs();
  if (dartsOfCurrentLeg.size()) dartsOfAllLegs.append(dartsOfCurrentLeg);
  if (dartsOfAllLegs.size()) dartsOfIndexedLeg = dartsOfAllLegs.at(iIndex);
  if (dartsOfIndexedLeg.size()) mDartCountOfCurrentLeg = (dartsOfIndexedLeg.size() - 1) * 3 + dartsOfIndexedLeg.last().size();
}
