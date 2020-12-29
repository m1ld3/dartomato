#include "playerclass.h"
#include <cmath>
#include <QDebug>

PlayerClass::PlayerClass(int startval, int setnumber, int legnumber, int playername) :
    sets(0), legs(0), totallegs(0), remaining(startval), avg1dart(0.0), avg3dart(0.0),
    checkout(0.0), CheckoutAttempts(0), Checkouts(0), numberoflegs(legnumber),
    numberofsets(setnumber), margin_legs(std::ceil(numberoflegs/2.0)),
    margin_sets(std::ceil(numberofsets/2.0)), PlayerName(playername-1),
    StartVal(startval), TotalDarts(0), score_leg({}), score_total({}),
    totalscores({}), numberofdartsarray({}), checkoutattemptarray({}), checkoutarray({}), mThrownDarts({})
{
    points_leg.push_back(remaining);
    compute_averages(0);
    compute_checkout(0, 0);
}

bool PlayerClass::increase_setslegs() {
    legs += 1;
    totallegs += 1;
    bool temp = false;
    if (legs == margin_legs) {
        sets += 1;
        legs = 0;
        temp = true;
    }
    if (sets == margin_sets) {
        emit signalGameWon(PlayerName);
    }
    return temp;
}

int PlayerClass::set_score(int& score) {
    score_leg.push_back(score);
    totalscores.push_back(score);
    remaining -= score;
    points_leg.push_back(remaining);
    return remaining;
}

void PlayerClass::set_darts(QVector<QString> darts)
{
    mThrownDarts.append(darts);
}

QVector<QVector<QString>> PlayerClass::get_darts()
{
    return mThrownDarts;
}

QVector<int> PlayerClass::get_total_scores()
{
    return totalscores;
}

void PlayerClass::update_history() {
    score_total.push_back(score_leg);
}

void PlayerClass::resetScore()
{
    remaining = StartVal;
    points_leg.push_back(remaining);
    score_leg = {};
}

void PlayerClass::resetLegs()
{
    legs = 0;
}

int PlayerClass::getPlayerName()
{
    return PlayerName;
}

void PlayerClass::undoStep() {
    mThrownDarts.pop_back();
    points_leg.pop_back();
    remaining = points_leg.back();
    score_leg.pop_back();
    totalscores.pop_back();
    TotalDarts -= numberofdartsarray.back();
    numberofdartsarray.pop_back();
    CheckoutAttempts -= checkoutattemptarray.back();
    checkoutattemptarray.pop_back();
    Checkouts -= checkoutarray.back();
    checkoutarray.pop_back();
//    Score180 -= array180.back();
//    array180.pop_back();
//    Plus160 -= array160.back();
//    array160.pop_back();
//    Plus140 -= array140.back();
//    array140.pop_back();
//    Plus120 -= array120.back();
//    array120.pop_back();
//    Plus100 -= array100.back();
//    array100.pop_back();
//    Score26 -= array26.back();
//    array26.pop_back();
//    Score11 -= array11.back();
//    array11.pop_back();
//    Score7 -= array7.back();
//    array7.pop_back();
//    Score3 -= array3.back();
//    array3.pop_back();
//    T20 -= arrayt20.back();
//    arrayt20.pop_back();
//    T19 -= arrayt19.back();
//    arrayt19.pop_back();
//    T18 -= arrayt18.back();
//    arrayt18.pop_back();
//    T17 -= arrayt17.back();
//    arrayt17.pop_back();
//    T16 -= arrayt16.back();
//    arrayt16.pop_back();
//    T15 -= arrayt15.back();
//    arrayt15.pop_back();
//    Bull -= arraybull.back();
//    arraybull.pop_back();
//    Ones -= arrayones.back();
//    arrayones.pop_back();
//    Miss -= arraymiss.back();
//    arraymiss.pop_back();
    if (CheckoutAttempts > 0) {
        checkout = (static_cast<double> (Checkouts) / static_cast<double> (CheckoutAttempts)) * 100.0;
    } else {
        checkout = 0.0;
    }
    double n = static_cast<double>(TotalDarts);
    if (TotalDarts > 0) {
        avg1dart = std::accumulate(totalscores.begin(),totalscores.end(), 0.0)/n;
    } else {
        avg1dart = 0.0;
    }
    avg3dart = 3 * avg1dart;
}

QVector<QVector<int> > PlayerClass::getScoreTotal()
{
    return score_total;
}

QVector<int> PlayerClass::getScoreLeg()
{
    return score_leg;
}

void PlayerClass::undo() {
    if (score_leg.size() > 0) {
        undoStep();
    } else if (score_total.size() > 0) {
        score_leg = score_total.back();
        score_total.pop_back();
        if (score_leg.size() > 0) {
            if (points_leg.back() == 0) {
                if (totallegs % margin_legs == 0) {
                    totallegs -= 1;
                    legs = margin_legs -1;
                    sets -= 1;
                    undoStep();
                } else {
                    totallegs -= 1;
                    legs -= 1;
                    undoStep();
                }
            } else {
                undoStep();
            }
        }
    }
}

void PlayerClass::compute_averages(int numberofdarts) {
    TotalDarts += numberofdarts;
    numberofdartsarray.push_back(numberofdarts);
    double n = static_cast<double>(TotalDarts);
    if (TotalDarts > 0) {
        avg1dart = std::accumulate(totalscores.begin(),totalscores.end(), 0.0)/n;
    } else {
        avg1dart = 0.0;
    }
    avg3dart = 3 * avg1dart;
}

void PlayerClass::compute_checkout(int checkoutattempts, int success) {
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

double PlayerClass::get_avg1dart() {
    return avg1dart;
}

double PlayerClass::get_avg3dart() {
    return avg3dart;
}

double PlayerClass::get_checkout() {
    return checkout;
}

QVector<int> PlayerClass::get_LegScores()
{
    return score_leg;
}

QVector<QVector<int>> PlayerClass::get_TotalScores()
{
    return score_total;
}

int PlayerClass::get_remaining()
{
    return remaining;
}

int PlayerClass::get_legs()
{
    return legs;
}

int PlayerClass::get_sets()
{
    return sets;
}
