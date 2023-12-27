#include "playerclass.h"
#include <cmath>
#include <QDebug>

CPlayerClass::CPlayerClass(int startval, int setnumber, int legnumber, int playername) :
    sets(0), legs(0), totallegs(0), remaining(startval), avg1dart(0.0), avg3dart(0.0),
    checkout(0.0), CheckoutAttempts(0), Checkouts(0), numberoflegs(legnumber),
    numberofsets(setnumber), margin_legs(std::ceil(numberoflegs/2.0)),
    margin_sets(std::ceil(numberofsets/2.0)), PlayerName(playername-1),
    StartVal(startval), TotalDarts(0), mScoresOfCurrentLeg({}), mRemainingPointsOfAllLegs({}), mAllScoresOfAllLegs({}),
    mAllScoresFlat({}), numberofdartsarray({}), checkoutattemptarray({}), checkoutarray({}), mThrownDartsOfCurrentLeg({}),
    mThrownDartsOfAllLegsFlat({}), mThrownDartsOfAllLegs({})
{
    mRemainingPointsOfCurrentLeg.push_back(remaining);
    compute_averages(0);
    compute_checkout(0, 0);
}

bool CPlayerClass::increase_setslegs() {
    legs += 1;
    totallegs += 1;
    bool temp = false;
    if (legs == margin_legs) {
        sets += 1;
        legs = 0;
        temp = true;
    }
    if (sets == margin_sets) {
        emit signal_game_won(PlayerName);
    }
    return temp;
}

int CPlayerClass::set_score(int& score) {
    mScoresOfCurrentLeg.push_back(score);
    mAllScoresFlat.push_back(score);
    remaining -= score;
    mRemainingPointsOfCurrentLeg.push_back(remaining);
    return remaining;
}

void CPlayerClass::set_darts(QVector<QString> darts)
{
    mThrownDartsOfCurrentLeg.append(darts);
    mThrownDartsOfAllLegsFlat.append(darts);
}

QVector<QVector<QString>> CPlayerClass::get_darts()
{
    return mThrownDartsOfAllLegsFlat;
}

QVector<int> CPlayerClass::get_total_scores()
{
    return mAllScoresFlat;
}

void CPlayerClass::update_history() {
    mAllScoresOfAllLegs.push_back(mScoresOfCurrentLeg);
    mThrownDartsOfAllLegs.push_back(mThrownDartsOfCurrentLeg);
    mRemainingPointsOfAllLegs.push_back(mRemainingPointsOfCurrentLeg);
}

void CPlayerClass::reset_score()
{
    remaining = StartVal;
    mRemainingPointsOfCurrentLeg = {remaining};
    mScoresOfCurrentLeg = {};
    mThrownDartsOfCurrentLeg = {};
}

void CPlayerClass::reset_legs()
{
    legs = 0;
}

int CPlayerClass::get_player_number()
{
    return PlayerName;
}

void CPlayerClass::perform_undo_step() {
    mThrownDartsOfCurrentLeg.pop_back();
    mRemainingPointsOfCurrentLeg.pop_back();
    remaining = mRemainingPointsOfCurrentLeg.back();
    mScoresOfCurrentLeg.pop_back();
    mAllScoresFlat.pop_back();
    mThrownDartsOfAllLegsFlat.pop_back();
    TotalDarts -= numberofdartsarray.back();
    numberofdartsarray.pop_back();
    CheckoutAttempts -= checkoutattemptarray.back();
    checkoutattemptarray.pop_back();
    Checkouts -= checkoutarray.back();
    checkoutarray.pop_back();

    if (CheckoutAttempts > 0) {
        checkout = (static_cast<double> (Checkouts) / static_cast<double> (CheckoutAttempts)) * 100.0;
    } else {
        checkout = 0.0;
    }
    double n = static_cast<double>(TotalDarts);
    if (TotalDarts > 0) {
        avg1dart = std::accumulate(mAllScoresFlat.begin(),mAllScoresFlat.end(), 0.0)/n;
    } else {
        avg1dart = 0.0;
    }
    avg3dart = 3 * avg1dart;
}

QVector<int> CPlayerClass::get_score_leg()
{
    return mScoresOfCurrentLeg;
}

QString CPlayerClass::get_checkout_attempts()
{
    return QString::number(Checkouts) + " / " + QString::number(CheckoutAttempts);
}

void CPlayerClass::undo() {
    if (mScoresOfCurrentLeg.size() > 0) {
        perform_undo_step();
    } else if (mAllScoresOfAllLegs.size() > 0) {
        mScoresOfCurrentLeg = mAllScoresOfAllLegs.back();
        mThrownDartsOfCurrentLeg = mThrownDartsOfAllLegs.back();
        mRemainingPointsOfCurrentLeg = mRemainingPointsOfAllLegs.back();
        mThrownDartsOfAllLegs.pop_back();
        mRemainingPointsOfAllLegs.pop_back();
        mAllScoresOfAllLegs.pop_back();
        if (mScoresOfCurrentLeg.size() > 0) {
            if (mRemainingPointsOfCurrentLeg.back() == 0) {
                if (totallegs % margin_legs == 0) {
                    totallegs -= 1;
                    legs = margin_legs -1;
                    sets -= 1;
                    perform_undo_step();
                } else {
                    totallegs -= 1;
                    legs -= 1;
                    perform_undo_step();
                }
            } else {
                perform_undo_step();
            }
            //mRemainingPointsOfCurrentLeg.pop_back();
        }
    }
}

void CPlayerClass::compute_averages(int numberofdarts) {
    TotalDarts += numberofdarts;
    numberofdartsarray.push_back(numberofdarts);
    double n = static_cast<double>(TotalDarts);
    if (TotalDarts > 0) {
        avg1dart = std::accumulate(mAllScoresFlat.begin(),mAllScoresFlat.end(), 0.0)/n;
    } else {
        avg1dart = 0.0;
    }
    avg3dart = 3 * avg1dart;
}

void CPlayerClass::compute_checkout(int checkoutattempts, int success) {
    CheckoutAttempts += checkoutattempts;
    checkoutattemptarray.push_back(checkoutattempts);
    checkoutarray.push_back(success);
    Checkouts += success;
    if (CheckoutAttempts > 0) {
        checkout = (static_cast<double> (Checkouts) / static_cast<double> (CheckoutAttempts)) * 100.0;
    } else {
        checkout = 0;
    }
}

double CPlayerClass::get_avg1dart() {
    return avg1dart;
}

double CPlayerClass::get_avg3dart() {
    return avg3dart;
}

double CPlayerClass::get_checkout() {
    return checkout;
}

QVector<int> CPlayerClass::get_leg_scores()
{
    return mScoresOfCurrentLeg;
}

QVector<QVector<QString>> CPlayerClass::get_LegDarts()
{
    return mThrownDartsOfCurrentLeg;
}

QVector<QVector<int>> CPlayerClass::get_TotalScores()
{
    return mAllScoresOfAllLegs;
}

QVector<QVector<QVector<QString>>> CPlayerClass::getThrownDartsOfAllLegs()
{
    return mThrownDartsOfAllLegs;
}

int CPlayerClass::get_remaining()
{
    return remaining;
}

QVector<int> CPlayerClass::get_leg_remaining()
{
    return mRemainingPointsOfCurrentLeg;
}

QVector<QVector<int> > CPlayerClass::get_remaining_of_all_legs()
{
    return mRemainingPointsOfAllLegs;
}

int CPlayerClass::get_legs()
{
    return legs;
}

int CPlayerClass::get_sets()
{
    return sets;
}
