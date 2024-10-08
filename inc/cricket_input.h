#ifndef CRICKET_INPUT_H
#define CRICKET_INPUT_H

#include <QDialog>
#include <QGraphicsView>
#include "dartboard_segment.h"
#include <QLCDNumber>
#include <QLabel>
#include <QGraphicsSvgItem>
#include "dartboard_cricket.h"
#include "settings.h"

class ICricketMainWindow;
class CCricketClass;
class CCricketGroupBox;

namespace Ui
{
  class CCricketInput;
}

inline std::unordered_map<uint32_t, ECricketSlots> Slot2IdxMap =
{
  {15, ECricketSlots::SLOT_15},
  {16, ECricketSlots::SLOT_16},
  {17, ECricketSlots::SLOT_17},
  {18, ECricketSlots::SLOT_18},
  {19, ECricketSlots::SLOT_19},
  {20, ECricketSlots::SLOT_20},
  {25, ECricketSlots::SLOT_25}
};

#ifndef TESTING
class CCricketInput : public QDialog
{
  Q_OBJECT
#else
class CCricketInput
{
  friend class CCricketInputTest;
#endif

public:

#ifdef TESTING
  CCricketInput(CDartBoard * iDb, ICricketMainWindow * iMainWindow)
    : mGameWindow(iMainWindow)
    , mDartBoard(static_cast<CDartBoardCricket*>(iDb))
  {}
#else
  CCricketInput(QWidget * iParent, const CSettings & iSettings,
                CCricketClass * iPlayer = nullptr, ICricketMainWindow * iGameWindow = nullptr);
#endif
  ~CCricketInput();
  virtual bool are_slots_full() const;
  void handle_segment_pressed_event(uint32_t iVal, QChar iType);
  void notify_cricket_submit_button_clicked(uint32_t iNumberOfDarts, QVector<QString> & iDarts);

#ifndef TESTING
private slots:
#else
private:
#endif

  void submit_button_clicked_slot();
  void undo_button_clicked_slot();

private:

  void compute_score();
  void set_score_labels(uint32_t iValue, QChar iType);
  void process_segment_cutthroat();
  void process_segment_default();
  void compute_cutthroat_scores_for_other_players(QVector<uint32_t> & iScores);
  void process_segment_common(uint32_t iVal, QChar & iType);
  void save_history();
  void handle_game_shot();
  void check_if_game_shot_cutthroat(QVector<uint32_t> & iScores);
  void handle_input_stop();
  void handle_warnings(bool iWarningCondition);
  void handle_slots_and_extra_points(uint32_t iVal, QChar & iType);
  void increase_extra_points(uint32_t iSlotIdx, uint32_t iSlotVal, uint32_t iHits);

  Ui::CCricketInput * mUi;
  QGraphicsView * mView;
  QGraphicsScene * mScene;
  uint32_t mCounter = 3;
  bool mStop = false;
  QLCDNumber * mScoreDart1;
  QLCDNumber * mScoreDart2;
  QLCDNumber * mScoreDart3;
  QVector<QString> mDarts{"","",""};
  CCricketClass * mPlayer;
  ICricketMainWindow * mGameWindow;
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mCutThroatExtraPointsArray;
  std::array<QVector<QVector<uint32_t>>, static_cast<int>(ECricketSlots::SLOT_MAX)> mCutThroatExtraPointsHistory;
  uint32_t mScore = 0;
  uint32_t mPlayerNumber = 0;
  CDartBoardCricket * mDartBoard;
#ifdef TESTING
  CSettings mSettings;
#else
  const CSettings & mSettings;
#endif
  CCricketGroupBox * mGroupBox;
#ifdef TESTING
  std::stringstream mOutput = std::stringstream("");
#endif
};

#endif  // CRICKET_INPUT_H
