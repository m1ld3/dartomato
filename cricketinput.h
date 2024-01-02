#ifndef CRICKETINPUT_H
#define CRICKETINPUT_H

#include <QDialog>
#include <QGraphicsView>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include "undobutton.h"
#include "submitbutton.h"
#include <QLabel>
#include <QGraphicsSvgItem>
#include <QSoundEffect>
#include "dartboard_cricket.h"
#include "settings.h"

class CCricketMainWindow;
class CCricketClass;

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

class CCricketInput : public QDialog
{
  Q_OBJECT

public:

  void set_score_labels(uint32_t iValue, QChar iType);
  void compute_score();
  bool are_slots_full() const;

public slots:

  void segment_pressed_slot(uint32_t iVal, QChar & iType);

signals:

  void signal_cricket_submit_button_pressed(uint32_t iNumberOfDarts, const QVector<QString> & iDarts);

public:

  CCricketInput(QWidget * iParent, const CSettings & ipSettings,
                CCricketClass * iPlayer = nullptr, CCricketMainWindow * iGameWindow = nullptr);
  ~CCricketInput();

private slots:

  void submit_button_clicked_slot();
  void undo_button_clicked_slot();

private:

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

  Ui::CCricketInput * mUi;
  QGraphicsView * mView;
  QGraphicsScene * mScene;
  uint32_t mCounter = 3;
  bool mStop = false;
  QLCDNumber * mScoreDart1;
  QLCDNumber * mScoreDart2;
  QLCDNumber * mScoreDart3;
  CUndoButton * mUndoButton;
  CSubmitButton * mSubmitButton;
  QVector<QString> mDarts{"","",""};
  CCricketClass * mPlayer;
  CCricketMainWindow * mGameWindow;
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsHistory = {{{0}, {0}, {0}, {0}, {0}, {0}, {0}}};
  std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> mCutThroatExtraPointsArray;
  std::array<QVector<QVector<uint32_t>>, static_cast<int>(ECricketSlots::SLOT_MAX)> mCutThroatExtraPointsHistory;
  uint32_t mScore;
  CDartBoardCricket * mDartBoard;
  const CSettings & mpSettings;
};

#endif  // CRICKETINPUT_H
