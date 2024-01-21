#include "cricket_class.h"
#include "cricket_mainwindow.h"
#include <cmath>

CCricketClass::CCricketClass(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings)
  : QObject(iParent)
  , mGameWindow(static_cast<CCricketMainWindow*>(iParent))
  , mPlayerNumber(iPlayerNumber-1)
  , mSettings(iSettings)
{
  mMarginLegs = std::ceil(mSettings.mLegs/2.0);
  mMarginSets = std::ceil(mSettings.mSets/2.0);
  mHitsPerRound = compute_hits_per_round(0,0);
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
    notify_game_won(mPlayerNumber);
  }

  return temp;
}

void CCricketClass::notify_game_won(uint32_t iPlayerNumber)
{
  mGameWindow->handle_game_won(iPlayerNumber);
}

void CCricketClass::update_history()
{
  mScoringHistory.push_back(mScoreLegs);
}

void CCricketClass::update_darts(QVector<QString> iDarts)
{
  mScoreLegs.push_back(iDarts);
  mTotalScores.push_back(iDarts);
}

void CCricketClass::reset_score()
{
  mScoreLegs = {};
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

void CCricketClass::perform_undo_step()
{
  mScoreLegs.pop_back();
  mTotalScores.pop_back();
  mScoreArray.pop_back();

  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mSlotHistory.at(i).pop_back();
    mExtraPointsHistory.at(i).pop_back();
    mSlotArray.at(i) = mSlotHistory.at(i).back();
    mExtraPointsArray.at(i) = mExtraPointsHistory.at(i).back();
  }

  set_score();
  mTotalDarts -= mNumberOfDartsArray.back();
  mNumberOfHitsArray.pop_back();
  mTotalHits = mNumberOfHitsArray.back();
  mNumberOfDartsArray.pop_back();
  mLegWinArray.pop_back();

  if (mTotalDarts > 0)
  {
    mHitsPerRound = 3 * static_cast<double>(mTotalHits) / static_cast<double>(mTotalDarts);
  }
  else
  {
    mHitsPerRound = 0.0;
  }
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
  mSlotHistory.at(static_cast<uint32_t>(iSlot)).push_back(iHits);
}

void CCricketClass::set_extra_points(const ECricketSlots iSlot, uint32_t iPoints)
{
  if (!mSettings.mCutThroat)
  {
    mExtraPointsArray.at(static_cast<uint32_t>(iSlot)) = iPoints;
  }
  else
  {
    mExtraPointsArray.at(static_cast<uint32_t>(iSlot)) += iPoints;
  }
  mExtraPointsHistory.at(static_cast<uint32_t>(iSlot)).push_back(mExtraPointsArray.at(static_cast<uint32_t>(iSlot)));
}



uint32_t CCricketClass::get_extra_points(const ECricketSlots iSlots) const
{
  return mExtraPointsArray.at(static_cast<uint32_t>(iSlots));
}

uint32_t CCricketClass::get_total_hits() const
{
  return mTotalHits;
}

uint32_t CCricketClass::get_total_darts() const
{
  return mTotalDarts;
}

void CCricketClass::set_score()
{
  mScore = 0;
  for (uint32_t i = 0; i < static_cast<uint32_t>(ECricketSlots::SLOT_MAX); i++)
  {
    mScore += mExtraPointsArray.at(i);
  }
  mScoreArray.push_back(mScore);
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
  return mScoreLegs;
}

QVector<QVector<QVector<QString>>> CCricketClass::get_scoring_history() const
{
  return mScoringHistory;
}

void CCricketClass::set_leg_win_array(bool iFinished)
{
  mLegWinArray.push_back(iFinished);
}

void CCricketClass::set_total_hits(uint32_t iHits)
{
  mTotalHits = iHits;
  mNumberOfHitsArray.push_back(mTotalHits);
}

void CCricketClass::set_total_darts(uint32_t iDarts)
{
  mTotalDarts = iDarts;
  mNumberOfDartsArray.push_back(mTotalDarts);
}

void CCricketClass::undo()
{
  if (mScoreLegs.size() > 0)
  {
    perform_undo_step();
  }
  else if (mScoringHistory.size() > 0)
  {
    handle_undo_after_legset_win();
  }
}

void CCricketClass::handle_undo_after_legset_win()
{
  mScoreLegs = mScoringHistory.back();
  mScoringHistory.pop_back();
  if (mScoreLegs.size() > 0)
  {
    bool finished = mLegWinArray.back();
    perform_undo_step();
    if (finished)
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
  }
}

double CCricketClass::compute_hits_per_round(uint32_t iNumberOfDarts, uint32_t iTotalHits)
{
  mTotalDarts += iNumberOfDarts;
  mNumberOfDartsArray.push_back(iNumberOfDarts);
  mTotalHits = iTotalHits;
  mNumberOfHitsArray.push_back(mTotalHits);

  if (iNumberOfDarts > 0)
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
