#ifndef DARTOMAT_MAINWINDOW_H
#define DARTOMAT_MAINWINDOW_H

#include <QMainWindow>
#include "x01_mainwindow.h"
#include "cricket_mainwindow.h"
#include "settings.h"
#include "game_data_handler.h"
#include "player_list_model.h"

class CGameDataHandler;

QT_BEGIN_NAMESPACE
namespace Ui
{
  class CDartomatMain;
}
QT_END_NAMESPACE

class CDartomatMain : public QMainWindow
{
  Q_OBJECT

public:

  CDartomatMain(QWidget * iParent = nullptr);
  ~CDartomatMain();
  void handle_selected_players(const QStringList & iSelectedPlayers);
  void start_game(CSettings iSettings);
  void resume_game(const CGameDataHandler::SGameData iGameData);
  
private slots:

  void push_button_startgame_clicked_slot();
  void combo_box_game_current_index_changed_slot(const QString & iGame);
  void push_button_select_players_clicked_slot();
  void show_about_dialog();
  void push_button_game_history_clicked_slot();
  void show_unfinished_game_popup(const QString & iTimeStamp);
  void push_button_stats_clicked_slot();

private:

  void play_game_on_sound();
  void create_menu();
  void check_for_unfinished_game();

  Ui::CDartomatMain * mUi;
  QPointer<CX01MainWindow> mX01MainWindow;
  QPointer<CCricketMainWindow> mCricketMainWindow;
#ifndef USE_TTS
  QSoundEffect mGameOnSound;
#endif
  CGameDataHandler mGameDataHandler;
  CPlayerListModel mPlayerListModel;
  QStringList mSelectedPlayers = {};
};

#endif  // DARTOMAT_MAINWINDOW_H
