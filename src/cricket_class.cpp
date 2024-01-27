#include "cricket_class.h"
#include "cricket_mainwindow.h"
#include <cmath>

CCricketClass::CCricketClass(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings)
  : QObject(iParent)
  , mGameWindow(static_cast<CCricketMainWindow*>(iParent))
  , mPlayerNumber(iPlayerNumber - 1)
  , mSettings(iSettings)
{
  mMarginLegs = std::ceil(mSettings.Legs / 2.0);
  mMarginSets = std::ceil(mSettings.Sets / 2.0);
  mHitsPerRound = compute_hits_per_round(0, 0);
}

bool CCricketClass::increase_setslegs()
{
  mLegsWonPerSet += 1;
  mTotalLegsWon += 1;
  bool temp = false;

  if (((mLegsWonPerSet % mMarginLegs) == 0) && (mLegsWonPerSet > 0))
  {
    mSetsWon += 1;
    mLegsWonPerSet = 0;
    temp = true;
  }

  if (mSetsWon == mMarginSets)
  {
    notify_game_won();
  }

  return temp;
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
  mScoresOfCurrentLeg = iData.ScoreOfCurrentLegs;
  mScoringHistory = iData.ScoringHistory;
  mSlotArray = iData.SlotArray;
  mExtraPointsArray = iData.ExtraPointsArray;
}

CCricketClass::CPlayerData CCricketClass::create_snapshot() const
{
  return CPlayerData(mSetsWon, mLegsWonPerSet, mTotalLegsWon, mTotalDarts, mScore, mTotalHits,
                     mHitsPerRound, mScoresOfCurrentLeg, mScoringHistory, mSlotArray, mExtraPointsArray);
}

void CCricketClass::notify_game_won()
{
  mGameWindow->handle_game_won(mPlayerNumber);
}

void CCricketClass::update_darts(QVector<QString> iDarts)
{
  mScoresOfCurrentLeg.push_back(iDarts);
}

void CCricketClass::reset_score()
{
  mScoringHistory.push_back(mScoresOfCurrentLeg);
  mScoresOfCurrentLeg = {};
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotArray.at(i) = 0;
    mExtraPointsArray.at(i) = 0;
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
  mSlotArray.at(static_cast<uint32_t>(iSlot)) = iHits;
}

void CCricketClass::set_extra_points(const ECricketSlots iSlot, uint32_t iPoints)
{
  if (!mSettings.CutThroat)
  {
    mExtraPointsArray.at(static_cast<uint32_t>(iSlot)) = iPoints;
  }
  else
  {
    mExtraPointsArray.at(static_cast<uint32_t>(iSlot)) += iPoints;
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

void CCricketClass::set_score()
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

void CCricketClass::set_total_hits(uint32_t iHits)
{
  mTotalHits = iHits;
}

double CCricketClass::compute_hits_per_round(uint32_t iNumberOfDarts, uint32_t iTotalHits)
{
  mTotalDarts += iNumberOfDarts;
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
