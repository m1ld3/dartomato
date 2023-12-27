#ifndef GROUPBOX_CRICKET_H
#define GROUPBOX_CRICKET_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "cricketinput.h"
#include "cricketclass.h"
#include <QPixmap>
#include <QSoundEffect>

class CCricketMainWindow;

namespace Ui
{
  class CCricketGroupBox;
}

class CCricketGroupBox : public QGroupBox
{
  Q_OBJECT

public:

  explicit CCricketGroupBox(QWidget * iParent = nullptr, uint32_t iPlayerNumber = 1, uint32_t iSets = 1, uint32_t iLegs = 1,
                            CCricketClass *iPlayer = nullptr, bool iCutThroat = false, bool iOffensive = false);
  ~CCricketGroupBox() override;
  void set_active();
  void set_inactive();
  void set_finished();
  void unset_finished();
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
  static void set_leg_started();
  static void set_set_started();
  static void unset_leg_started();
  static void unset_set_started();
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
  void set_label_extra15(uint32_t iPoints);
  void set_label_extra16(uint32_t iPoints);
  void set_label_extra17(uint32_t iPoints);
  void set_label_extra18(uint32_t iPoints);
  void set_label_extra19(uint32_t iPoints);
  void set_label_extra20(uint32_t iPoints);
  void set_label_extra25(uint32_t iPoints);
  void set_label15(uint32_t iHits);
  void set_label16(uint32_t iHits);
  void set_label17(uint32_t iHits);
  void set_label18(uint32_t iHits);
  void set_label19(uint32_t iHits);
  void set_label20(uint32_t iHits);
  void set_label25(uint32_t iHits);
  uint32_t get_score() const;
  void set_leg_history();
  void increase_extra15(uint32_t iPoints);
  void increase_extra16(uint32_t iPoints);
  void increase_extra17(uint32_t iPoints);
  void increase_extra18(uint32_t iPoints);
  void increase_extra19(uint32_t iPoints);
  void increase_extra20(uint32_t iPoints);
  void increase_extra25(uint32_t iPoints);
  void set_score();
  void update_labels();
  void update_darts(QVector<QString> && iDarts);
  void play_fail_sounds();
  void set_lcd_legs();
  void perform_undo();

signals:

  void signal_update_player(QString iType);
  void signal_reset_scores();
  void signal_inactivate_players(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void signal_update_history();

public slots:

  void on_pushButton_name_clicked();
  void on_pushButton_score_clicked();
  void ok_button_clicked(QString && iName);
  void signal_cricket_submit_button_pressed(uint32_t iNumberOfDarts, QVector<QString> & iDarts);
  void signal_player_active_button_pressed();
  void on_pushButton_undo_clicked();
  void signal_set_label_slot(uint32_t iHits, uint32_t iSlot);

private:

  Ui::CCricketGroupBox * mUi;
  QString mPlayerName;
  CDialogNameInput *mDialogNameInput;
  CCricketInput *mScoreInput;
  CCricketClass *mPlayer;
  uint32_t mPlayerNumber, mSets, mLegs, mScore;
  bool mActive, mFinished, mSetBegin, mLegBegin, mCutThroat, mOffensive;
  static bool mLegStarted;
  static bool mSetStarted;
  QPixmap mPixMap = QPixmap(":/resources/img/darts.svg");
  CCricketMainWindow* mGameWindow;
  uint32_t mTotalHits;
  uint32_t mSlot15, mSlot16, mSlot17, mSlot18, mSlot19, mSlot20, mSlot25,
  mExtra15, mExtra16, mExtra17, mExtra18, mExtra19, mExtra20, mExtra25;
  QSoundEffect mSound1;
  QSoundEffect mSound2;
  QSoundEffect mSound3;
  QSoundEffect mSound4;
  QSoundEffect mSound5;
  QSoundEffect mSound6;
  QSoundEffect mSound7;
  QSoundEffect mSound8;
  QSoundEffect mSound9;
  QSoundEffect mSound10;
  QSoundEffect mSound11;
};

#endif  // GROUPBOX_CRICKET_H
