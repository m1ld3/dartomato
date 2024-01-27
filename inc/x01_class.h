#ifndef X01_CLASS_H
#define X01_CLASS_H

#include <QVector>
#include <QObject>
#include "settings.h"

class CX01MainWindow;

class CX01Class : public QObject
{
  Q_OBJECT

public:

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
                QVector<QVector<QString>> iThrownDartsOfCurrentLeg,
                QVector<QVector<QString>> iThrownDartsOfAllLegsFlat,
                QVector<QVector<QVector<QString>>> iThrownDartsOfAllLegs,
                QVector<uint32_t> iRemainingPointsOfCurrentLeg,
                QVector<QVector<uint32_t>> iRemainingPointsOfAllLegs
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
      , ThrownDartsOfAllLegsFlat(iThrownDartsOfAllLegsFlat)
      , ThrownDartsOfAllLegs(iThrownDartsOfAllLegs)
      , RemainingPointsOfCurrentLeg(iRemainingPointsOfCurrentLeg)
      , RemainingPointsOfAllLegs(iRemainingPointsOfAllLegs)
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
    QVector<uint32_t> ScoresOfCurrentLeg                    = {};
    QVector<QVector<uint32_t>> AllScoresOfAllLegs           = {};
    QVector<uint32_t> AllScoresFlat                         = {};
    QVector<QVector<QString>> ThrownDartsOfCurrentLeg       = {};
    QVector<QVector<QString>> ThrownDartsOfAllLegsFlat      = {};
    QVector<QVector<QVector<QString>>> ThrownDartsOfAllLegs = {};
    QVector<uint32_t> RemainingPointsOfCurrentLeg           = {};
    QVector<QVector<uint32_t>> RemainingPointsOfAllLegs     = {};
  };

public:

  CX01Class(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings);
  uint32_t set_score(uint32_t iScore);
  void set_darts(QVector<QString> iDarts);
  QVector<QVector<QString>> get_darts() const;
  QVector<uint32_t> get_total_scores_flat() const;
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
  void reset_score();
  void reset_legs();
  uint32_t get_player_number() const;
  QString get_checkout_attempts_str() const;
  uint32_t get_checkout_attempts() const;
  bool increment_won_legs_and_check_if_set_won();
  void restore_state(CPlayerData iData);
  CPlayerData create_snapshot() const;

private:

  void notify_game_won();
  void compute_checkout();

  CX01MainWindow * mGameWindow;
  const CSettings & mSettings;
  uint32_t mSetsWon = 0;
  uint32_t mLegsWonPerSet = 0;
  uint32_t mTotalLegsWon = 0;
  uint32_t mCheckoutAttempts = 0;
  uint32_t mCheckoutHits = 0;
  uint32_t mTotalDarts = 0;
  uint32_t mMarginLegs, mMarginSets, mRemainingPoints, mPlayerNumber;
  double mAvg1Dart = 0.0;
  double mAvg3Dart = 0.0;
  double mCheckoutRate = 0.0;
  QVector<uint32_t> mScoresOfCurrentLeg = {};
  QVector<QVector<uint32_t>> mAllScoresOfAllLegs = {};
  QVector<uint32_t> mAllScoresFlat = {};
  QVector<QVector<QString>> mThrownDartsOfCurrentLeg = {};
  QVector<QVector<QString>> mThrownDartsOfAllLegsFlat = {};
  QVector<QVector<QVector<QString>>> mThrownDartsOfAllLegs = {};
  QVector<uint32_t> mRemainingPointsOfCurrentLeg;
  QVector<QVector<uint32_t>> mRemainingPointsOfAllLegs = {};
};

#endif  // X01_CLASS_H
