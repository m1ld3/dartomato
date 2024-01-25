#ifndef GROUPBOX_CRICKET_H
#define GROUPBOX_CRICKET_H

#include <QGroupBox>
#include "cricket_input.h"
#include "cricket_class.h"
#include <QPixmap>
#include <QPointer>

class CCricketMainWindow;

namespace Ui
{
  class CCricketGroupBox;
}

class CCricketGroupBox : public QGroupBox
{
  Q_OBJECT

public:

  explicit CCricketGroupBox(QWidget * iParent,
                            const CSettings & iSettings,
                            uint32_t iPlayerNumber = 1,
                            CCricketClass * const iPlayer = nullptr);
  ~CCricketGroupBox() override;
  void set_active();
  void set_inactive();
  void set_finished();
  void close_cricket_input();
  QString get_player_number() const;
  void set_set_begin();
  void unset_set_begin();
  void set_leg_begin();
  void unset_leg_begin();
  bool has_begun_leg() const;
  bool has_begun_set() const;
  void update_history();
  void reset_legs();
  void reset();
  uint32_t get_slot(const ECricketSlots iSlot) const;
  uint32_t get_extra_points(const ECricketSlots iSlot) const;  
  uint32_t get_score() const;
  void increase_extra_points(const ECricketSlots iSlot, uint32_t iPoints);
  void set_score();
  void update_extra_points_labels();
  void update_darts(QVector<QString> && iDarts);
  void set_lcd_legs();
  void handle_submit_button_clicked(uint32_t iNumberOfDarts, QVector<QString> & iDarts);

private slots:

  void push_button_score_clicked_slot();
  void player_active_button_pressed_slot();
  void push_button_undo_clicked_slot();

private:

  void unset_finished();
  static void set_leg_started();
  static void set_set_started();
  static void unset_leg_started();
  static void unset_set_started();
  void set_slot(const ECricketSlots iSlot, uint32_t iHits);
  void set_extra_points(const ECricketSlots iSlot, uint32_t iPoints);
  void set_extra_points_label(const ECricketSlots iSlot, uint32_t iPoints);
  void set_slot_label(const ECricketSlots iSlot, uint32_t iHits);
  void set_leg_history();
  void perform_undo();
  void update_players(const EUpdateType iType);
  void reset_scores_of_all_players();
  void inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void update_history_of_all_players();
  void connect_slots();
  void filter_leg_scores_cutthroat(QVector<QVector<QString>> & oLegScores);
  void display_leg_scores(const QVector<QVector<QString>> & iLegScores);
  void display_leg_score_line(uint32_t iLegNumber, const QVector<QString> & iLegScore);
  QString format_leg_score(const QVector<QString> & iLegScore);
  void load_slot_arrays_from_player();
  void write_slot_arrays_to_player(const std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & iExtraPointsCutThroat);
  void handle_leg_won();
  void handle_switch_to_next_player();
  void process_single_dart(uint32_t iDartIdx, QVector<QString> & oDarts,
                           std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat);
  void submit_score_to_player(uint32_t iNumberOfDarts, const QVector<QString> & iDart,
                              const std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & iExtraPointsCutThroat);
  bool has_leg_won();
  void calculate_extra_points(uint32_t iSlotIdx, uint32_t iHits, uint32_t iSlotVal,
                              std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat);
  void handle_slot_hits_overflow(uint32_t iSlotIdx, uint32_t iHits, uint32_t iSlotVal, QString & oDart,
                                 std::array<uint32_t, static_cast<uint32_t>(ECricketSlots::SLOT_MAX)> & oExtraPointsCutThroat);
  void fill_slot_hits(uint32_t iSlotIdx, uint32_t iHits);

  Ui::CCricketGroupBox * mUi;
  QString mPlayerName;
  QPointer<CCricketInput> mScoreInput;
  CCricketClass * const mPlayer;
  uint32_t mPlayerNumber;
  uint32_t mScore = 0;
  bool mActive = false;
  bool mFinished = false;
  bool mSetBegin = false;
  bool mLegBegin = false;
  static bool mLegStarted;
  static bool mSetStarted;
  QPixmap mPixMap = QPixmap(":/resources/img/darts.svg");
  CCricketMainWindow * mGameWindow;
  uint32_t mTotalHits = 0;
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mSlotArray = {0, 0, 0, 0, 0, 0, 0};
  std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> mExtraPointsArray = {0, 0, 0, 0, 0, 0, 0};
  const CSettings & mSettings;
};

#endif  // GROUPBOX_CRICKET_H
