#ifndef CRICKET_CLASS_H
#define CRICKET_CLASS_H

#include <QObject>
#include "settings.h"

class CCricketMainWindow;

class CCricketClass : public QObject
{
  Q_OBJECT

public:

  class CPlayerData
  {

    friend class CCricketClass;
    friend class CGameDataHandler;
    friend class CGameResultsModel;
    friend class CStatsWindowCricket;
    friend class CAlltimePlayerStatsModel;

  public:

    CPlayerData() = default;

    CPlayerData(uint32_t iSetsWon,
                uint32_t iLegsWonPerSet,
                uint32_t iTotalLegsWon,
                uint32_t iTotalDarts,
                uint32_t iScore,
                uint32_t iTotalHits,
                double iHitsPerRound,
                QVector<QVector<QString>> iScoresOfCurrentLeg,
                QVector<QVector<QVector<QString>>> iScoringHistory,
                QVector<uint32_t> iHitsOfCurrentLeg,
                QVector<QVector<uint32_t>> iHitsHistory,
                QVector<uint32_t> iSlotArray,
                QVector<uint32_t> iExtraPointsArray,
                QVector<bool> iLegWonVec,
                QVector<QVector<bool>> iLegWonHistory,
                bool iActive = false
                )
      : SetsWon(iSetsWon)
      , LegsWonPerSet(iLegsWonPerSet)
      , TotalLegsWon(iTotalLegsWon)
      , TotalDarts(iTotalDarts)
      , Score(iScore)
      , TotalHits(iTotalHits)
      , HitsPerRound(iHitsPerRound)
      , ScoresOfCurrentLeg(iScoresOfCurrentLeg)
      , ScoringHistory(iScoringHistory)
      , HitsOfCurrentLeg(iHitsOfCurrentLeg)
      , HitsHistory(iHitsHistory)
      , SlotArray(iSlotArray)
      , ExtraPointsArray(iExtraPointsArray)
      , LegWonVec(iLegWonVec)
      , LegWonHistory(iLegWonHistory)
      , Active(iActive)
    {}

    bool Active = false;
    bool Finished = false;
    bool GameWon = false;

  private:

    uint32_t SetsWon       = 0;
    uint32_t LegsWonPerSet = 0;
    uint32_t TotalLegsWon  = 0;
    uint32_t TotalDarts    = 0;
    uint32_t Score         = 0;
    uint32_t TotalHits     = 0;
    double HitsPerRound    = 0.0;
    QVector<QVector<QString>> ScoresOfCurrentLeg      = {};
    QVector<QVector<QVector<QString>>> ScoringHistory = {};
    QVector<uint32_t> HitsOfCurrentLeg                = {};
    QVector<QVector<uint32_t>> HitsHistory            = {};
    QVector<uint32_t> SlotArray        = {0, 0, 0, 0, 0, 0, 0};
    QVector<uint32_t> ExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
    QVector<bool> LegWonVec = {};
    QVector<QVector<bool>> LegWonHistory = {};
  };

  CCricketClass(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & iSettings);
  double compute_hits_per_round(uint32_t iNumberofdarts, uint32_t iTotalhits);
  uint32_t get_legs() const;
  uint32_t get_sets() const;
  void update_darts(QVector<QString> iDarts);
  void reset_score();
  void reset_legs();
  uint32_t get_player_number() const;
  uint32_t get_slot(const ECricketSlots iSlot) const;
  void set_slot(const ECricketSlots iSlot, uint32_t iHits);
  void set_extra_points(const ECricketSlots iSlot, uint32_t iPoints);
  uint32_t get_extra_points(const ECricketSlots iSlot) const;
  uint32_t get_total_hits() const;
  void set_score();
  uint32_t get_score() const;
  double get_hits_per_round() const;
  QVector<QVector<QString>> get_score_legs() const;
  QVector<QVector<QVector<QString>>> get_scoring_history() const;
  bool increment_won_legs_and_check_if_set_won();
  void restore_state(CPlayerData iData);
  CPlayerData create_snapshot() const;
  void set_leg_won(bool iHasWon) { mLegWonVec.append(iHasWon); }
  bool has_won_game() const;

private:

  uint32_t mSetsWon = 0;
  uint32_t mLegsWonPerSet = 0;
  uint32_t mTotalLegsWon = 0;
  uint32_t mTotalDarts = 0;
  uint32_t mScore = 0;
  uint32_t mTotalHits = 0;
  uint32_t mMarginLegs, mMarginSets, mPlayerNumber;
  double mHitsPerRound;
  QVector<QVector<QString>> mScoresOfCurrentLeg = {};
  QVector<QVector<QVector<QString>>> mScoringHistory = {};
  QVector<uint32_t> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  QVector<uint32_t> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  QVector<uint32_t> mHitsOfCurrentLeg = {};
  QVector<QVector<uint32_t>> mHitsHistory = {};
  QVector<bool> mLegWonVec = {};
  QVector<QVector<bool>> mLegWonHistory = {};
  const CSettings & mSettings;
};

#endif  // CRICKET_CLASS_H
