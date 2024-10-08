#ifndef X01_CLASS_H
#define X01_CLASS_H

#include <QVector>
#include "settings.h"

class CX01MainWindow;

class CX01Class
{
public:

  class CPlayerData
  {

    friend class CX01Class;
    friend class CGameDataHandler;
    friend class CLegStatsModel;
    friend class CStatsWindowX01;
    friend class CGameResultsModel;
    friend class CAlltimePlayerStatsModel;
    friend class CAllTimeStatsDialog;

  public:

    CPlayerData() = default;

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
                double iFirst9Avg,
                QVector<uint32_t> iScoresOfCurrentLeg,
                QVector<QVector<uint32_t>> iAllScoresOfAllLegs,
                QVector<uint32_t> iAllScoresFlat,
                QVector<QVector<QString>> iThrownDartsOfCurrentLeg,
                QVector<QVector<QString>> iThrownDartsOfAllLegsFlat,
                QVector<QVector<QVector<QString>>> iThrownDartsOfAllLegs,
                QVector<uint32_t> iRemainingPointsOfCurrentLeg,
                QVector<QVector<uint32_t>> iRemainingPointsOfAllLegs,
                bool iActive = false
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
      , First9Avg(iFirst9Avg)
      , ScoresOfCurrentLeg(iScoresOfCurrentLeg)
      , AllScoresOfAllLegs(iAllScoresOfAllLegs)
      , AllScoresFlat(iAllScoresFlat)
      , ThrownDartsOfCurrentLeg(iThrownDartsOfCurrentLeg)
      , ThrownDartsOfAllLegsFlat(iThrownDartsOfAllLegsFlat)
      , ThrownDartsOfAllLegs(iThrownDartsOfAllLegs)
      , RemainingPointsOfCurrentLeg(iRemainingPointsOfCurrentLeg)
      , RemainingPointsOfAllLegs(iRemainingPointsOfAllLegs)
      , Active(iActive)
    {}

    double rnd2Decimals(double iNum) const
    {
      double val = std::ceil(iNum * 100.0) / 100.0;
      return val;
    }

    bool operator==(const CPlayerData & iOther) const
    {
      return SetsWon == iOther.SetsWon &&
             LegsWonPerSet == iOther.LegsWonPerSet &&
             TotalLegsWon == iOther.TotalLegsWon &&
             RemainingPoints == iOther.RemainingPoints &&
             CheckoutAttempts == iOther.CheckoutAttempts &&
             CheckoutHits == iOther.CheckoutHits &&
             TotalDarts == iOther.TotalDarts &&
             rnd2Decimals(Avg1Dart) == rnd2Decimals(iOther.Avg1Dart) &&
             rnd2Decimals(Avg3Dart) == rnd2Decimals(iOther.Avg3Dart) &&
             rnd2Decimals(CheckoutRate) == rnd2Decimals(iOther.CheckoutRate) &&
             rnd2Decimals(First9Avg) == rnd2Decimals(iOther.First9Avg) &&
             ScoresOfCurrentLeg == iOther.ScoresOfCurrentLeg &&
             AllScoresOfAllLegs == iOther.AllScoresOfAllLegs &&
             AllScoresFlat == iOther.AllScoresFlat &&
             ThrownDartsOfCurrentLeg == iOther.ThrownDartsOfCurrentLeg &&
             ThrownDartsOfAllLegsFlat == iOther.ThrownDartsOfAllLegsFlat &&
             ThrownDartsOfAllLegs == iOther.ThrownDartsOfAllLegs &&
             RemainingPointsOfCurrentLeg == iOther.RemainingPointsOfCurrentLeg &&
             RemainingPointsOfAllLegs == iOther.RemainingPointsOfAllLegs &&
             Active == iOther.Active;
    }

    bool Active = false;
    bool Finished = false;

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
    double First9Avg          = 0.0;
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

  CX01Class(uint32_t iPlayerNumber, const CSettings iSettings);
  uint32_t set_score(uint32_t iScore);
  void set_darts(QVector<QString> iDarts);
  void compute_averages(uint32_t iNumberOfDarts);
  void update_checkout(uint32_t iCheckoutAttempts, uint32_t iSuccess);
  double get_avg1dart() const;
  double get_avg3dart() const;
  double get_checkout() const;
  uint32_t get_remaining() const;
  uint32_t get_legs() const;
  uint32_t get_sets() const;
  void reset_score();
  void reset_legs();
  uint32_t get_player_number() const;
  bool increment_won_legs_and_check_if_set_won();
  void restore_state(CPlayerData iData);
  CPlayerData create_snapshot() const;
  bool has_won_game() const;

private:

  void compute_checkout();
  void compute_first9_avg();

  const CSettings mSettings;
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
  double mFirst9Avg = 0.0;
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
