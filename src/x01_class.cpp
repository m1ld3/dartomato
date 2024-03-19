#include "x01_class.h"
#include "x01_mainwindow.h"
#include <cmath>

CX01Class::CX01Class(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings)
  : QObject(iParent)
  , mSettings(iSettings)
  , mRemainingPoints(static_cast<uint32_t>(mSettings.Game))
  , mMarginLegs(std::ceil(mSettings.Legs / 2.0))
  , mMarginSets(std::ceil(mSettings.Sets / 2.0))
  , mPlayerNumber(iPlayerNumber)
  , mRemainingPointsOfCurrentLeg({mRemainingPoints})
{
  compute_averages(0);
  update_checkout(0, 0);
}

bool CX01Class::increment_won_legs_and_check_if_set_won()
{
  mLegsWonPerSet = (mLegsWonPerSet + 1) % mMarginLegs;
  mTotalLegsWon += 1;
  bool hasWonSet = false;
  if (mLegsWonPerSet == 0)
  {
    mSetsWon += 1;
    hasWonSet = true;
  }
  return hasWonSet;
}

void CX01Class::restore_state(CPlayerData iData)
{
  mSetsWon = iData.SetsWon;
  mLegsWonPerSet = iData.LegsWonPerSet;
  mTotalLegsWon = iData.TotalLegsWon;
  mRemainingPoints = iData.RemainingPoints;
  mCheckoutAttempts = iData.CheckoutAttempts;
  mCheckoutHits = iData.CheckoutHits;
  mTotalDarts = iData.TotalDarts;
  mAvg1Dart = iData.Avg1Dart;
  mAvg3Dart = iData.Avg3Dart;
  mCheckoutRate = iData.CheckoutRate;
  mFirst9Avg = iData.First9Avg;
  mScoresOfCurrentLeg = iData.ScoresOfCurrentLeg;
  mAllScoresOfAllLegs = iData.AllScoresOfAllLegs;
  mAllScoresFlat = iData.AllScoresFlat;
  mThrownDartsOfCurrentLeg = iData.ThrownDartsOfCurrentLeg;
  mThrownDartsOfAllLegsFlat = iData.ThrownDartsOfAllLegsFlat;
  mThrownDartsOfAllLegs = iData.ThrownDartsOfAllLegs;
  mRemainingPointsOfCurrentLeg = iData.RemainingPointsOfCurrentLeg;
  mRemainingPointsOfAllLegs = iData.RemainingPointsOfAllLegs;
}

CX01Class::CPlayerData CX01Class::create_snapshot() const
{
  return CPlayerData(mSetsWon, mLegsWonPerSet,
                     mTotalLegsWon, mRemainingPoints,
                     mCheckoutAttempts, mCheckoutHits,
                     mTotalDarts, mAvg1Dart, mAvg3Dart, mCheckoutRate,
                     mFirst9Avg, mScoresOfCurrentLeg, mAllScoresOfAllLegs,
                     mAllScoresFlat, mThrownDartsOfCurrentLeg,
                     mThrownDartsOfAllLegsFlat, mThrownDartsOfAllLegs,
                     mRemainingPointsOfCurrentLeg, mRemainingPointsOfAllLegs);
}

bool CX01Class::has_won_game() const
{
  return mSetsWon == mMarginSets;
}

uint32_t CX01Class::set_score(uint32_t score)
{
  mScoresOfCurrentLeg.push_back(score);
  mAllScoresFlat.push_back(score);
  mRemainingPoints -= score;
  mRemainingPointsOfCurrentLeg.push_back(mRemainingPoints);
  return mRemainingPoints;
}

void CX01Class::set_darts(QVector<QString> darts)
{
  mThrownDartsOfCurrentLeg.append(darts);
  mThrownDartsOfAllLegsFlat.append(darts);
}

void CX01Class::reset_score()
{
  mAllScoresOfAllLegs.push_back(mScoresOfCurrentLeg);
  mThrownDartsOfAllLegs.push_back(mThrownDartsOfCurrentLeg);
  mRemainingPointsOfAllLegs.push_back(mRemainingPointsOfCurrentLeg);
  mRemainingPoints = static_cast<uint32_t>(mSettings.Game);
  mRemainingPointsOfCurrentLeg = {mRemainingPoints};
  mScoresOfCurrentLeg = {};
  mThrownDartsOfCurrentLeg = {};
}

void CX01Class::reset_legs()
{
  mLegsWonPerSet = 0;
}

uint32_t CX01Class::get_player_number() const
{
  return mPlayerNumber;
}

void CX01Class::compute_averages(uint32_t numberofdarts)
{
  mTotalDarts += numberofdarts;
  double n = static_cast<double>(mTotalDarts);

  if (mTotalDarts > 0)
  {
    mAvg1Dart = std::accumulate(mAllScoresFlat.begin(), mAllScoresFlat.end(), 0.0) / n;
  }
  else
  {
    mAvg1Dart = 0.0;
  }
  mAvg3Dart = 3 * mAvg1Dart;
  compute_first9_avg();
}

void CX01Class::compute_first9_avg()
{
  uint32_t points = 0;
  for (const auto & scores : mAllScoresOfAllLegs)
  {
    for (size_t i = 0; i < 3; i++)
    {
      if (i < scores.size())
      {
        points += scores.at(i);
      }
    }
  }

  int idx = 0;
  for (const auto score : mScoresOfCurrentLeg)
  {
    if (idx < 3) points += score;
    idx++;
  }

  auto legs = mScoresOfCurrentLeg.size() > 0 ? mAllScoresOfAllLegs.size() + 1 : mAllScoresOfAllLegs.size();
  if (legs == 0) mFirst9Avg = 0.0;
  else mFirst9Avg = static_cast<double>(points) / (legs * 3);
}

void CX01Class::compute_checkout()
{
  if (mCheckoutAttempts > 0)
  {
    mCheckoutRate = (static_cast<double> (mCheckoutHits) / static_cast<double> (mCheckoutAttempts)) * 100.0;
  }
  else
  {
    mCheckoutRate = 0;
  }
}

void CX01Class::update_checkout(uint32_t iCheckoutattempts, uint32_t iSuccess)
{
  mCheckoutAttempts += iCheckoutattempts;
  mCheckoutHits += iSuccess;

  compute_checkout();
}

double CX01Class::get_avg1dart() const
{
  return mAvg1Dart;
}

double CX01Class::get_avg3dart() const
{
  return mAvg3Dart;
}

double CX01Class::get_checkout() const
{
  return mCheckoutRate;
}

uint32_t CX01Class::get_remaining() const
{
  return mRemainingPoints;
}

uint32_t CX01Class::get_legs() const
{
  return mLegsWonPerSet;
}

uint32_t CX01Class::get_sets() const
{
  return mSetsWon;
}
