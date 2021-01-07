#ifndef X01MAINWINDOW_H
#define X01MAINWINDOW_H

#include <QMainWindow>
#include "groupbox_player.h"
#include <QSound>
#include "dartboard.h"

namespace Ui {
class X01MainWindow;
}

class X01MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit X01MainWindow(QWidget *parent = nullptr);
    explicit X01MainWindow(QWidget *parent = nullptr, int numberofplayers = 1, int game = 501, int sets = 1, int legs = 1,
                        bool singleIn = true, bool singleOut = false, bool doubleIn = false, bool doubleOut = false,
                        bool masterIn = false, bool masterOut = false, bool offensive = false);
    ~X01MainWindow();
    void updatePlayer();
    void closeEvent(QCloseEvent *event) override;
    void setActivePlayer(int player);

private slots:
    void signalUpdatePlayer(QString type);
    void signalResetScores();
    void signalGameWon(int playername);
    void signalInactivatePlayers(int player, bool legstarted, bool setstarted);
    void signalUpdateHistory();
    void submitButtonPressedSlot(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);
    void on_submitButton_clicked();
    void on_undoButton_clicked();
    void displayScoreSlot(int score);
    void displayDart1Slot(int val);
    void displayDart2Slot(int val);
    void displayDart3Slot(int val);
    void eraseDart1Slot();
    void eraseDart2Slot();
    void eraseDart3Slot();
    void updateFinishesSlot(int score, int numberOfDarts);
    void setFocusToSubmitButtonSlot();

private:
    Ui::X01MainWindow *ui;
    GroupBox_player *gb;
    int NumberOfPlayers, Game, Sets, Legs, ActivePlayer;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Offensive;
    QVector<GroupBox_player*> playerbox;
    QVector<PlayerClass*> mplayer;
    DartBoard * mDartBoard;
};

#endif // X01MAINWINDOW_H
