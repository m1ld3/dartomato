#include "cricketclass.h"
#include <cmath>
#include <QDebug>

cricketclass::cricketclass(QWidget* parent, int numberofsets, int numberoflegs, int playername, bool cutthroat) :
    mSets(0), mLegs(0), mTotalLegs(0), CutThroat(cutthroat), mScoreLegs({}), mScoringHistory({}),
    mTotalScores({}), mNumberOfLegs(numberoflegs), mNumberOfSets(numberofsets),
    mPlayerName(playername-1), mNumberOfDartsArray({}), mTotalDarts(0), mSlot15(0),
    mSlot16(0), mSlot17(0), mSlot18(0), mSlot19(0), mSlot20(0), mSlot25(0), mExtra15(0),
    mExtra16(0), mExtra17(0), mExtra18(0), mExtra19(0), mExtra20(0), mExtra25(0), mScore(0),
    mSlot15Array({0}), mSlot16Array({0}), mSlot17Array({0}), mSlot18Array({0}), mSlot19Array({0}),
    mSlot20Array({0}), mSlot25Array({0}), mExtra15Array({0}), mExtra16Array({0}), mExtra17Array({0}),
    mExtra18Array({0}), mExtra19Array({0}), mExtra20Array({0}), mExtra25Array({0}), mScoreArray({0}), mLegWinArray({false})
{
    mGameWindow = dynamic_cast<GameWindow*>(parent);
    mMarginLegs = std::ceil(numberoflegs/2.0);
    mMarginSets = std::ceil(numberofsets/2.0);
    mHitsPerRound = compute_hits_per_round(0,0);
}

bool cricketclass::increase_setslegs() {
    mLegs += 1;
    mTotalLegs += 1;
    bool temp = false;
    if (((mLegs % mMarginLegs) == 0) && (mLegs > 0)) {
        mSets += 1;
        mLegs = 0;
        temp = true;
    }
    if (mSets == mMarginSets) {
        emit signal_game_won(mPlayerName);
    }
    return temp;
}

void cricketclass::update_history() {
    mScoringHistory.push_back(mScoreLegs);
}

void cricketclass::update_darts(QVector<QString> darts)
{
    mScoreLegs.push_back(darts);
    mTotalScores.push_back(darts);
}

void cricketclass::reset_score()
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

void cricketclass::reset_legs()
{
    mLegs = 0;
}

void cricketclass::undoStep() {
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
    setScore();
    mTotalDarts -= mNumberOfDartsArray.back();
    mNumberOfHitsArray.pop_back();
    mTotalHits = mNumberOfHitsArray.back();
    mNumberOfDartsArray.pop_back();
    mLegWinArray.pop_back();
    if (mTotalDarts > 0) {
        mHitsPerRound = 3 * static_cast<double>(mTotalHits) / static_cast<double>(mTotalDarts);
    } else {
        mHitsPerRound = 0.0;
    }
}

int cricketclass::get_player_name()
{
    return mPlayerName;
}

int cricketclass::getSlot15()
{
    return mSlot15;
}

int cricketclass::getSlot16()
{
    return mSlot16;
}

int cricketclass::getSlot17()
{
    return mSlot17;
}

int cricketclass::getSlot18()
{
    return mSlot18;
}

int cricketclass::getSlot19()
{
    return mSlot19;
}

int cricketclass::getSlot20()
{
    return mSlot20;
}

int cricketclass::getSlot25()
{
    return mSlot25;
}

void cricketclass::setSlot15(int hits)
{
    mSlot15 = hits;
    mSlot15Array.push_back(mSlot15);
}

void cricketclass::setSlot16(int hits)
{
    mSlot16 = hits;
    mSlot16Array.push_back(mSlot16);
}

void cricketclass::setSlot17(int hits)
{
    mSlot17 = hits;
    mSlot17Array.push_back(mSlot17);
}

void cricketclass::setSlot18(int hits)
{
    mSlot18 = hits;
    mSlot18Array.push_back(mSlot18);
}

void cricketclass::setSlot19(int hits)
{
    mSlot19 = hits;
    mSlot19Array.push_back(mSlot19);
}

void cricketclass::setSlot20(int hits)
{
    mSlot20 = hits;
    mSlot20Array.push_back(mSlot20);
}

void cricketclass::setSlot25(int hits)
{
    mSlot25 = hits;
    mSlot25Array.push_back(mSlot25);
}

