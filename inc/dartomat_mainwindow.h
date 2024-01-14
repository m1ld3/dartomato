#ifndef DARTOMAT_MAINWINDOW_H
#define DARTOMAT_MAINWINDOW_H

#include <QMainWindow>
#include "x01_mainwindow.h"
#include "cricket_mainwindow.h"
#include "settings.h"

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
  void add_new_players(const QVector<QString> & iPlayers);

private slots:

  void push_button_startgame_clicked_slot();
  void combo_box_game_current_index_changed_slot(const QString & iGame);
  void push_button_select_players_clicked_slot();

private:

  void play_game_on_sound();

  Ui::CDartomatMain * mUi;
  QPointer<CX01MainWindow> mX01MainWindow;
  QPointer<CCricketMainWindow> mCricketMainWindow;
#ifndef USE_TTS
  QSoundEffect mGameOnSound;
#endif
  CSettings mSettings;
  QPointer<CGameDataHandler> mGameDataHandler;
};

#endif  // DARTOMAT_MAINWINDOW_H
