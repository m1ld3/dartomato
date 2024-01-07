#include "playerclass.h"
#include <cmath>
#include <QDebug>

CX01Class::CX01Class(uint32_t iPlayerNumber, const CSettings & ipSettings)
  : mpSettings(ipSettings)
  , mRemaining(static_cast<uint32_t>(mpSettings.mGame))
  , mMarginLegs(std::ceil(mpSettings.mLegs/2.0))
  , mMarginSets(std::ceil(mpSettings.mSets/2.0))
  , mPlayerNumber(iPlayerNumber-1)
{
  mRemainingPointsOfCurrentLeg.push_back(mRemaining);
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
    emit signal_game_won(mPlayerNumber);
  }
  return hasWonSet;
}

uint32_t CX01Class::set_score(uint32_t score)
{
  mScoresOfCurrentLeg.push_back(score);
  mAllScoresFlat.push_back(score);
  mRemaining -= score;
  mRemainingPointsOfCurrentLeg.push_back(mRemaining);
  return mRemaining;
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
  mRemaining = static_cast<uint32_t>(mpSettings.mGame);
  mRemainingPointsOfCurrentLeg = {mRemaining};
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

void CX01Class::perform_undo_step()
{
  mThrownDartsOfCurrentLeg.pop_back();
  mRemainingPointsOfCurrentLeg.pop_back();
  mRemaining = mRemainingPointsOfCurrentLeg.back();
  mScoresOfCurrentLeg.pop_back();
  mAllScoresFlat.pop_back();
  mThrownDartsOfAllLegsFlat.pop_back();
  mTotalDarts -= mNumberOfDartsArray.back();
  mNumberOfDartsArray.pop_back();
  mCheckoutAttempts -= mCheckoutAttemptsArray.back();
  mCheckoutAttemptsArray.pop_back();
  mCheckouts -= mCheckoutsArray.back();
  mCheckoutsArray.pop_back();

  compute_checkout();

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

QVector<uint32_t> CX01Class::get_score_leg() const
{
  return mScoresOfCurrentLeg;
}

QString CX01Class::get_checkout_attempts() const
{
  return QString::number(mCheckouts) + " / " + QString::number(mCheckoutAttempts);
}

void CX01Class::undo_last_won_leg_or_set()
{
  if (mTotalLegsWon % mMarginLegs == 0)
  {
    mTotalLegsWon -= 1;
    mLegsWonPerSet = mMarginLegs -1;
    mSetsWon -= 1;
  }
  else
  {
    mTotalLegsWon -= 1;
    mLegsWonPerSet -= 1;
  }
}

void CX01Class::undo()
{
  if (mScoresOfCurrentLeg.size() > 0)
  {
    perform_undo_step();
  }
  else if (mAllScoresOfAllLegs.size() > 0)
  {
    mScoresOfCurrentLeg = mAllScoresOfAllLegs.back();
    mThrownDartsOfCurrentLeg = mThrownDartsOfAllLegs.back();
    mRemainingPointsOfCurrentLeg = mRemainingPointsOfAllLegs.back();
    mThrownDartsOfAllLegs.pop_back();
    mRemainingPointsOfAllLegs.pop_back();
    mAllScoresOfAllLegs.pop_back();

    if (mScoresOfCurrentLeg.size() > 0)
    {
      if (mRemainingPointsOfCurrentLeg.back() == 0)
      {
        undo_last_won_leg_or_set();
      }
      perform_undo_step();
    }
  }
}

void CX01Class::compute_averages(uint32_t numberofdarts)
{
  mTotalDarts += numberofdarts;
  mNumberOfDartsArray.push_back(numberofdarts);
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
    mCheckout = (static_cast<double> (mCheckouts) / static_cast<double> (mCheckoutAttempts)) * 100.0;
  }
  else
  {
    mCheckout = 0;
  }
}

void CX01Class::update_checkout(uint32_t checkoutattempts, uint32_t success)
{
  mCheckoutAttempts += checkoutattempts;
  mCheckoutAttemptsArray.push_back(checkoutattempts);
  mCheckoutsArray.push_back(success);
  mCheckouts += success;

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
  return mCheckout;
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
  return mRemaining;
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
