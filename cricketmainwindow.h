#ifndef CRICKETMAINWINDOW_H
#define CRICKETMAINWINDOW_H

#include <QMainWindow>
#include "groupbox_cricket.h"
#include <QSoundEffect>
#include <QGridLayout>

class groupbox_cricket;

namespace Ui {
class CricketMainWindow;
}

class CricketMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CricketMainWindow(QWidget *parent = nullptr);
    explicit CricketMainWindow(QWidget *parent = nullptr, int numberofplayers = 1, int sets = 1, int legs = 1, bool cutthroat = false, bool offensive = false);
    ~CricketMainWindow();
    void updatePlayer();
    void closeEvent(QCloseEvent *event) override;
    void setActivePlayer(int player);
    bool isSlot15free(int player);
    bool isSlot16free(int player);
    bool isSlot17free(int player);
    bool isSlot18free(int player);
    bool isSlot19free(int player);
    bool isSlot20free(int player);
    bool isSlot25free(int player);
    bool isScoreBigger(int score);
    bool isScoreSmaller(int score);
    void increaseScore15(int points);
    void increaseScore16(int points);
    void increaseScore17(int points);
    void increaseScore18(int points);
    void increaseScore19(int points);
    void increaseScore20(int points);
    void increaseScore25(int points);
    QVector<int> computeExtra15s(int points, int player);
    QVector<int> computeExtra16s(int points, int player);
    QVector<int> computeExtra17s(int points, int player);
    QVector<int> computeExtra18s(int points, int player);
    QVector<int> computeExtra19s(int points, int player);
    QVector<int> computeExtra20s(int points, int player);
    QVector<int> computeExtra25s(int points, int player);
    void setScores();
    void updateLabels();
    void updateDarts(int player);

private slots:
    void signalUpdatePlayer(QString type);
    void signalResetScores();
    void signalGameWon(int playername);
    void signalInactivatePlayers(int player, bool legstarted, bool setstarted);
    void signalUpdateHistory();

private:
    Ui::CricketMainWindow *ui;
    groupbox_cricket *gbc;
    int NumberOfPlayers, Sets, Legs, ActivePlayer;
    bool CutThroat, Offensive;
    QVector<groupbox_cricket*> cricketbox;
    QVector<cricketclass*> mcricketplayer;
};

#endif // CRICKETMAINWINDOW_H
