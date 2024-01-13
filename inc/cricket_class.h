#ifndef CRICKET_CLASS_H
#define CRICKET_CLASS_H

#include <QObject>
#include "settings.h"

class CCricketMainWindow;

class CCricketClass : public QObject
{
  Q_OBJECT

public:

  CCricketClass(QWidget * iParent, uint32_t iPlayerNumber, const CSettings & ipSettings);
  void undo();
  double compute_hits_per_round(uint32_t iNumberofdarts, uint32_t iTotalhits);
  uint32_t get_legs() const;
  uint32_t get_sets() const;
  void update_history();
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
  void set_leg_win_array(bool iFinished);
  void set_total_hits(uint32_t iHits);
  void set_total_darts(uint32_t iDarts);
  uint32_t get_total_darts() const;
  bool increase_setslegs();

private:

  void handle_undo_after_legset_win();
  void notify_game_won(uint32_t iPlayerNumber);
  void perform_undo_step();

  CCricketMainWindow * mpGameWindow;
  uint32_t mSetsWon = 0;  // sets won
  uint32_t mLegsWonPerSet = 0;  // legs won per set
  uint32_t mTotalLegsWon = 0;  // total legs won
  QVector<QVector<QString>> mScoreLegs = {};  // all rounds of current leg
  QVector<QVector<QVector<QString>>> mScoringHistory = {};  //  complete scoring history of the current game
  QVector<QVector<QString>> mTotalScores = {};  // all rounds in one vector
  uint32_t mMarginLegs, mMarginSets;  // required legs/sets to win for one set
  uint32_t mPlayerNumber;  // instance of player
  QVector<uint32_t> mNumberOfDartsArray = {};
  uint32_t mTotalDarts = 0;  // total amount of thrown darts
  uint32_t mTotalHits;  // total amount of hits
  QVector<uint32_t> mNumberOfHitsArray;
  double mHitsPerRound;
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  uint32_t mScore = 0;
  QVector<uint32_t> mScoreArray = {0};
  QVector<bool> mLegWinArray = {false};
  const CSettings & mpSettings;
};

#endif  // CRICKET_CLASS_H
