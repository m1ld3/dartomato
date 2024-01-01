#ifndef CRICKETCLASS_H
#define CRICKETCLASS_H

#include <QObject>
#include "cricketmainwindow.h"
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
  void perform_undo_step();
  uint32_t get_player_number() const;
  uint32_t get_slot15() const;
  uint32_t get_slot16() const;
  uint32_t get_slot17() const;
  uint32_t get_slot18() const;
  uint32_t get_slot19() const;
  uint32_t get_slot20() const;
  uint32_t get_slot25() const;
  void set_slot15(uint32_t iHits);
  void set_slot16(uint32_t iHits);
  void set_slot17(uint32_t iHits);
  void set_slot18(uint32_t iHits);
  void set_slot19(uint32_t iHits);
  void set_slot20(uint32_t iHits);
  void set_slot25(uint32_t iHits);
  void set_extra15(uint32_t iPoints);
  void set_extra16(uint32_t iPoints);
  void set_extra17(uint32_t iPoints);
  void set_extra18(uint32_t iPoints);
  void set_extra19(uint32_t iPoints);
  void set_extra20(uint32_t iPoints);
  void set_extra25(uint32_t iPoints);
  uint32_t get_extra15() const;
  uint32_t get_extra16() const;
  uint32_t get_extra17() const;
  uint32_t get_extra18() const;
  uint32_t get_extra19() const;
  uint32_t get_extra20() const;
  uint32_t get_extra25() const;
  uint32_t get_total_hits() const;
  bool are_slots_full() const;
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

signals:

  void signal_game_won(uint32_t iPlayerNumber);

private:

  CCricketMainWindow * mGameWindow;
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
  uint32_t mSlot15 = 0;
  uint32_t mSlot16 = 0;
  uint32_t mSlot17 = 0;
  uint32_t mSlot18 = 0;
  uint32_t mSlot19 = 0;
  uint32_t mSlot20 = 0;
  uint32_t mSlot25 = 0;
  uint32_t mExtra15 = 0;
  uint32_t mExtra16 = 0;
  uint32_t mExtra17 = 0;
  uint32_t mExtra18 = 0;
  uint32_t mExtra19 = 0;
  uint32_t mExtra20 = 0;
  uint32_t mExtra25 = 0;
  uint32_t mScore = 0;
  QVector<uint32_t> mSlot15Array = {0};
  QVector<uint32_t> mSlot16Array = {0};
  QVector<uint32_t> mSlot17Array = {0};
  QVector<uint32_t> mSlot18Array = {0};
  QVector<uint32_t> mSlot19Array = {0};
  QVector<uint32_t> mSlot20Array = {0};
  QVector<uint32_t> mSlot25Array = {0};
  QVector<uint32_t> mExtra15Array = {0};
  QVector<uint32_t> mExtra16Array = {0};
  QVector<uint32_t> mExtra17Array = {0};
  QVector<uint32_t> mExtra18Array = {0};
  QVector<uint32_t> mExtra19Array = {0};
  QVector<uint32_t> mExtra20Array = {0};
  QVector<uint32_t> mExtra25Array = {0};
  QVector<uint32_t> mScoreArray = {0};
  QVector<bool> mLegWinArray = {false};
  const CSettings & mpSettings;
};

#endif  // CRICKETCLASS_H
