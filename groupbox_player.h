#ifndef GROUPBOX_PLAYER_H
#define GROUPBOX_PLAYER_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "scoreinput.h"
#include "playerclass.h"
#include <QPixmap>
#include <QSound>
#include <QSoundEffect>
class GameWindow;

namespace Ui {
class GroupBox_player;
}

class GroupBox_player : public QGroupBox
{
    Q_OBJECT

public:
    explicit GroupBox_player(QWidget *parent = nullptr, int game = 501, int player_nr = 1, int sets = 1, int legs = 1,
                             bool singleIn = true, bool singleOut = false, bool doubleIn = false, bool doubleOut = false,
                             bool masterIn = false, bool masterOut = false, bool offensive = false, PlayerClass *player = nullptr);
    ~GroupBox_player();
    void setActive();
    void setInactive();
    void reset();
    void setFinished();
    void unsetFinished();
    void closeScoreInput();
    QString getPlayerName();
    void setSetBegin();
    void unsetSetBegin();
    void setLegBegin();
    void unsetLegBegin();
    bool hasBegunLeg();
    bool hasBegunSet();
    void updateHistory();
    void resetLegs();
    void displayFinishes();
    void playFailSounds();
    void setLcdLegs();
    void play_offensive_sounds();

signals:
    void signalUpdatePlayer(QString type);
    void signalResetScores();
    void signalInactivatePlayers(int player, bool legstarted, bool setstarted);
    void signalUpdateHistory();

public slots:
    void on_pushButton_name_clicked();
    void on_pushButton_score_clicked();
    void okButtonClicked(QString& name);
    void signalSubmitButtonPressed2(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);
    void signalPlayerActiveButtonPressed();
private slots:
    void on_pushButton_stats_clicked();
    void on_pushButton_undo_clicked();

private:
    Ui::GroupBox_player *ui;
    QString playername;
    DialogNameinput *dn;
    ScoreInput *scoreinput;
    PlayerClass *Player;
    int StartVal, Player_Nr, Sets, Legs, Score, mCurrentScore;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Offensive, Active, Finished, SetBegin, LegBegin;
    QPixmap pix = QPixmap(":/resources/img/darts.svg");
    static bool legstarted;
    static bool setstarted;
    GameWindow* mGameWindow;
//    QSound *sexy69 = new QSound(":/resources/sounds/sexy69.wav");
//    QSound *anotherone = new QSound(":/resources/sounds/anotherone.wav");
//    QSound *sound1 = new QSound(":/resources/sounds/yousuck1.wav");
//    QSound *sound2 = new QSound(":/resources/sounds/yousuck2.wav");
//    QSound *sound3 = new QSound(":/resources/sounds/yousuck3.wav");
//    QSound *sound4 = new QSound(":/resources/sounds/youfuckedup.wav");
//    QSound *sound5 = new QSound(":/resources/sounds/poorjob.wav");
//    QSound *sound6 = new QSound(":/resources/sounds/nichtgut.wav");
//    QSound *sound7 = new QSound(":/resources/sounds/newwaytosuck.wav");
//    QSound *sound8 = new QSound(":/resources/sounds/loser.wav");
//    QSound *sound9 = new QSound(":/resources/sounds/littlegirl.wav");
//    QSound *sound10 = new QSound(":/resources/sounds/gutschlecht.wav");
//    QSound *sound11 = new QSound(":/resources/sounds/daswarscheisse.wav");

    QSoundEffect sexy69;
    QSoundEffect anotherone;
    QSoundEffect sound1;
    QSoundEffect sound2;
    QSoundEffect sound3;
    QSoundEffect sound4;
    QSoundEffect sound5;
    QSoundEffect sound6;
    QSoundEffect sound7;
    QSoundEffect sound8;
    QSoundEffect sound9;
    QSoundEffect sound10;
    QSoundEffect sound11;
    QSoundEffect scoresound;

};

#endif // GROUPBOX_PLAYER_H
