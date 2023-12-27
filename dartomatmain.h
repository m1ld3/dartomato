#ifndef DARTOMATMAIN_H
#define DARTOMATMAIN_H

#include <QMainWindow>
#include "xo1mainwindow.h"
#include "cricketmainwindow.h"

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

  CDartomatMain(QWidget *parent = nullptr);
  ~CDartomatMain();

private slots:

  void on_pushButton_startgame_clicked();

private:

  Ui::CDartomatMain * mUi;
  CX01MainWindow * mX01MainWindow;
  CCricketMainWindow * mCricketMainWindow;
  QSoundEffect mGameOnSound;
};

#endif  // DARTOMATMAIN_H
