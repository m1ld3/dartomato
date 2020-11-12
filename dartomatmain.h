#ifndef DARTOMATMAIN_H
#define DARTOMATMAIN_H

#include <QMainWindow>
#include "gamewindow.h"

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
    GameWindow *gw;

};
#endif // DARTOMATMAIN_H
