#ifndef CRICKETMAINWINDOW_H
#define CRICKETMAINWINDOW_H

#include <QMainWindow>
#include "groupbox_cricket.h"
#include <QSoundEffect>
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
  bool is_slot15_free(uint32_t iPlayer) const;
  bool is_slot16_free(uint32_t iPlayer) const;
  bool is_slot17_free(uint32_t iPlayer) const;
  bool is_slot18_free(uint32_t iPlayer) const;
  bool is_slot19_free(uint32_t iPlayer) const;
  bool is_slot20_free(uint32_t iPlayer) const;
  bool is_slot25_free(uint32_t iPlayer) const;
  bool is_score_bigger(uint32_t iScore) const;
  bool is_score_smaller(uint32_t iScore) const;
  void increase_score15(uint32_t iPoints);
  void increase_score16(uint32_t iPoints);
  void increase_score17(uint32_t iPoints);
  void increase_score18(uint32_t iPoints);
  void increase_score19(uint32_t iPoints);
  void increase_score20(uint32_t iPoints);
  void increase_score25(uint32_t iPoints);
  QVector<uint32_t> compute_extra15s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra16s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra17s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra18s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra19s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra20s(uint32_t iPoints, uint32_t iPlayer);
  QVector<uint32_t> compute_extra25s(uint32_t iPoints, uint32_t iPlayer);
  void set_scores();
  void update_labels();
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
};

#endif  // CRICKETMAINWINDOW_H
