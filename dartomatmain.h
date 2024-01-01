#ifndef DARTOMATMAIN_H
#define DARTOMATMAIN_H

#include <QMainWindow>
#include "xo1mainwindow.h"
#include "cricketmainwindow.h"
#include "settings.h"

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

private slots:

  void push_button_startgame_clicked_slot();
  void combo_box_game_current_index_changed_slot(const QString & iGame);

private:

  Ui::CDartomatMain * mUi;
  QPointer<CX01MainWindow> mX01MainWindow;
  QPointer<CCricketMainWindow> mCricketMainWindow;
  QSoundEffect mGameOnSound;
  CSettings mSettings;
};

#endif  // DARTOMATMAIN_H
