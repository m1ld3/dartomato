#ifndef GROUPBOX_CRICKET_H
#define GROUPBOX_CRICKET_H

#include <QGroupBox>
#include "dialognameinput.h"
#include "cricketinput.h"
#include "cricketclass.h"
#include <QPixmap>
class GameWindow;

namespace Ui {
class groupbox_cricket;
}

class groupbox_cricket : public QGroupBox
{
    Q_OBJECT

public:
    explicit groupbox_cricket(QWidget *parent = nullptr, int player_nr = 1, int sets = 1, int legs = 1,
                              cricketclass *player = nullptr, bool cutthroat = false, bool offensive = false);
    ~groupbox_cricket();
    void setActive();
    void setInactive();
    void setFinished();
    void unsetFinished();
    void closeCricketInput();
    QString getPlayerName();
    void setSetBegin();
    void unsetSetBegin();
    void setLegBegin();
    void unsetLegBegin();
    bool hasBegunLeg();
    bool hasBegunSet();
    void updateHistory();
    void resetLegs();
    void reset();
    static void setLegStarted();
    static void setSetStarted();
    static void unsetLegStarted();
    static void unsetSetStarted();
    int getSlot15();
    int getSlot16();
    int getSlot17();
    int getSlot18();
    int getSlot19();
    int getSlot20();
    int getSlot25();
    void setSlot15(int hits);
    void setSlot16(int hits);
    void setSlot17(int hits);
    void setSlot18(int hits);
    void setSlot19(int hits);
    void setSlot20(int hits);
    void setSlot25(int hits);
    void setExtra15(int points);
    void setExtra16(int points);
    void setExtra17(int points);
    void setExtra18(int points);
    void setExtra19(int points);
    void setExtra20(int points);
    void setExtra25(int points);
    int getExtra15();
    int getExtra16();
    int getExtra17();
    int getExtra18();
    int getExtra19();
    int getExtra20();
    int getExtra25();
    void setLabelExtra15(int points);
    void setLabelExtra16(int points);
    void setLabelExtra17(int points);
    void setLabelExtra18(int points);
    void setLabelExtra19(int points);
    void setLabelExtra20(int points);
    void setLabelExtra25(int points);
    void setLabel15(int hits);
    void setLabel16(int hits);
    void setLabel17(int hits);
    void setLabel18(int hits);
    void setLabel19(int hits);
    void setLabel20(int hits);
    void setLabel25(int hits);
    int getScore();
    void setLegHistory();
    void increaseExtra15(int points);
    void increaseExtra16(int points);
    void increaseExtra17(int points);
    void increaseExtra18(int points);
    void increaseExtra19(int points);
    void increaseExtra20(int points);
    void increaseExtra25(int points);
    void setScore();
    void updateLabels();
    void updateDarts(QVector<QString> darts);
    void playFailSounds();
    void setLcdLegs();
    void performUndo();

signals:
    void signalUpdatePlayer(QString type);
    void signalResetScores();
    void signalInactivatePlayers(int player, bool legstarted, bool setstarted);
    void signalUpdateHistory();

public slots:
    void on_pushButton_name_clicked();
    void on_pushButton_score_clicked();
    void okButtonClicked(QString& name);
    void signalSubmitButtonPressed2(int &numberofdarts, QVector<QString> darts);
    void signalPlayerActiveButtonPressed();
    void on_pushButton_undo_clicked();
    void signalSetLabelSlot(int hits, int slot);

private:
    Ui::groupbox_cricket *ui;
    QString playername;
    DialogNameinput *dn;
    cricketinput *scoreinput;
    cricketclass *Player;
    int Player_Nr, Sets, Legs, Score;
    bool Active, Finished, SetBegin, LegBegin, CutThroat, Offensive;
    static bool legstarted;
    static bool setstarted;
    QPixmap pix = QPixmap(":/resources/img/darts.svg");
    GameWindow* mGameWindow;
    int mTotalHits;
    int mSlot15, mSlot16, mSlot17, mSlot18, mSlot19, mSlot20, mSlot25,
    mExtra15, mExtra16, mExtra17, mExtra18, mExtra19, mExtra20, mExtra25;
    QSound *sound1 = new QSound(":/resources/sounds/yousuck1.wav");
    QSound *sound2 = new QSound(":/resources/sounds/yousuck2.wav");
    QSound *sound3 = new QSound(":/resources/sounds/yousuck3.wav");
    QSound *sound4 = new QSound(":/resources/sounds/youfuckedup.wav");
    QSound *sound5 = new QSound(":/resources/sounds/poorjob.wav");
    QSound *sound6 = new QSound(":/resources/sounds/nichtgut.wav");
    QSound *sound7 = new QSound(":/resources/sounds/newwaytosuck.wav");
    QSound *sound8 = new QSound(":/resources/sounds/loser.wav");
    QSound *sound9 = new QSound(":/resources/sounds/littlegirl.wav");
    QSound *sound10 = new QSound(":/resources/sounds/gutschlecht.wav");
    QSound *sound11 = new QSound(":/resources/sounds/daswarscheisse.wav");
};
#endif // GROUPBOX_CRICKET_H
