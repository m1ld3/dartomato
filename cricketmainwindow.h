#ifndef CRICKETMAINWINDOW_H
#define CRICKETMAINWINDOW_H

#include <QMainWindow>
#include "groupbox_cricket.h"
#ifndef USE_TTS
#include <QSoundEffect>
#endif
#include <QGridLayout>

class CCricketGroupBox;

namespace Ui
{
  class CCricketMainWindow;
}

class CCricketMainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit CCricketMainWindow(QWidget * iParent, const CSettings & ipSettings);
  ~CCricketMainWindow() override;
  void update_player();
  void closeEvent(QCloseEvent * iEvent) override;
  void set_active_player(uint32_t iPlayer);
  bool is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const;
  bool is_score_bigger(uint32_t iScore) const;
  bool is_score_smaller(uint32_t iScore) const;
  void increase_slot_score(const ECricketSlots iSlot, uint32_t iPoints);
  QVector<uint32_t> compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer);
  void set_scores();
  void update_extra_points_labels();
  void update_darts(uint32_t iPlayer);

private slots:

  void update_player_slot(const EUpdateType iType);
  void reset_scores_slot();
  void game_won_slot(uint32_t iPlayerNumber);
  void inactivate_players_slot(uint32_t iPlayer, bool iLegStarted, bool iSetStarted);
  void update_history_slot();

private:

  Ui::CCricketMainWindow * mUi;
  CCricketGroupBox * mCricketGroupBox;
  uint32_t mActivePlayer = 0;
  QVector<CCricketGroupBox*> mCricketBox;
  const CSettings & mpSettings;
  QVector<CCricketClass*> mCricketPlayer;
  void handle_update_default();
  void handle_update_leg();
  void handle_update_set();
  void inactivate_all_players();
  void unset_leg_begin_for_all_players();
  void unset_set_begin_for_all_players();
};

#endif  // CRICKETMAINWINDOW_H
