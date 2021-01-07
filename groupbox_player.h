#ifndef GROUPBOX_PLAYER_H
#define GROUPBOX_PLAYER_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "dartboard.h"
#include "playerclass.h"
#include <QPixmap>
#include <QSound>
#include <QSoundEffect>

class X01MainWindow;

namespace Ui {
class GroupBox_player;
}

class GroupBox_player : public QGroupBox
{
    Q_OBJECT

public:
    explicit GroupBox_player(QWidget* parent = nullptr, int game = 501, int player_nr = 1, int sets = 1, int legs = 1,
                             bool singleIn = true, bool singleOut = false, bool doubleIn = false, bool doubleOut = false,
                             bool masterIn = false, bool masterOut = false, bool offensive = false, PlayerClass* player = nullptr, DartBoard* dartboard = nullptr);
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
    void displayFinishes(int remaining, int numberOfDarts);
    void playFailSounds();
    void setLcdLegs();
    void play_offensive_sounds();
    int getRemaining();
    void submitScore(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);

signals:
    void signalUpdatePlayer(QString type);
    void signalResetScores();
    void signalInactivatePlayers(int player, bool legstarted, bool setstarted);
    void signalUpdateHistory();

public slots:
    void on_pushButton_name_clicked();
    void okButtonClicked(QString& name);
    void signalPlayerActiveButtonPressed();
    void performUndo();
private slots:
    void on_pushButton_stats_clicked();
    void on_pushButton_undo_clicked();

private:
    Ui::GroupBox_player *ui;
    QString playername;
    DialogNameinput *dn;
    PlayerClass *Player;
    DartBoard * mDartBoard;
    int StartVal, Player_Nr, Sets, Legs, Remaining, mCurrentScore;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Offensive, Active, Finished, SetBegin, LegBegin;
    QPixmap pix = QPixmap(":/resources/img/darts.svg");
    static bool legstarted;
    static bool setstarted;
    X01MainWindow* mGameWindow;
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
