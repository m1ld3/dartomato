#ifndef X01_CLASS_H
#define X01_CLASS_H

#include <QVector>
#include <QObject>
#include "settings.h"

class CX01MainWindow;

class CX01Class : public QObject
{
  Q_OBJECT

  class CPlayerData
  {

    friend class CX01Class;

  public:

    CPlayerData() = delete;
    CPlayerData(uint32_t iSetsWon,
                uint32_t iLegsWonPerSet,
                uint32_t iTotalLegsWon,
                uint32_t iRemainingPoints,
                uint32_t iCheckoutAttempts,
                uint32_t iCheckoutHits,
                uint32_t iTotalDarts,
                double iAvg1Dart,
                double iAvg3Dart,
                double iCheckoutRate,
                QVector<uint32_t> iScoresOfCurrentLeg,
                QVector<QVector<uint32_t>> iAllScoresOfAllLegs,
                QVector<uint32_t> iAllScoresFlat,
                QVector<QVector<QString>> iThrownDartsOfCurrentLeg
                )
      : SetsWon(iSetsWon)
      , LegsWonPerSet(iLegsWonPerSet)
      , TotalLegsWon(iTotalLegsWon)
      , RemainingPoints(iRemainingPoints)
      , CheckoutAttempts(iCheckoutAttempts)
      , CheckoutHits(iCheckoutHits)
      , TotalDarts(iTotalDarts)
      , Avg1Dart(iAvg1Dart)
      , Avg3Dart(iAvg3Dart)
      , CheckoutRate(iCheckoutRate)
      , ScoresOfCurrentLeg(iScoresOfCurrentLeg)
      , AllScoresOfAllLegs(iAllScoresOfAllLegs)
      , AllScoresFlat(iAllScoresFlat)
      , ThrownDartsOfCurrentLeg(iThrownDartsOfCurrentLeg)
    {}

  private:

    uint32_t SetsWon          = 0;
    uint32_t LegsWonPerSet    = 0;
    uint32_t TotalLegsWon     = 0;
    uint32_t RemainingPoints  = 0;
    uint32_t CheckoutAttempts = 0;
    uint32_t CheckoutHits     = 0;
    uint32_t TotalDarts       = 0;
    double Avg1Dart           = 0.0;
    double Avg3Dart           = 0.0;
    double CheckoutRate       = 0.0;
    QVector<uint32_t> ScoresOfCurrentLeg               = {};
    QVector<QVector<uint32_t>> AllScoresOfAllLegs     = {};
    QVector<uint32_t> AllScoresFlat                    = {};
    QVector<QVector<QString>> ThrownDartsOfCurrentLeg = {};
  };

public:

  CX01Class(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings);
  uint32_t set_score(uint32_t iScore);
  void set_darts(QVector<QString> iDarts);
  QVector<QVector<QString>> get_darts() const;
  QVector<uint32_t> get_total_scores_flat() const;
  void undo();
  void compute_averages(uint32_t iNumberOfDarts);
  void update_checkout(uint32_t iCheckoutAttempts, uint32_t iSuccess);
  double get_avg1dart() const;
  double get_avg3dart() const;
  double get_checkout() const;
  QVector<uint32_t> get_scores_of_current_leg() const;
  QVector<QVector<QString>> get_thrown_darts_of_current_leg() const;
  QVector<QVector<uint32_t>> get_all_scores_of_all_legs() const;
  QVector<QVector<QVector<QString>>> get_thrown_darts_of_all_legs() const;
  uint32_t get_remaining() const;
  QVector<uint32_t> get_remaining_points_of_current_leg() const;
  QVector<QVector<uint32_t>> get_remaining_points_of_all_legs() const;
  uint32_t get_legs() const;
  uint32_t get_sets() const;
  void update_history();
  void reset_score();
  void reset_legs();
  uint32_t get_player_number() const;
  QString get_checkout_attempts_str() const;
  uint32_t get_checkout_attempts() const;
  bool increment_won_legs_and_check_if_set_won();
  void restore_state(CPlayerData iData);
  CPlayerData create_snapshot() const;

private:

  void undo_last_won_leg_or_set();
  void perform_undo_step();
  void notify_game_won(uint32_t iPlayerNumber);
  void compute_checkout();

  CX01MainWindow * mGameWindow;
  const CSettings & mSettings;
  uint32_t mSetsWon = 0;  // sets won
  uint32_t mLegsWonPerSet = 0;  // legs won per set
  uint32_t mTotalLegsWon = 0;  // total legs won
  uint32_t mRemainingPoints;  // current remaining points
  double mAvg1Dart = 0.0;  // 1-dart average
  double mAvg3Dart = 0.0;  // 3-dart average
  double mCheckoutRate = 0.0;  // checkout percentage
  uint32_t mCheckoutAttempts = 0;  // checkout attempts
  uint32_t mCheckoutHits = 0;  // successful checkouts
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

#endif  // X01_CLASS_H
