#include "cricketclass.h"
#include <cmath>
#include <QDebug>

CCricketClass::CCricketClass(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & ipSettings)
  : mPlayerNumber(iPlayerNumber-1)
  , mpSettings(ipSettings)
{
  mGameWindow = dynamic_cast<CCricketMainWindow*>(iParent);
  mMarginLegs = std::ceil(mpSettings.mLegs/2.0);
  mMarginSets = std::ceil(mpSettings.mSets/2.0);
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
    emit signal_game_won(mPlayerNumber);
  }

  return temp;
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
  mSlot15 = 0;
  mSlot16 = 0;
  mSlot17 = 0;
  mSlot18 = 0;
  mSlot19 = 0;
  mSlot20 = 0;
  mSlot25 = 0;
  mExtra15 = 0;
  mExtra16 = 0;
  mExtra17 = 0;
  mExtra18 = 0;
  mExtra19 = 0;
  mExtra20 = 0;
  mExtra25 = 0;
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
  mSlot15Array.pop_back();
  mSlot16Array.pop_back();
  mSlot17Array.pop_back();
  mSlot18Array.pop_back();
  mSlot19Array.pop_back();
  mSlot20Array.pop_back();
  mSlot25Array.pop_back();
  mExtra15Array.pop_back();
  mExtra16Array.pop_back();
  mExtra17Array.pop_back();
  mExtra18Array.pop_back();
  mExtra19Array.pop_back();
  mExtra20Array.pop_back();
  mExtra25Array.pop_back();
  mSlot15 = mSlot15Array.back();
  mSlot16 = mSlot16Array.back();
  mSlot17 = mSlot17Array.back();
  mSlot18 = mSlot18Array.back();
  mSlot19 = mSlot19Array.back();
  mSlot20 = mSlot20Array.back();
  mSlot25 = mSlot25Array.back();
  mExtra15 = mExtra15Array.back();
  mExtra16 = mExtra16Array.back();
  mExtra17 = mExtra17Array.back();
  mExtra18 = mExtra18Array.back();
  mExtra19 = mExtra19Array.back();
  mExtra20 = mExtra20Array.back();
  mExtra25 = mExtra25Array.back();
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

uint32_t CCricketClass::get_slot15() const
{
  return mSlot15;
}

uint32_t CCricketClass::get_slot16() const
{
  return mSlot16;
}

uint32_t CCricketClass::get_slot17() const
{
  return mSlot17;
}

uint32_t CCricketClass::get_slot18() const
{
  return mSlot18;
}

uint32_t CCricketClass::get_slot19() const
{
  return mSlot19;
}

uint32_t CCricketClass::get_slot20() const
{
  return mSlot20;
}

uint32_t CCricketClass::get_slot25() const
{
  return mSlot25;
}

void CCricketClass::set_slot15(uint32_t iHits)
{
  mSlot15 = iHits;
  mSlot15Array.push_back(mSlot15);
}

void CCricketClass::set_slot16(uint32_t iHits)
{
  mSlot16 = iHits;
  mSlot16Array.push_back(mSlot16);
}

void CCricketClass::set_slot17(uint32_t iHits)
{
  mSlot17 = iHits;
  mSlot17Array.push_back(mSlot17);
}

void CCricketClass::set_slot18(uint32_t iHits)
{
  mSlot18 = iHits;
  mSlot18Array.push_back(mSlot18);
}

void CCricketClass::set_slot19(uint32_t iHits)
{
  mSlot19 = iHits;
  mSlot19Array.push_back(mSlot19);
}

void CCricketClass::set_slot20(uint32_t iHits)
{
  mSlot20 = iHits;
  mSlot20Array.push_back(mSlot20);
}

void CCricketClass::set_slot25(uint32_t iHits)
{
  mSlot25 = iHits;
  mSlot25Array.push_back(mSlot25);
}

void CCricketClass::set_extra15(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra15 = iPoints;
  }
  else
  {
    mExtra15 += iPoints;
  }
  mExtra15Array.push_back(mExtra15);
}

void CCricketClass::set_extra16(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra16 = iPoints;
  }
  else
  {
    mExtra16 += iPoints;
  }
  mExtra16Array.push_back(mExtra16);
}

void CCricketClass::set_extra17(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra17 = iPoints;
  }
  else
  {
    mExtra17 += iPoints;
  }
  mExtra17Array.push_back(mExtra17);
}

void CCricketClass::set_extra18(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra18 = iPoints;
  }
  else
  {
    mExtra18 += iPoints;
  }
  mExtra18Array.push_back(mExtra18);
}

void CCricketClass::set_extra19(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra19 = iPoints;
  }
  else
  {
    mExtra19 += iPoints;
  }
  mExtra19Array.push_back(mExtra19);
}

void CCricketClass::set_extra20(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra20 = iPoints;
  }
  else
  {
    mExtra20 += iPoints;
  }
  mExtra20Array.push_back(mExtra20);
}

uint32_t CCricketClass::get_extra15() const
{
  return mExtra15;
}

uint32_t CCricketClass::get_extra16() const
{
  return mExtra16;
}

uint32_t CCricketClass::get_extra17() const
{
  return mExtra17;
}

uint32_t CCricketClass::get_extra18() const
{
  return mExtra18;
}

uint32_t CCricketClass::get_extra19() const
{
  return mExtra19;
}

uint32_t CCricketClass::get_extra20() const
{
  return mExtra20;
}

uint32_t CCricketClass::get_extra25() const
{
  return mExtra25;
}

uint32_t CCricketClass::get_total_hits() const
{
  return mTotalHits;
}

uint32_t CCricketClass::get_total_darts() const
{
  return mTotalDarts;
}

bool CCricketClass::are_slots_full() const
{
  bool full15 = mSlot15 == 3;
  bool full16 = mSlot16 == 3;
  bool full17 = mSlot17 == 3;
  bool full18 = mSlot18 == 3;
  bool full19 = mSlot19 == 3;
  bool full20 = mSlot20 == 3;
  bool full25 = mSlot25 == 3;
  return full15 && full16 && full17 && full18 && full19 && full20 && full25;
}

void CCricketClass::set_score()
{
  mScore = mExtra15 + mExtra16 + mExtra17 + mExtra18 + mExtra19 + mExtra20 + mExtra25;
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

void CCricketClass::set_extra25(uint32_t iPoints)
{
  if (!mpSettings.mCutThroat)
  {
    mExtra25 = iPoints;
  }
  else
  {
    mExtra25 += iPoints;
  }
  mExtra25Array.push_back(mExtra25);
}

void CCricketClass::undo()
{
  if (mScoreLegs.size() > 0)
  {
    perform_undo_step();
  }
  else if (mScoringHistory.size() > 0)
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
