#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include <QVector>
#include <QObject>
#include "settings.h"

class CX01Class : public QObject
{
  Q_OBJECT

public:

  CX01Class(uint32_t iPlayerNumber, const CSettings & ipSettings);
  uint32_t set_score(uint32_t iScore);
  void set_darts(QVector<QString> iDarts);
  QVector<QVector<QString>> get_darts() const;
  QVector<uint32_t> get_total_scores_flat() const;
  void undo();
  void compute_averages(uint32_t iNumberOfDarts);
  void compute_checkout(uint32_t iCheckoutAttempts, uint32_t iSuccess);
  double get_avg1dart() const;
  double get_avg3dart() const;
  double get_checkout() const;
  QVector<uint32_t> get_leg_scores() const;
  QVector<QVector<QString>> get_leg_darts() const;
  QVector<QVector<uint32_t>> get_total_scores() const;
  QVector<QVector<QVector<QString>>> get_thrown_darts_of_all_legs() const;
  uint32_t get_remaining() const;
  QVector<uint32_t> get_leg_remaining() const;
  QVector<QVector<uint32_t>> get_remaining_of_all_legs() const;
  uint32_t get_legs() const;
  uint32_t get_sets() const;
  void update_history();
  void reset_score();
  void reset_legs();
  uint32_t get_player_number() const;
  void perform_undo_step();
  QVector<uint32_t> get_score_leg() const;
  QString get_checkout_attempts() const;
  bool increase_setslegs();

signals:

  void signal_game_won(uint32_t iPlayerNumber);

private:

  const CSettings & mpSettings;
  uint32_t mSetsWon = 0;  // sets won
  uint32_t mLegsWonPerSet = 0;  // legs won per set
  uint32_t mTotalLegsWon = 0;  // total legs won
  uint32_t mRemaining;  // current remaining points
  double mAvg1Dart = 0.0;  // 1-dart average
  double mAvg3Dart = 0.0;  // 3-dart average
  double mCheckout = 0.0;  // checkout percentage
  uint32_t mCheckoutAttempts = 0;  // checkout attempts
  uint32_t mCheckouts = 0;  // successful checkouts
  uint32_t mMarginLegs, mMarginSets;  // required legs/sets to win per set/game
  uint32_t mPlayerNumber;  // instance of player
  uint32_t mTotalDarts = 0;  // total amount of thrown darts
  QVector<uint32_t> mScoresOfCurrentLeg = {};  // all scores of current leg
  QVector<uint32_t> mRemainingPointsOfCurrentLeg;  // all intermediate remaining points of current leg
  QVector<QVector<uint32_t>> mRemainingPointsOfAllLegs = {};
  QVector<QVector<uint32_t>> mAllScoresOfAllLegs = {};  // complete scoring history of the current game
  QVector<uint32_t> mAllScoresFlat = {};  // all scores in one vector
  QVector<uint32_t> mNumberOfDartsArray = {};
  QVector<uint32_t> mCheckoutAttemptsArray = {};
  QVector<uint32_t> mCheckoutsArray = {};
  QVector<QVector<QString>> mThrownDartsOfCurrentLeg = {};
  QVector<QVector<QString>> mThrownDartsOfAllLegsFlat = {};
  QVector<QVector<QVector<QString>>> mThrownDartsOfAllLegs = {};
};

#endif  // PLAYERCLASS_H
