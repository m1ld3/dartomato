#include "cricket_class.h"
#include "cricket_mainwindow.h"
#include <cmath>

CCricketClass::CCricketClass(uint32_t iPlayerNumber, const CSettings & iSettings)
  : mPlayerNumber(iPlayerNumber)
  , mSettings(iSettings)
{
  mMarginLegs = std::ceil(mSettings.Legs / 2.0);
  mMarginSets = std::ceil(mSettings.Sets / 2.0);
  mHitsPerRound = compute_hits_per_round(0, 0);
}

bool CCricketClass::increment_won_legs_and_check_if_set_won()
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

void CCricketClass::restore_state(CPlayerData iData)
{
  mSetsWon = iData.SetsWon;
  mLegsWonPerSet = iData.LegsWonPerSet;
  mTotalLegsWon = iData.TotalLegsWon;
  mTotalDarts = iData.TotalDarts;
  mScore = iData.Score;
  mTotalHits = iData.TotalHits;
  mHitsPerRound = iData.HitsPerRound;
  mScoresOfCurrentLeg = iData.ScoresOfCurrentLeg;
  mScoringHistory = iData.ScoringHistory;
  mHitsOfCurrentLeg = iData.HitsOfCurrentLeg;
  mHitsHistory = iData.HitsHistory;
  mSlotArray = iData.SlotArray;
  mExtraPointsArray = iData.ExtraPointsArray;
  mLegWonVec = iData.LegWonVec;
  mLegWonHistory = iData.LegWonHistory;
}

CCricketClass::CPlayerData CCricketClass::create_snapshot() const
{
  return CPlayerData(mSetsWon, mLegsWonPerSet, mTotalLegsWon, mTotalDarts,
                     mScore, mTotalHits, mHitsPerRound, mScoresOfCurrentLeg,
                     mScoringHistory, mHitsOfCurrentLeg, mHitsHistory, mSlotArray, mExtraPointsArray, mLegWonVec, mLegWonHistory);
}

bool CCricketClass::has_won_game() const
{
  return mSetsWon == mMarginSets;
}

void CCricketClass::update_darts(QVector<QString> iDarts)
{
  mScoresOfCurrentLeg.push_back(iDarts);
}

void CCricketClass::reset_score()
{
  mScoringHistory.push_back(mScoresOfCurrentLeg);
  mHitsHistory.push_back(mHitsOfCurrentLeg);
  mLegWonHistory.push_back(mLegWonVec);
  mScoresOfCurrentLeg = {};
  mHitsOfCurrentLeg = {};
  mLegWonVec = {};
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray[i] = 0;
    mExtraPointsArray[i] = 0;
  }
  mScore = 0;
}

void CCricketClass::reset_legs()
{
  mLegsWonPerSet = 0;
}

uint32_t CCricketClass::get_player_number() const
{
  return mPlayerNumber;
}

uint32_t CCricketClass::get_slot(const ECricketSlots iSlot) const
{
  return mSlotArray.at(static_cast<uint32_t>(iSlot));
}

void CCricketClass::set_slot(const ECricketSlots iSlot, uint32_t iHits)
{
  mSlotArray[static_cast<uint32_t>(iSlot)] = iHits;
}

void CCricketClass::set_extra_points(const ECricketSlots iSlot, uint32_t iPoints)
{
  if (!mSettings.CutThroat)
  {
    mExtraPointsArray[static_cast<uint32_t>(iSlot)] = iPoints;
  }
  else
  {
    mExtraPointsArray[static_cast<uint32_t>(iSlot)] += iPoints;
  }
}

uint32_t CCricketClass::get_extra_points(const ECricketSlots iSlots) const
{
  return mExtraPointsArray.at(static_cast<uint32_t>(iSlots));
}

uint32_t CCricketClass::get_total_hits() const
{
  return mTotalHits;
}

void CCricketClass::compute_score()
{
  mScore = 0;
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mScore += mExtraPointsArray.at(i);
  }
}

uint32_t CCricketClass::get_score() const
{
  return mScore;
}

double CCricketClass::get_hits_per_round() const
{
  return mHitsPerRound;
}

QVector<QVector<QString> > CCricketClass::get_score_legs() const
{
  return mScoresOfCurrentLeg;
}

QVector<QVector<QVector<QString>>> CCricketClass::get_scoring_history() const
{
  return mScoringHistory;
}

double CCricketClass::compute_hits_per_round(uint32_t iNumberOfDarts, uint32_t iTotalHits)
{
  mTotalDarts += iNumberOfDarts;
  mHitsOfCurrentLeg.append(iTotalHits - mTotalHits);
  mTotalHits = iTotalHits;

  if (mTotalDarts > 0)
  {
    mHitsPerRound = 3 * static_cast<double>(mTotalHits) / static_cast<double>(mTotalDarts);
  }
  else
  {
    mHitsPerRound = 0.0;
  }

  return mHitsPerRound;
}

uint32_t CCricketClass::get_legs() const
{
  return mLegsWonPerSet;
}

uint32_t CCricketClass::get_sets() const
{
  return mSetsWon;
}
