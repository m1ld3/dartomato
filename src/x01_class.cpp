#include "x01_class.h"
#include "x01_mainwindow.h"
#include <cmath>

CX01Class::CX01Class(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings)
  : QObject(iParent)
  , mGameWindow(static_cast<CX01MainWindow*>(iParent))
  , mSettings(iSettings)
  , mRemainingPoints(static_cast<uint32_t>(mSettings.mGame))
  , mMarginLegs(std::ceil(mSettings.mLegs/2.0))
  , mMarginSets(std::ceil(mSettings.mSets/2.0))
  , mPlayerNumber(iPlayerNumber-1)
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
  if (mSetsWon == mMarginSets)
  {
    notify_game_won(mPlayerNumber);
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
                     mTotalDarts, mAvg1Dart,
                     mAvg3Dart, mCheckoutRate,
                     mScoresOfCurrentLeg, mAllScoresOfAllLegs,
                     mAllScoresFlat, mThrownDartsOfCurrentLeg,
                     mThrownDartsOfAllLegsFlat, mThrownDartsOfAllLegs,
                     mRemainingPointsOfCurrentLeg, mRemainingPointsOfAllLegs);
}

void CX01Class::notify_game_won(uint32_t iPlayerNumber)
{
  mGameWindow->handle_game_won(iPlayerNumber);
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

QVector<QVector<QString>> CX01Class::get_darts() const
{
  return mThrownDartsOfAllLegsFlat;
}

QVector<uint32_t> CX01Class::get_total_scores_flat() const
{
  return mAllScoresFlat;
}

void CX01Class::update_history()
{
  mAllScoresOfAllLegs.push_back(mScoresOfCurrentLeg);
  mThrownDartsOfAllLegs.push_back(mThrownDartsOfCurrentLeg);
  mRemainingPointsOfAllLegs.push_back(mRemainingPointsOfCurrentLeg);
}

void CX01Class::reset_score()
{
  mRemainingPoints = static_cast<uint32_t>(mSettings.mGame);
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

QString CX01Class::get_checkout_attempts_str() const
{
  return QString::number(mCheckoutHits) + " / " + QString::number(mCheckoutAttempts);
}

uint32_t CX01Class::get_checkout_attempts() const
{
  return mCheckoutAttempts;
}

void CX01Class::compute_averages(uint32_t numberofdarts)
{
  mTotalDarts += numberofdarts;
  double n = static_cast<double>(mTotalDarts);

  if (mTotalDarts > 0)
  {
    mAvg1Dart = std::accumulate(mAllScoresFlat.begin(),mAllScoresFlat.end(), 0.0)/n;
  }
  else
  {
    mAvg1Dart = 0.0;
  }
  mAvg3Dart = 3 * mAvg1Dart;
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

QVector<uint32_t> CX01Class::get_scores_of_current_leg() const
{
  return mScoresOfCurrentLeg;
}

QVector<QVector<QString>> CX01Class::get_thrown_darts_of_current_leg() const
{
  return mThrownDartsOfCurrentLeg;
}

QVector<QVector<uint32_t>> CX01Class::get_all_scores_of_all_legs() const
{
  return mAllScoresOfAllLegs;
}

QVector<QVector<QVector<QString>>> CX01Class::get_thrown_darts_of_all_legs() const
{
  return mThrownDartsOfAllLegs;
}

uint32_t CX01Class::get_remaining() const
{
  return mRemainingPoints;
}

QVector<uint32_t> CX01Class::get_remaining_points_of_current_leg() const
{
  return mRemainingPointsOfCurrentLeg;
}

QVector<QVector<uint32_t> > CX01Class::get_remaining_points_of_all_legs() const
{
  return mRemainingPointsOfAllLegs;
}

uint32_t CX01Class::get_legs() const
{
  return mLegsWonPerSet;
}

uint32_t CX01Class::get_sets() const
{
  return mSetsWon;
}
