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

  public:

    CPlayerData() = delete;
    CPlayerData(uint32_t iSetsWon,
                uint32_t iLegsWonPerSet,
                uint32_t iTotalLegsWon,
                uint32_t iTotalDarts,
                uint32_t iScore,
                uint32_t iTotalHits,
                double iHitsPerRound,
                QVector<QVector<QString>> iScoreOfCurrentLegs,
                QVector<QVector<QVector<QString>>> iScoringHistory,
                std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> iSlotArray,
                std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> iExtraPointsArray
                )
      : SetsWon(iSetsWon)
      , LegsWonPerSet(iLegsWonPerSet)
      , TotalLegsWon(iTotalLegsWon)
      , TotalDarts(iTotalDarts)
      , Score(iScore)
      , TotalHits(iTotalHits)
      , HitsPerRound(iHitsPerRound)
      , ScoreOfCurrentLegs(iScoreOfCurrentLegs)
      , ScoringHistory(iScoringHistory)
      , SlotArray(iSlotArray)
      , ExtraPointsArray(iExtraPointsArray)
    {}

  private:

    uint32_t SetsWon = 0;
    uint32_t LegsWonPerSet = 0;
    uint32_t TotalLegsWon = 0;
    uint32_t TotalDarts = 0;
    uint32_t Score = 0;
    uint32_t TotalHits = 0;
    double HitsPerRound = 0.0;
    QVector<QVector<QString>> ScoreOfCurrentLegs = {};
    QVector<QVector<QVector<QString>>> ScoringHistory = {};
    std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> SlotArray = {};
    std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> ExtraPointsArray = {};
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
  void set_total_hits(uint32_t iHits);
  bool increase_setslegs();
  void restore_state(CPlayerData iData);
  CPlayerData create_snapshot() const;

private:

  void notify_game_won();

  CCricketMainWindow * mGameWindow;
  uint32_t mSetsWon = 0;
  uint32_t mLegsWonPerSet = 0;
  uint32_t mTotalLegsWon = 0;
  uint32_t mTotalDarts = 0;
  uint32_t mScore = 0;
  uint32_t mMarginLegs, mMarginSets, mPlayerNumber, mTotalHits;
  double mHitsPerRound;
  QVector<QVector<QString>> mScoresOfCurrentLeg = {};
  QVector<QVector<QVector<QString>>> mScoringHistory = {};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  const CSettings & mSettings;
};

#endif  // CRICKET_CLASS_H
