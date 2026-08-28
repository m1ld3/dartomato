#ifndef CRICKET_CLASS_H
#define CRICKET_CLASS_H

#include <utility>

#include "settings.h"

class CCricketMainWindow;

class CCricketClass
{
public:

  class CPlayerData
  {

    friend class CCricketClass;
    friend class CGameDataHandler;
    friend class CGameResultsModel;
    friend class CStatsWindowCricket;
    friend class CAlltimePlayerStatsModel;
    friend class CAllTimeStatsDialog;

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
      , ScoresOfCurrentLeg(std::move(iScoresOfCurrentLeg))
      , ScoringHistory(std::move(iScoringHistory))
      , HitsOfCurrentLeg(std::move(iHitsOfCurrentLeg))
      , HitsHistory(std::move(iHitsHistory))
      , SlotArray(std::move(iSlotArray))
      , ExtraPointsArray(std::move(iExtraPointsArray))
      , LegWonVec(std::move(iLegWonVec))
      , LegWonHistory(std::move(iLegWonHistory))
      , Active(iActive)
    {}

    static double rnd2Decimals(const double iNum)
    {
      return std::ceil(iNum * 100.0) / 100.0;
    }

    bool operator==(const CPlayerData & iOther) const
    {
      return SetsWon == iOther.SetsWon &&
             LegsWonPerSet == iOther.LegsWonPerSet &&
             TotalLegsWon == iOther.TotalLegsWon &&
             TotalDarts == iOther.TotalDarts &&
             Score == iOther.Score &&
             TotalHits == iOther.TotalHits &&
             rnd2Decimals(HitsPerRound) == rnd2Decimals(iOther.HitsPerRound) &&
             ScoresOfCurrentLeg == iOther.ScoresOfCurrentLeg &&
             ScoringHistory == iOther.ScoringHistory &&
             HitsOfCurrentLeg == iOther.HitsOfCurrentLeg &&
             HitsHistory == iOther.HitsHistory &&
             SlotArray == iOther.SlotArray &&
             ExtraPointsArray == iOther.ExtraPointsArray &&
             LegWonVec == iOther.LegWonVec &&
             LegWonHistory == iOther.LegWonHistory &&
             Active == iOther.Active;
    }

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

  CCricketClass(uint32_t iPlayerNumber, const CSettings & iSettings);
  double compute_hits_per_round(uint32_t iNumberOfDarts, uint32_t iTotalHits);
  [[nodiscard]] uint32_t get_legs() const;
  [[nodiscard]] uint32_t get_sets() const;
  void update_darts(const QVector<QString>& iDarts);
  void reset_score();
  void reset_legs();
  [[nodiscard]] uint32_t get_player_number() const;
  [[nodiscard]] uint32_t get_slot(ECricketSlots iSlot) const;
  void set_slot(ECricketSlots iSlot, uint32_t iHits);
  void set_extra_points(ECricketSlots iSlot, uint32_t iPoints);
  [[nodiscard]] uint32_t get_extra_points(ECricketSlots iSlot) const;
  [[nodiscard]] uint32_t get_total_hits() const;
  void compute_score();
  [[nodiscard]] uint32_t get_score() const;
  [[nodiscard]] double get_hits_per_round() const;
  [[nodiscard]] QVector<QVector<QString>> get_score_legs() const;
  [[nodiscard]] QVector<QVector<QVector<QString>>> get_scoring_history() const;
  bool increment_won_legs_and_check_if_set_won();
  void restore_state(const CPlayerData& iData);
  [[nodiscard]] CPlayerData create_snapshot() const;
  void set_leg_won(const bool iHasWon) { mLegWonVec.append(iHasWon); }
  [[nodiscard]] bool has_won_game() const;

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
