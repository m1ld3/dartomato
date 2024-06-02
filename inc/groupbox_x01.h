#ifndef GROUPBOX_X01_H
#define GROUPBOX_X01_H

#include <QGroupBox>
#include "x01_class.h"
#include <QPixmap>
#include <QSoundEffect>
#include "settings.h"

class CX01MainWindow;
class CDartBoard;
class CGameDataHandler;
#ifdef TESTING
#include "mocks.h"
#endif

namespace Ui
{
  class CX01GroupBox;
}

#ifdef TESTING
class CX01GroupBox
{
  friend class CX01GroupBoxTest;
#else
class CX01GroupBox : public QGroupBox
{
  Q_OBJECT
#endif

public:

#ifdef TESTING
  explicit CX01GroupBox(const CSettings iSettings, uint32_t iPlayerNumber = 0, CDartBoard * iDartBoard = nullptr);
#else
  explicit CX01GroupBox(QWidget * iParent, const CSettings iSettings,
                        uint32_t iPlayerNumber = 0, CDartBoard * iDartboard = nullptr);
  ~CX01GroupBox() override;
#endif

  void set_active();
  void set_inactive();
  void set_finished();
  void set_set_begin();
  void unset_set_begin();
  void set_leg_begin();
  void unset_leg_begin();
  bool has_begun_leg() const;
  bool has_begun_set() const;
  void reset_legs();
  void reset();
  void set_lcd_legs();
  void create_snapshot();
  bool is_finished() const { return mFinished; }
  QVector<CX01Class::CPlayerData> & get_game_data() { return mHistory; };
  void set_game_data(QVector<CX01Class::CPlayerData> iGameData);

  uint32_t get_remaining_points() const;
  void submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts);
  void display_finishes(uint32_t iRemaining, uint32_t iNumberOfDarts);

#ifdef TESTING
private:
#else
private slots:
#endif

  void player_active_button_pressed_slot();
  void push_button_undo_clicked_slot();
  void push_button_stats_clicked_slot();

private:

  void unset_finished();

  void update_players(const EUpdateType iType);
  void reset_scores_of_all_players();
  void inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void perform_undo();
  void connect_slots();
  void display_stats_and_finishes();
  void set_lcd_legs_and_sets();
  void play_score_sound();
  void handle_game_shot(uint32_t iCheckoutAttempts);
  void handle_default_score(uint32_t iCheckoutAttempts);
  const QMap<uint32_t, QVector<QString>> & get_checkout_map(uint32_t iNumberOfDarts);
  void prepare_score_sound();
  void create_snapshots_of_all_players();

  Ui::CX01GroupBox * mUi;
  CX01Class mPlayer;
  CDartBoard * mDartBoard;
  const CSettings mSettings;
  QString mPlayerName;
  uint32_t mPlayerNumber, mRemainingPoints;
  uint32_t mCurrentScore = 0;
  bool mActive = false;
  bool mFinished = false;
  bool mSetBegin = false;
  bool mLegBegin = false;
#ifndef TESTING
  QPixmap mPixMapHand = QPixmap(":/resources/img/hand.svg");
  QPixmap mPixMapDot = QPixmap(":/resources/img/dot.png");
#endif
  static bool mLegAlreadyStarted;
  static bool mSetAlreadyStarted;
  CX01MainWindow * mGameWindow;
  QSoundEffect mScoreSound;
  QVector<CX01Class::CPlayerData> mHistory;
};

#endif  // GROUPBOX_X01_H
