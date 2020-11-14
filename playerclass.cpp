#include "playerclass.h"
#include <cmath>
#include <QDebug>

PlayerClass::PlayerClass(int startval, int setnumber, int legnumber, int playername) :
    Plus100(0), Plus120(0), Plus140(0), Plus160(0), Score180(0), Score3(0), Score7(0), Score11(0),
    Score26(0), T20(0), T19(0), T18(0), T17(0), T16(0), T15(0), Bull(0), Ones(0), Miss(0)
{
    sets = 0;
    legs = 0;
    totallegs = 0;
    remaining = startval;
    StartVal = startval;
    numberofsets = setnumber;
    numberoflegs = legnumber;
    margin_legs = std::ceil(numberoflegs/2.0);
    margin_sets = std::ceil(numberofsets/2.0);
    score_leg = {};
    score_total = {};
    totalscores = {};
    array180 = {};
    array160 = {};
    array140 = {};
    array120 = {};
    array100 = {};
    array26 = {};
    array11 = {};
    array7 = {};
    array3 = {};
    arrayt20 = {};
    arrayt19 = {};
    arrayt18 = {};
    arrayt17 = {};
    arrayt16 = {};
    arrayt15 = {};
    arraybull = {};
    arrayones = {};
    arraymiss = {};
    numberofdartsarray = {};
    checkoutarray = {};
    checkoutattemptarray = {};
    points_leg.push_back(remaining);
    TotalDarts = 0;
    compute_averages(0);
    CheckoutAttempts = 0;
    Checkouts = 0;
    compute_checkout(0,0);
    PlayerName = playername-1;
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
    score_leg = {};
}

void PlayerClass::updateStats(int score180, int plus160, int plus140, int plus120, int plus100, int score26, int score11, int score7, int score3, int t20, int t19, int t18, int t17, int t16, int t15, int bull, int ones, int miss)
{
    Score180 += score180;
    array180.push_back(score180);
    Plus160 += plus160;
    array160.push_back(plus160);
    Plus140 += plus140;
    array140.push_back(plus140);
    Plus120 += plus120;
    array120.push_back(plus120);
    Plus100 += plus100;
    array100.push_back(plus100);
    Score26 += score26;
    array26.push_back(score26);
    Score11 += score11;
    array11.push_back(score11);
    Score7 += score7;
    array7.push_back(score7);
    Score3 += score3;
    array3.push_back(score3);
    T20 += t20;
    arrayt20.push_back(t20);
    arrayt19.push_back(t19);
    arrayt18.push_back(t18);
    arrayt17.push_back(t17);
    arrayt16.push_back(t16);
    arrayt15.push_back(t15);
    arraybull.push_back(bull);
    arrayones.push_back(ones);
    arraymiss.push_back(miss);
    T19 += t19;
    T18 += t18;
    T17 += t17;
    T16 += t16;
    T15 += t15;
    Bull += bull;
    Ones += ones;
    Miss += miss;
}

int PlayerClass::get_180()
{
    return Score180;
}

int PlayerClass::get_160()
{
    return Plus160;
}

int PlayerClass::get_140()
{
    return Plus140;
}

int PlayerClass::get_120()
{
    return Plus120;
}

int PlayerClass::get_100()
{
    return Plus100;
}

int PlayerClass::get_26()
{
    return Score26;
}

int PlayerClass::get_11()
{
    return Score11;
}

int PlayerClass::get_7()
{
    return Score7;
}

int PlayerClass::get_3()
{
    return Score3;
}

int PlayerClass::get_t20()
{
    return T20;
}

int PlayerClass::get_t19()
{
    return T19;
}

int PlayerClass::get_t18()
{
    return T18;
}

int PlayerClass::get_t17()
{
    return T17;
}

int PlayerClass::get_t16()
{
    return T16;
}

int PlayerClass::get_t15()
{
    return T15;
}

int PlayerClass::get_bull()
{
    return Bull;
}

int PlayerClass::get_ones()
{
    return Ones;
}

int PlayerClass::get_miss()
{
    return Miss;
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
    Score180 -= array180.back();
    array180.pop_back();
    Plus160 -= array160.back();
    array160.pop_back();
    Plus140 -= array140.back();
    array140.pop_back();
    Plus120 -= array120.back();
    array120.pop_back();
    Plus100 -= array100.back();
    array100.pop_back();
    Score26 -= array26.back();
    array26.pop_back();
    Score11 -= array11.back();
    array11.pop_back();
    Score7 -= array7.back();
    array7.pop_back();
    Score3 -= array3.back();
    array3.pop_back();
    T20 -= arrayt20.back();
    arrayt20.pop_back();
    T19 -= arrayt19.back();
    arrayt19.pop_back();
    T18 -= arrayt18.back();
    arrayt18.pop_back();
    T17 -= arrayt17.back();
    arrayt17.pop_back();
    T16 -= arrayt16.back();
    arrayt16.pop_back();
    T15 -= arrayt15.back();
    arrayt15.pop_back();
    Bull -= arraybull.back();
    arraybull.pop_back();
    Ones -= arrayones.back();
    arrayones.pop_back();
    Miss -= arraymiss.back();
    arraymiss.pop_back();
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
