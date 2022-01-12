#ifndef DARTOMATMAIN_H
#define DARTOMATMAIN_H

#include <QMainWindow>
#include "xo1mainwindow.h"
#include "cricketmainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DartomatMain; }
QT_END_NAMESPACE

class DartomatMain : public QMainWindow
{
    Q_OBJECT

public:
    DartomatMain(QWidget *parent = nullptr);
    ~DartomatMain();


private slots:
    void on_pushButton_startgame_clicked();
    void on_comboBox_game_currentIndexChanged(const QString &arg1);

private:
    Ui::DartomatMain *ui;
    X01MainWindow *x01MainWindow;
    CricketMainWindow * cricketMainWindow;
    QSoundEffect gameon;
};
#endif // DARTOMATMAIN_H