void cricketclass::setExtra15(int points)
{
    if (!CutThroat) {
        mExtra15 = points;
    } else {
        mExtra15 += points;
    }
    mExtra15Array.push_back(mExtra15);
}

void cricketclass::setExtra16(int points)
{
    if (!CutThroat) {
        mExtra16 = points;
    } else {
        mExtra16 += points;
    }
    mExtra16Array.push_back(mExtra16);
}

void cricketclass::setExtra17(int points)
{
    if (!CutThroat) {
        mExtra17 = points;
    } else {
        mExtra17 += points;
    }
    mExtra17Array.push_back(mExtra17);
}

void cricketclass::setExtra18(int points)
{
    if (!CutThroat) {
        mExtra18 = points;
    } else {
        mExtra18 += points;
    }
    mExtra18Array.push_back(mExtra18);
}

void cricketclass::setExtra19(int points)
{
    if (!CutThroat) {
        mExtra19 = points;
    } else {
        mExtra19 += points;
    }
    mExtra19Array.push_back(mExtra19);
}

void cricketclass::setExtra20(int points)
{
    if (!CutThroat) {
        mExtra20 = points;
    } else {
        mExtra20 += points;
    }
    mExtra20Array.push_back(mExtra20);
}

int cricketclass::getExtra15()
{
    return mExtra15;
}

int cricketclass::getExtra16()
{
    return mExtra16;
}

int cricketclass::getExtra17()
{
    return mExtra17;
}

int cricketclass::getExtra18()
{
    return mExtra18;
}

int cricketclass::getExtra19()
{
    return mExtra19;
}

int cricketclass::getExtra20()
{
    return mExtra20;
}

int cricketclass::getExtra25()
{
    return mExtra25;
}

int cricketclass::getTotalHits()
{
    return mTotalHits;
}

int cricketclass::getTotalDarts()
{
    return mTotalDarts;
}

bool cricketclass::areSlotsFull()
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

void cricketclass::setScore()
{
    mScore = mExtra15 + mExtra16 + mExtra17 + mExtra18 + mExtra19 + mExtra20 + mExtra25;
    mScoreArray.push_back(mScore);
}

int cricketclass::getScore()
{
    return mScore;
}

double cricketclass::get_hits_per_round()
{
    return mHitsPerRound;
}

QVector<QVector<QString> > cricketclass::getMScoreLegs()
{
    return mScoreLegs;
}

QVector<QVector<QVector<QString> > > cricketclass::getmScoringHistory()
{
    return mScoringHistory;
}

void cricketclass::setLegWinArray(bool finished)
{
    mLegWinArray.push_back(finished);
}

void cricketclass::setTotalHits(int hits)
{
    mTotalHits = hits;
    mNumberOfHitsArray.push_back(mTotalHits);
}

void cricketclass::setTotalDarts(int darts)
{
    mTotalDarts = darts;
    mNumberOfDartsArray.push_back(mTotalDarts);
}

void cricketclass::setExtra25(int points)
{
    if (!CutThroat) {
        mExtra25 = points;
    } else {
        mExtra25 += points;
    }
    mExtra25Array.push_back(mExtra25);
}

void cricketclass::undo() {
    if (mScoreLegs.size() > 0) {
        undoStep();
    } else if (mScoringHistory.size() > 0) {
        mScoreLegs = mScoringHistory.back();
        mScoringHistory.pop_back();
        if (mScoreLegs.size() > 0) {
            bool finished = mLegWinArray.back();
            undoStep();
            if (finished) {
                if (mTotalLegs % mMarginLegs == 0) {
                    mTotalLegs -= 1;
                    mLegs = mMarginLegs -1;
                    mSets -= 1;
                } else {
                    mTotalLegs -= 1;
                    mLegs -= 1;
                }
            } else {
            }
        }
    }
}

double cricketclass::compute_hits_per_round(int numberofdarts, int totalhits) {
    mTotalDarts += numberofdarts;
    mNumberOfDartsArray.push_back(numberofdarts);
    mTotalHits = totalhits;
    mNumberOfHitsArray.push_back(mTotalHits);
    if (numberofdarts > 0) {
        mHitsPerRound = 3 * static_cast<double>(mTotalHits) / static_cast<double>(mTotalDarts);
    } else {
        mHitsPerRound = 0.0;
    }
    return mHitsPerRound;
}

int cricketclass::get_legs()
{
    return mLegs;
}

int cricketclass::get_sets()
{
    return mSets;
}
