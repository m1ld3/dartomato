#include "groupbox_player.h"
#include "ui_groupbox_player.h"
#include "dialognameinput.h"
#include "statswindow.h"
#include "QPixmap"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <QMessageBox>
#include <QString>
#include <algorithm>
#include <QBarSet>
#include <QtCharts>
#include <gamewindow.h>

GroupBox_player::GroupBox_player(QWidget *parent, int player_nr, int game, int sets, int legs, bool singleIn,
                                 bool singleOut, bool doubleIn, bool doubleOut, bool masterIn,
                                 bool masterOut, bool offensive, PlayerClass *player, DartBoard * dartboard) :
    QGroupBox(parent), ui(new Ui::GroupBox_player), Player(player), mDartBoard(dartboard), StartVal(game),
    Sets(sets), Legs(legs), Remaining(game), mCurrentScore(0), SingleIn(singleIn), SingleOut(singleOut),
    DoubleIn(doubleIn), DoubleOut(doubleOut), MasterIn(masterIn),
    MasterOut(masterOut), Offensive(offensive), Finished(false), SetBegin(false), LegBegin(false),
    sexy69(this), anotherone(this), sound1(this), sound2(this), sound3(this), sound4(this), sound5(this),
    sound6(this), sound7(this), sound8(this), sound9(this), sound10(this), sound11(this), scoresound(this)
{
    ui->setupUi(this);
    ui->lcdNumber->setDigitCount(3);
    ui->lcdNumber->display(game);
    ui->lcdNumber->setPalette(Qt::darkBlue);
    QString text = "Player " + QString::number(player_nr);
    ui->label_playername->setText(text);
    int w = 80;
    int h = 80;
    if (Active) {
        ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    } else {
        ui->label_pic->clear();
    }
    QString avg1dart = QString::number(Player->get_avg1dart(),'f',3);
    QString avg3dart = QString::number(Player->get_avg3dart(),'f',3);
    QString checkout = QString::number(Player->get_checkout(),'f',3) + "%";
    ui->label_1dartInput->setText(avg1dart);
    ui->label_3dartInput->setText(avg3dart);
    ui->label_checkoutInput->setText(checkout);
    connect(ui->label_pic,SIGNAL(signalPlayerActiveButtonPressed()),this,SLOT(signalPlayerActiveButtonPressed()));
    displayFinishes(Remaining, 3);
    mGameWindow = dynamic_cast<GameWindow*>(parent);
    sexy69.setSource(QUrl("qrc:/resources/sounds/sexy69.wav"));
    sexy69.setLoopCount(1);
    sexy69.setVolume(1);
    anotherone.setSource(QUrl("qrc:/resources/sounds/anotherone.wav"));
    anotherone.setLoopCount(1);
    anotherone.setVolume(1);
    sound1.setSource(QUrl("qrc:/resources/sounds/yousuck1.wav"));
    sound1.setLoopCount(1);
    sound1.setVolume(1);
    sound2.setSource(QUrl("qrc:/resources/sounds/yousuck2.wav"));
    sound2.setLoopCount(1);
    sound2.setVolume(1);
    sound3.setSource(QUrl("qrc:/resources/sounds/yousuck3.wav"));
    sound3.setLoopCount(1);
    sound3.setVolume(1);
    sound4.setSource(QUrl("qrc:/resources/sounds/youfuckedup.wav"));
    sound4.setLoopCount(1);
    sound4.setVolume(1);
    sound5.setSource(QUrl("qrc:/resources/sounds/poorjob.wav"));
    sound5.setLoopCount(1);
    sound5.setVolume(1);
    sound6.setSource(QUrl("qrc:/resources/sounds/nichtgut.wav"));
    sound6.setLoopCount(1);
    sound6.setVolume(1);
    sound7.setSource(QUrl("qrc:/resources/sounds/newwaytosuck.wav"));
    sound7.setLoopCount(1);
    sound7.setVolume(1);
    sound8.setSource(QUrl("qrc:/resources/sounds/loser.wav"));
    sound8.setLoopCount(1);
    sound8.setVolume(1);
    sound9.setSource(QUrl("qrc:/resources/sounds/littlegirl.wav"));
    sound9.setLoopCount(1);
    sound9.setVolume(1);
    sound10.setSource(QUrl("qrc:/resources/sounds/gutschlecht.wav"));
    sound10.setLoopCount(1);
    sound10.setVolume(1);
    sound11.setSource(QUrl("qrc:/resources/sounds/daswarscheisse.wav"));
    sound11.setLoopCount(1);
    sound11.setVolume(1);
    scoresound.setLoopCount(1);
    scoresound.setVolume(1);
    connect(&scoresound, &QSoundEffect::playingChanged, this, [this]{
                if(!scoresound.isPlaying() && Offensive) play_offensive_sounds();
    });
}

GroupBox_player::~GroupBox_player()
{
    delete ui;
}

void GroupBox_player::setActive()
{
    Active = true;
    int w = 80;
    int h = 80;
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void GroupBox_player::setInactive()
{
    Active = false;
    ui->label_pic->clear();
}

void GroupBox_player::reset()
{
    Remaining = StartVal;
    ui->lcdNumber->display(Remaining);
    Player->resetScore();
}

void GroupBox_player::setFinished()
{
    Finished = true;
}

void GroupBox_player::unsetFinished()
{
    Finished = false;
}

QString GroupBox_player::getPlayerName()
{
    QString name = ui->label_playername->text();
    return name;
}

void GroupBox_player::on_pushButton_name_clicked()
{
    QString pretext = ui->label_playername->text();
    DialogNameinput *dn = new DialogNameinput(this, pretext);
    dn->setAttribute(Qt::WA_DeleteOnClose);
    connect(dn,SIGNAL(okButtonClicked(QString&)),this,SLOT(okButtonClicked(QString&)));
    dn->show();
}

void GroupBox_player::okButtonClicked(QString& name)
{
    playername = name;
    ui->label_playername->setText(playername);
}

void GroupBox_player::submitScore(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts)
{
    mCurrentScore = score;
    GroupBox_player::legstarted = true;
    GroupBox_player::setstarted = true;
    bool newset = false;
    Remaining = Player->set_score(mCurrentScore);
    Player->set_darts(darts);
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << mCurrentScore;
    std::string digits = ss.str();
    std::string strpath = "qrc:/resources/sounds/" + digits +".wav";
    QString filepath = QString::fromStdString(strpath);
    scoresound.setSource(filepath);
    Player->compute_averages(numberofdarts);
    if (Remaining == 0) {
        Player->compute_checkout(checkoutattempts, 1);
        newset = Player->increase_setslegs();
        emit signalUpdateHistory();
        scoresound.play();
        emit signalResetScores();
        if (Active && !newset) {
            emit signalUpdatePlayer("leg");
            GroupBox_player::legstarted = false;
        } else if (Active && newset) {
            emit signalUpdatePlayer("set");
            GroupBox_player::setstarted = false;
            GroupBox_player::legstarted = false;
        }
        ui->lcdNumber_legs->display(Player->get_legs());
        ui->lcdNumber_sets->display(Player->get_sets());
    } else {
        Player->compute_checkout(checkoutattempts, 0);
        scoresound.play();
        ui->lcdNumber->display(Remaining);
        if (Active) {
            emit signalUpdatePlayer("default");
        }
    }
    QString avg1dart = QString::number(Player->get_avg1dart(),'f',3);
    QString avg3dart = QString::number(Player->get_avg3dart(),'f',3);
    QString checkout = QString::number(Player->get_checkout(),'f',3) + "%";
    ui->label_1dartInput->setText(avg1dart);
    ui->label_3dartInput->setText(avg3dart);
    ui->label_checkoutInput->setText(checkout);
    displayFinishes(Remaining, 3);
}

void GroupBox_player::signalPlayerActiveButtonPressed()
{
    if (!Active) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                              QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            emit signalInactivatePlayers(Player->getPlayerName(),GroupBox_player::legstarted, GroupBox_player::setstarted);
            setActive();
            mDartBoard->initDartBoard(Remaining);
        }
    }
}

void GroupBox_player::setSetBegin()
{
    SetBegin = true;
}

void GroupBox_player::unsetSetBegin()
{
    SetBegin = false;
}

void GroupBox_player::setLegBegin()
{
    LegBegin = true;
    ui->label_pic->setStyleSheet("#label_pic { border: 5px solid red; }");
}

void GroupBox_player::unsetLegBegin()
{
    LegBegin = false;
    ui->label_pic->setStyleSheet("");
}

bool GroupBox_player::hasBegunLeg()
{
    return LegBegin;
}

bool GroupBox_player::hasBegunSet()
{
    return SetBegin;
}

void GroupBox_player::updateHistory()
{
    Player->update_history();
}

void GroupBox_player::resetLegs()
{
    Player->resetLegs();
}

void GroupBox_player::displayFinishes(int remaining, int numberOfDarts)
{
    QMap<int, QVector<QString>> doubleoutSingleDartCheckoutList =
    {
        {2,  {"D01"}},
        {4,  {"D02"}},
        {6,  {"D03"}},
        {8,  {"D04"}},
        {10, {"D05"}},
        {12, {"D06"}},
        {14, {"D07"}},
        {16, {"D08"}},
        {18, {"D09"}},
        {20, {"D10"}},
        {22, {"D11"}},
        {24, {"D12"}},
        {26, {"D13"}},
        {28, {"D14"}},
        {30, {"D15"}},
        {32, {"D16"}},
        {34, {"D17"}},
        {36, {"D18"}},
        {38, {"D19"}},
        {40, {"D20"}},
        {50, {"D25"}}
    };
    QMap<int, QVector<QString>> doubleoutTwoDartCheckoutList =
    {
        {3,   {"S01   D01"}},
        {5,   {"S01   D02"}},
        {6,   {"S02   D02"}},
        {7,   {"S03   D04"}},
        {9,   {"S01   D04", "S05   D02"}},
        {10,  {"S02   D04", "S06   D02"}},
        {11,  {"S03   D04", "S07   D02"}},
        {13,  {"S05   D04", "S09   D02"}},
        {14,  {"S02   D06", "S06   D04", "S10   D02"}},
        {15,  {"S03   D06", "S07   D04", "S11   D02"}},
        {17,  {"S01   D08", "S05   D06", "S09   D04", "S13   D02"}},
        {18,  {"S02   D08", "S06   D06", "S10   D04", "S14   D02"}},
        {19,  {"S03   D08", "S07   D06", "S11   D04", "S15   D02"}},
        {21,  {"S05   D08", "S01   D10", "S09   D06", "S13   D04", "S17   D02"}},
        {22,  {"S06   D08", "S02   D10", "S10   D06", "S14   D04", "S18   D02"}},
        {23,  {"S07   D08", "S03   D10", "S11   D06", "S15   D04", "S19   D02"}},
        {25,  {"S09   D08", "S01   D12", "S05   D10", "S13   D06", "S17   D04"}},
        {26,  {"S10   D08", "S02   D12", "S06   D10", "S14   D06", "S18   D04"}},
        {27,  {"S11   D08", "S03   D12", "S07   D10", "S15   D06", "S19   D04"}},
        {29,  {"S13   D08", "S01   D14", "S05   D12", "S09   D10", "S17   D06"}},
        {30,  {"S14   D08", "S02   D14", "S06   D12", "S10   D10", "S18   D06"}},
        {31,  {"S15   D08", "S03   D14", "S07   D12", "S11   D10", "S19   D06"}},
        {33,  {"S01   D16", "S17   D08", "S05   D14", "S09   D12", "S13   D10"}},
        {34,  {"S02   D16", "S18   D08", "S06   D14", "S10   D12", "S14   D10"}},
        {35,  {"S03   D16", "S19   D08", "S07   D14", "S11   D12", "S15   D10"}},
        {37,  {"S01   D18", "S05   D16", "S09   D14", "S13   D12", "S17   D10"}},
        {38,  {"S02   D18", "S06   D16", "S10   D14", "S14   D12", "S18   D10"}},
        {39,  {"S03   D18", "S07   D16", "S11   D14", "S15   D12", "S19   D10"}},
        {41,  {"S01   D20", "S09   D16", "S05   D18", "S13   D14", "S17   D12"}},
        {42,  {"S02   D20", "S10   D16", "S06   D18", "S14   D14", "S18   D12"}},
        {43,  {"S03   D20", "S11   D16", "S07   D18", "S15   D14", "S19   D12"}},
        {44,  {"S04   D20", "S12   D16", "S08   D18", "S16   D14", "S20   D12"}},
        {45,  {"S05   D20", "S13   D16", "S09   D18", "S17   D14"}},
        {46,  {"S06   D20", "S14   D16", "S10   D18", "S18   D14"}},
        {47,  {"S07   D20", "S15   D16", "S11   D18", "S19   D14"}},
        {48,  {"S08   D20", "S16   D16", "S12   D18", "S20   D14"}},
        {49,  {"S09   D20", "S17   D16", "S13   D18"}},
        {50,  {"S10   D20", "S18   D16", "S14   D18"}},
        {51,  {"S11   D20", "S19   D16", "S15   D18"}},
        {52,  {"S12   D20", "S20   D16", "S16   D18"}},
        {53,  {"S13   D20", "S17   D18"}},
        {54,  {"S14   D20", "S18   D18"}},
        {55,  {"S15   D20", "S19   D18"}},
        {56,  {"S16   D20", "S20   D18"}},
        {57,  {"S17   D20"}},
        {58,  {"S18   D20"}},
        {59,  {"S19   D20"}},
        {60,  {"S20   D20"}},
        {61,  {"T15   D08", "S25   D18", "T11   D14", "T19   D02"}},
        {62,  {"T10   D16", "T14   D10", "T18   D04"}},
        {63,  {"T13   D12", "T17   D06", "T09   D18"}},
        {64,  {"T16   D08", "T08   D20", "T12   D14", "T20   D02"}},
        {65,  {"T11   D16", "T19   D04", "T15   D10", "S24   D20"}},
        {66,  {"T14   D12", "T10   D18", "T18   D06"}},
        {67,  {"T17   D08", "T09   D20", "T13   D14"}},
        {68,  {"T20   D04", "T12   D16", "T16   D10"}},
        {69,  {"T19   D06", "T11   D18", "T15   D12"}},
        {70,  {"T18   D08", "T10   D20", "T14   D14"}},
        {71,  {"T13   D16", "T17   D10"}},
        {72,  {"T16   D12", "D18   D18", "T12   D18", "T20   D06"}},
        {73,  {"T19   D08", "T11   D20", "T15   D14"}},
        {74,  {"T14   D16", "T18   D10"}},
        {75,  {"T17   D12", "T13   D18"}},
        {76,  {"T20   D08", "T12   D20", "T16   D14"}},
        {77,  {"T19   D10", "T15   D16"}},
        {78,  {"T18   D12", "T14   D18"}},
        {79,  {"T13   D20", "T17   D14", "T19   D11"}},
        {80,  {"T20   D10", "T16   D16", "D20   D20"}},
        {81,  {"T19   D12", "T15   D18"}},
        {82,  {"T14   D20", "T18   D14", "D25   D16"}},
        {83,  {"T17   D16"}},
        {84,  {"T20   D12", "T16   D18"}},
        {85,  {"T15   D20", "T19   D14"}},
        {86,  {"T18   D16"}},
        {87,  {"T17   D18"}},
        {88,  {"T20   D14", "T16   D20"}},
        {89,  {"T19   D16"}},
        {90,  {"T18   D18", "T20   D15"}},
        {91,  {"T17   D20"}},
        {92,  {"T20   D16"}},
        {93,  {"T19   D18"}},
        {94,  {"T18   D20"}},
        {95,  {"T19   D19"}},
        {96,  {"T20   D18"}},
        {97,  {"T19   D20"}},
        {98,  {"T20   D19"}},
        {100, {"T20   D20"}},
        {101, {"T17   D25"}},
        {104, {"T18   D25"}},
        {107, {"T19   D25"}},
        {110, {"T20   D25"}}
    };
    QMap<int, QVector<QString>> doubleoutThreeDartCheckoutList =
    {
        {99,  {"T19   S10   D16", "T20   S07   D16", "T18   S13   D16", "T17   S16   D16",
               "T16   S19   D16", "T19   S02   D20", "T18   S05   D20", "T17   S08   D20",
               "T16   S11   D20", "T15   S14   D20", "T14   S17   D20", "T13   S20   D20",
               "T20   S03   D18", "T19   S06   D18", "T18   S09   D18", "T17   S12   D18",
               "T16   S15   D18", "T15   S18   D18", "T20   S11   D14", "T19   S14   D14",
               "T18   S17   D14", "T17   S20   D14", "T20   S15   D12", "T19   S18   D12",
               "T20   S19   D10"}},
        {101, {"T20   S09   D16", "T19   S12   D16", "T18   S15   D16", "T17   S18   D16",
               "T20   S01   D20", "T19   S04   D20", "T18   S07   D20", "T17   S10   D20",
               "T16   S13   D20", "T15   S16   D20", "T14   S19   D20", "T20   S05   D18",
               "T19   S08   D18", "T18   S11   D18", "T17   S14   D18", "T16   S17   D18",
               "T15   S20   D18", "T20   S13   D14", "T19   S16   D14", "T18   S19   D14",
               "T20   S17   D12", "T19   S20   D12"}},
        {102, {"T20   S02   D20", "T19   S05   D20", "T18   S08   D20", "T17   S11   D20",
               "T16   S14   D20", "T15   S17   D20", "T14   S20   D20", "T20   S06   D18",
               "T19   S09   D18", "T18   S12   D18", "T17   S15   D18", "T16   S18   D18",
               "T20   S10   D16", "T19   S13   D16", "T18   S16   D16", "T17   S19   D16",
               "T20   S14   D14", "T19   S17   D14", "T18   S20   D14", "T20   S18   D12"}},
        {103, {"T20   S03   D20", "T19   S06   D20", "T18   S09   D20", "T15   S18   D20",
               "T20   S11   D16", "T19   S14   D16", "T18   S17   D16", "T17   S20   D16",
               "T19   S10   D18", "T20   S07   D18", "T17   S16   D18", "T16   S19   D18",
               "T18   S13   D18", "T19   S18   D14", "T20   S15   D14", "T20   S19   D12"}},
        {104, {"T16   S16   D20", "T20   S04   D20", "T19   S07   D20", "T18   S10   D20",
               "T17   S13   D20", "T15   S19   D20", "T20   S12   D16", "T19   S15   D16",
               "T18   S18   D16", "T20   S08   D18", "T19   S11   D18", "T18   S14   D18",
               "T17   S17   D18", "T16   S20   D18", "T20   S20   D12", "T19   S19   D14",
               "T20   S16   D14"}},
        {105, {"T20   S05   D20", "T19   S08   D20", "T18   S11   D20", "T17   S14   D20",
               "T16   S17   D20", "T15   S20   D20", "T20   S13   D16", "T19   S16   D16",
               "T18   S19   D16", "T20   S09   D18", "T19   S12   D18", "T18   S15   D18",
               "T17   S18   D18", "T20   S17   D14", "T19   S20   D14"}},
        {106, {"T20   S06   D20", "T19   S09   D20", "T18   S12   D20", "T20   S14   D16",
               "T19   S17   D16", "T18   S20   D16", "T20   S10   D18", "T19   S13   D18",
               "T18   S16   D18", "T17   S19   D18"}},
        {107, {"T20   S07   D20", "T19   S10   D20", "T18   S13   D20", "T17   S16   D20",
               "T16   S19   D20", "T20   S15   D16", "T19   S18   D16", "T20   S11   D18",
               "T19   S14   D18", "T18   S17   D18", "T17   S20   D18", "T20   S19   D14"}},
        {108, {"T20   S08   D20", "T19   S11   D20", "T18   S14   D20", "T17   S17   D20",
               "T16   S20   D20", "T20   S16   D16", "T19   S19   D16", "T20   S12   D18",
               "T19   S15   D18", "T18   S18   D18", "T20   S20   D14"}},
        {109, {"T20   S09   D20", "T19   S12   D20", "T18   S15   D20", "T17   S18   D20",
               "T20   S17   D16", "T19   S20   D16", "T20   S13   D18", "T19   S16   D18",
               "T18   S19   D18"}},
        {110, {"T20   S10   D20", "T19   S13   D20", "T18   S16   D20", "T17   S19   D20",
               "D25   S20   D20", "T20   S18   D16", "T20   S14   D18", "T19   S17   D18",
               "T18   S20   D18"}},
        {111, {"T20   S11   D20", "T19   S14   D20", "T18   S17   D20", "T17   S20   D20",
               "T20   S19   D16", "T20   S15   D18", "T19   S18   D18"}},
        {112, {"T20   S12   D20", "T19   S15   D20", "T18   S18   D20", "T20   S20   D16",
               "T20   S16   D18", "T19   S19   D18"}},
        {113, {"T20   S13   D20", "T19   S16   D20", "T18   S19   D20", "T20   S17   D18",
               "T19   S20   D18"}},
        {114, {"T20   S14   D20", "T19   S17   D20", "T18   S20   D20", "T20   S18   D18"}},
        {115, {"T20   S19   D18", "T20   S15   D20", "T19   S18   D20"}},
        {116, {"T20   S16   D20", "T19   S19   D20", "T20   S20   D18"}},
        {117, {"T20   S17   D20", "T19   S20   D20"}},
        {118, {"T20   S18   D20"}},
        {119, {"T20   S19   D20", "T19   S12   D25"}},
        {120, {"T20   S20   D20"}},
        {121, {"T20   S11   D25", "T20   T15   D08", "T19   T16   D08", "T18   T17   D08",
               "T17   T10   D20", "T16   T11   D20", "T15   T12   D20", "T14   T13   D20",
               "T20   T19   D02", "T20   S25   D18", "T19   T14   D11", "T19   S14   D25",
               "T18   S17   D25", "T17   S20   D25"}},
        {122, {"T18   S18   D25", "T20   T10   D16", "T19   T11   D16", "T18   T12   D16",
               "T17   T13   D16", "T16   T14   D16", "T15   T15   D16", "T20   T18   D04",
               "T19   T19   D04", "T19   S25   D20", "T20   T14   D10", "T19   T15   D10",
               "T18   T16   D10", "T17   T17   D10", "T20   S12   D25", "T19   S15   D25"}},
        {123, {"T19   S16   D25", "T20   T17   D06", "T19   T18   D06", "T20   T13   D12",
               "T19   T14   D12", "T18   T15   D12", "T17   T16   D12", "T20   T09   D18",
               "T19   T10   D18", "T18   T11   D18", "T17   T12   D18", "T16   T13   D18",
               "T15   T14   D18", "T20   S13   D25", "T18   S19   D25", "T16   S25   D25"}},
        {124, {"T20   S14   D25", "T20   T20   D02", "T20   T16   D08", "T20   T08   D20",
               "T20   T12   D14", "T17   T11   D20", "T17   T15   D14", "T14   T14   D20",
               "T19   T17   D08", "T19   T13   D14", "T19   T09   D20", "T19   S17   D25",
               "T18   T18   D08", "T18   T14   D14", "T18   T10   D20", "T18   S20   D25",
               "T16   T16   D14", "T16   T12   D20", "T15   T13   D20"}},
        {125, {"T20   S25   D20", "T20   S15   D25", "T20   T19   D04", "T20   T15   D10",
               "T20   T11   D16", "T18   T17   D10", "T18   T13   D16", "T19   T20   D04",
               "T19   T16   D10", "T19   T12   D16", "T19   S18   D25", "T17   T14   D16",
               "T16   T15   D16", "T15   T10   D25", "T14   T11   D25", "T13   T12   D25",
               "S25   D25   D25"}},
        {126, {"T19   S19   D25", "T20   T10   D18", "T19   T11   D18", "T18   T12   D18",
               "T18   D18   D18", "T17   T13   D18", "T16   T14   D18", "T15   T15   D18",
               "T20   T14   D12", "T19   T15   D12", "T18   T16   D12", "T17   T17   D12",
               "T20   T18   D06", "T19   T19   D06", "T20   S16   D25", "T17   S25   D25"}},
        {127, {"T20   S17   D25", "T20   T17   D08", "T20   T09   D20", "T20   T13   D14",
               "T17   T12   D20", "T17   T16   D14", "T19   T18   D08", "T19   T10   D20",
               "T19   T14   D14", "T19   T12   D17", "T18   T11   D20", "T18   T15   D14",
               "T16   T13   D20", "T15   T14   D20"}},
        {128, {"T20   S18   D25", "T20   T12   D16", "T20   T20   D04", "T20   T16   D10",
               "T19   T13   D16", "T18   T14   D16", "T17   T15   D16", "T16   T16   D16",
               "T19   T17   D10", "T18   T18   D10", "T16   T10   D25", "T15   T11   D25",
               "T14   T12   D25", "T13   T13   D25"}},
        {129, {"T20   S19   D25", "T20   T11   D18", "T19   T12   D18", "T18   T13   D18",
               "T17   T14   D18", "T16   T15   D18", "T20   T15   D12", "T19   T16   D12",
               "T18   T17   D12", "T20   T19   D06", "T18   S25   D25"}},
        {130, {"T20   S20   D25", "T20   T18   D08", "T20   T10   D20", "T20   T14   D14",
               "T19   T19   D08", "T19   T11   D20", "T18   T12   D20", "T17   T13   D20",
               "T16   T14   D20", "T15   T15   D20", "T19   T15   D14", "T18   T16   D14",
               "T17   T17   D14"}},
        {131, {"T20   T13   D16", "T19   T14   D16", "T18   T15   D16", "T17   T16   D16",
               "T20   T17   D10", "T19   T18   D10", "T17   T10   D25", "T16   T11   D25",
               "T15   T12   D25", "T14   T13   D25"}},
        {132, {"T19   S25   D25", "T20   T20   D06", "T20   T12   D18", "T20   D18   D18",
               "T19   T13   D18", "T18   T14   D18", "T17   T15   D18", "T16   T16   D18",
               "T20   T16   D12", "T19   T17   D12", "T18   T18   D12"}},
        {133, {"T20   T19   D08", "T20   T11   D20", "T19   T12   D20", "T18   T13   D20",
               "T17   T14   D20", "T16   T15   D20", "T20   T15   D14", "T19   T16   D14",
               "T18   T17   D14", "T19   D19   D19"}},
        {134, {"T20   T14   D16", "T19   T15   D16", "T18   T16   D16", "T17   T17   D16",
               "T20   T18   D10", "T19   T19   D10", "T16   T16   D19", "T18   T10   D25",
               "T17   T11   D25", "T16   T12   D25", "T15   T13   D25", "T14   T14   D25"}},
        {135, {"T20   S25   D25", "T20   T13   D18", "T19   T14   D18", "T18   T15   D18",
               "T17   T16   D18", "T20   T17   D12", "T19   T18   D12", "T20   T15   D15",
               "T18   T17   D15", "T19   T16   D15"}},
        {136, {"T20   T20   D08", "T20   T12   D20", "T19   T13   D20", "T18   T14   D20",
               "T17   T15   D20", "T16   T16   D20", "T20   T16   D14", "T19   T17   D14",
               "T18   T18   D14", "T20   T14   D17", "T19   T15   D17", "T18   T16   D17",
               "T17   T17   D17", "T20   T18   D11", "T19   T19   D11"}},
        {137, {"T20   T19   D10", "T20   T15   D16", "T19   T16   D16", "T18   T17   D16",
               "T20   T13   D19", "T19   T14   D19", "T18   T15   D19", "T17   T16   D19",
               "T19   T10   D25", "T18   T11   D25", "T17   T12   D25", "T16   T13   D25",
               "T15   T14   D25"}},
        {138, {"T20   T18   D12", "T19   T19   D12", "T20   T14   D18", "T19   T15   D18",
               "T18   T16   D18", "T17   T17   D18", "T20   T16   D15", "T19   T17   D15",
               "T18   T18   D15", "T20   T20   D09"}},
        {139, {"T19   T14   D20", "T20   T13   D20", "T18   T15   D20", "T17   T16   D20",
               "T20   T17   D14", "T19   T18   D14", "T20   T15   D17", "T19   T16   D17",
               "T18   T17   D17", "T20   T19   D11"}},
        {140, {"T20   T20   D10", "T20   T16   D16", "T19   T17   D16", "T18   T18   D16",
               "T17   T17   D19", "T20   D20   D20", "T20   T10   D25", "T19   T11   D25",
               "T18   T12   D25", "T17   T13   D25", "T16   T14   D25"}},
        {141, {"T20   T19   D12", "T20   T15   D18", "T19   T16   D18", "T18   T17   D18",
               "T20   T17   D15", "T19   T18   D15"}},
        {142, {"T20   T14   D20", "T19   T15   D20", "T18   T16   D20", "T17   T17   D20",
               "T20   T18   D14", "T19   T19   D14", "T20   T16   D17", "T19   T17   D17",
               "T18   T18   D17", "T20   T20   D11", "T14   D25   D25"}},
        {143, {"T20   T17   D16", "T19   T18   D16", "T20   T15   D19", "T19   T16   D19",
               "T18   T17   D19", "T20   T19   D13", "T20   T11   D25", "T19   T12   D25",
               "T18   T13   D25", "T17   T14   D25", "T16   T15   D25"}},
        {144, {"T20   T20   D12", "T20   T16   D18", "T19   T17   D18", "T18   T18   D18",
               "T20   T18   D15", "T19   T19   D15"}},
        {145, {"T20   T15   D20", "T19   T16   D20", "T18   T17   D20", "T20   T19   D14",
               "T20   T17   D17", "T19   T18   D17", "T15   D25   D25"}},
        {146, {"T20   T18   D16", "T19   T19   D16", "T20   T20   D13", "T20   T16   D19",
               "T19   T17   D19", "T18   T18   D19", "T20   T12   D25", "T19   T13   D25",
               "T18   T14   D25", "T17   T15   D25", "T16   T16   D25"}},
        {147, {"T20   T17   D18", "T19   T18   D18", "T20   T19   D15"}},
        {148, {"T20   T20   D14", "T20   T16   D20", "T19   T17   D20", "T18   T18   D20",
               "T20   T18   D17", "T19   T19   D17", "T16   D25   D25"}},
        {149, {"T20   T19   D16", "T20   T17   D19", "T19   T18   D19", "T20   T13   D25",
               "T19   T14   D25", "T18   T15   D25", "T17   T16   D25"}},
        {150, {"T20   T18   D18", "T19   T19   D18", "T20   T20   D15", "T20   D20   D25",
               "D25   D25   D25"}},
        {151, {"T20   T17   D20", "T19   T18   D20", "T20   T19   D17", "T17   D25   D25"}},
        {152, {"T20   T20   D16", "T19   T19   D19", "T20   T18   D19", "T20   T14   D25",
               "T19   T15   D25", "T18   T16   D25", "T17   T17   D25"}},
        {153, {"T20   T19   D18"}},
        {154, {"T20   T18   D20", "T19   T19   D20", "T20   T20   D17", "T18   D25   D25"}},
        {155, {"T20   T19   D19", "T20   T15   D25", "T19   T16   D25", "T18   T17   D25"}},
        {156, {"T20   T20   D18"}},
        {157, {"T20   T19   D20", "T19   D25   D25"}},
        {158, {"T20   T20   D19", "T20   T16   D25", "T19   T17   D25", "T18   T18   D25"}},
        {160, {"T20   T20   D20", "T20   D25   D25"}},
        {161, {"T20   T17   D25", "T19   T18   D25"}},
        {164, {"T20   T18   D25", "T19   T19   D25"}},
        {167, {"T20   T19   D25"}},
        {170, {"T20   T20   D25"}}
    };

    QVector<int> vals = {60,57,54,51,50,48,45,42,40,39,38,36,36,34,33,32,30,30,28,27,
                         26,25,24,24,22,21,20,20,19,18,18,18,17,16,16,15,15,14,14,13,
                         12,12,12,11,10,10,9,9,8,8,7,6,6,6,5,4,4,3,3,2,2,1};
    QVector<int> doubles = {50,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2};
    QVector<int> triples = {60,57,54,51,48,45,42,39,36,33,30,27,24,21,18,15,12,9,6,3};
    QVector<QString> valslabels = {"T20","T19","T18","T17","D25","T16","T15","T14","D20","T13","D19","T12",
                                  "D18","D17","T11","D16","T10","D15","D14","T09","D13","S25","T08","D12",
                                  "D11","T07","D10","S20","S19","T06","D09","S18","S17","D08","S16","T05","S15",
                                  "D07","S14","S13","T04","D06","S12","S11","D05","S10","T03","S09","D04","S08",
                                  "S07","T02","D03","S06","S05","D02","S04","T01","S03","D01","S02","S01"};
    QVector<QString> doubleslabels = {"D25","D20","D19","D18","D17","D16","D15","D14","D13","D12","D11",
                                      "D10","D09","D08","D07","D06","D05","D04","D03","D02","D01"};
    QVector<QString> tripleslabels = {"T20","T19","T18","T17","T16","T15","T14","T13","T12","T11","T10","T09",
                                      "T08","T07","T06","T05","T04","T03","T02","T01"};
    ui->textBrowser->clear();
    ui->textBrowser->setText("Checkouts:");
    if (SingleOut) {
        for (int i = 0; i < vals.size(); i++) {
            if (remaining - vals[i] == 0) {
                QString text = valslabels[i];
                ui->textBrowser->append(text);
            }
        }
        if (numberOfDarts > 1)
        {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    if (remaining - vals[i]-vals[j] == 0) {
                        QString text = valslabels[i] + "   " + valslabels[j];
                        ui->textBrowser->append(text);
                    }
                }
            }
        }
        if (remaining > 50 && numberOfDarts > 2) {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    for (int k = 0; k < vals.size(); k++) {
                        if (remaining - vals[i]-vals[j]-vals[k] == 0) {
                            QString text = valslabels[i] + "   " + valslabels[j] + "   " + valslabels[k];
                            ui->textBrowser->append(text);
                        }
                    }
                }
            }
        }
    } else if (DoubleOut) {
//        for (int i = 0; i < doubles.size(); i++) {
//            if (remaining - doubles[i] == 0) {
//                QString text = doubleslabels[i];
//                ui->textBrowser->append(text);
//            }
//        }
//        if (numberOfDarts > 1)
//        {
//            for (int i = vals.size()-1; i >= 0; i--) {
//                for (int j = 0; j < doubles.size(); j++) {
//                    if (remaining - vals[i]-doubles[j] == 0) {
//                        QString text = valslabels[i] + "   " + doubleslabels[j];
//                        ui->textBrowser->append(text);
//                    }
//                }
//            }
//        }
//        if (remaining > 50 && numberOfDarts > 2) {
//            for (int i = 0; i < vals.size(); i++) {
//                for (int j = 0; j < vals.size(); j++) {
//                    for (int k = 0; k < doubles.size(); k++) {
//                        if (remaining - vals[i]-vals[j]-doubles[k] == 0) {
//                            QString text = valslabels[i] + "   " + valslabels[j] + "   " + doubleslabels[k];
//                            ui->textBrowser->append(text);
//                        }
//                    }
//                }
//            }
//        }
        if (doubleoutSingleDartCheckoutList.find(remaining) != doubleoutSingleDartCheckoutList.end())
        {
            QVector<QString> checkouts = doubleoutSingleDartCheckoutList.find(remaining).value();
            for (auto checkout : checkouts)
            {
                ui->textBrowser->append(checkout);
            }
        }
        if (numberOfDarts > 1)
        {
            if (doubleoutTwoDartCheckoutList.find(remaining) != doubleoutTwoDartCheckoutList.end())
            {
                QVector<QString> checkouts = doubleoutTwoDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
                }
            }
        }
        if (numberOfDarts >2)
        {
            if (doubleoutThreeDartCheckoutList.find(remaining) != doubleoutThreeDartCheckoutList.end())
            {
                QVector<QString> checkouts = doubleoutThreeDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
                }
            }
        }
    } else if (MasterOut) {
        for (int i = 0; i < triples.size(); i++) {
            if (remaining - vals[i] == 0) {
                QString text = tripleslabels[i];
                ui->textBrowser->append(text);
            }
        }
        if (numberOfDarts > 1)
        {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < triples.size(); j++) {
                    if (remaining - vals[i]-triples[j] == 0) {
                        QString text = valslabels[i] + "   " + tripleslabels[j];
                        ui->textBrowser->append(text);
                    }
                }
            }
        }
        if (remaining > 50 && numberOfDarts > 2) {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    for (int k = 0; k < triples.size(); k++) {
                        if (remaining - vals[i]-vals[j]-triples[k] == 0) {
                            QString text = valslabels[i] + "   " + valslabels[j] + "   " + tripleslabels[k];
                            ui->textBrowser->append(text);
                        }
                    }
                }
            }
        }
    }
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void GroupBox_player::playFailSounds()
{
    std::srand(static_cast<unsigned> (std::time(0)));
    int rnd = 1 + (std::rand() % 11);
    switch (rnd) {
    case 1:
        sound1.play();
        break;
    case 2:
        sound2.play();
        break;
    case 3:
        sound3.play();
        break;
    case 4:
        sound4.play();
        break;
    case 5:
        sound5.play();
        break;
    case 6:
        sound6.play();
        break;
    case 7:
        sound7.play();
        break;
    case 8:
        sound8.play();
        break;
    case 9:
        sound9.play();
        break;
    case 10:
        sound10.play();
        break;
    case 11:
        sound11.play();
        break;
    default: {}
    }
}

void GroupBox_player::setLcdLegs()
{
    ui->lcdNumber_legs->display(Player->get_legs());
}

void GroupBox_player::play_offensive_sounds()
{
    if (!(Remaining == StartVal && mCurrentScore > 0))
    {
        std::srand(static_cast<unsigned> (std::time(0)));
        int rnd = 1 + (std::rand() % 3);
        switch (mCurrentScore) {
        case 3:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
                playFailSounds();
                break;
            case 3:
                break;
            }
            break;
        case 7:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
                playFailSounds();
                break;
            case 3:
                break;
            }
            break;
        case 11:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
                playFailSounds();
                break;
            case 3:
                break;
            }
            break;
        case 26:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
                playFailSounds();
                break;
            case 3:
                break;
            }
            break;
        case 41:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
            case 3:
                break;
            }
            break;
        case 45:
            switch (rnd) {
            case 1:
                anotherone.play();
                break;
            case 2:
            case 3:
                break;
            }
            break;
        case 69:
            sexy69.play();
            break;
        default:
            if (mCurrentScore < 30) {
                switch (rnd) {
                case 1:
                    playFailSounds();
                    break;
                case 2:
                    playFailSounds();
                    break;
                case 3:
                    break;
                }
            }
        }
    }
}

int GroupBox_player::getRemaining()
{
    return Player->get_remaining();
}

void GroupBox_player::performUndo()
{
    Player->undo();
    Remaining = Player->get_remaining();
    ui->lcdNumber->display(Remaining);
    mDartBoard->initDartBoard(Remaining);
    ui->lcdNumber_legs->display(Player->get_legs());
    ui->lcdNumber_sets->display(Player->get_sets());
    QString avg1dart = QString::number(Player->get_avg1dart(),'f',3);
    QString avg3dart = QString::number(Player->get_avg3dart(),'f',3);
    QString checkout = QString::number(Player->get_checkout(),'f',3) + "%";
    ui->label_1dartInput->setText(avg1dart);
    ui->label_3dartInput->setText(avg3dart);
    ui->label_checkoutInput->setText(checkout);
    if (Finished) {
        unsetFinished();
    }
    displayFinishes(Remaining, 3);
}

void GroupBox_player::on_pushButton_stats_clicked()
{
    StatsWindow *stats = new StatsWindow;
    stats->setAttribute(Qt::WA_DeleteOnClose);
    stats->setModal(true);
    QBarSet *setScores = new QBarSet("Scores");
    QBarSet *setDarts = new QBarSet("Single Darts");
    QVector<QVector<QString>> thrownDarts = Player->get_darts();
    QVector<QString> thrownDarts_flat;
    for (auto darts : thrownDarts)
    {
        thrownDarts_flat.append(darts);
    }
    for (auto &dart : thrownDarts_flat)
    {
        if (dart[0] == "d")
        {
            QString temp = dart.remove(0,1);
            dart = "D" + QString::number(temp.toInt() / 2);
        }
        else if (dart[0] == "t")
        {
            QString temp = dart.remove(0,1);
            dart = "T" + QString::number(temp.toInt() / 3);
        }
        else
        {
            QString temp = dart.remove(0,1);
            dart = "S" + temp;
        }
    }
    std::map<QString, int> dart_counts;
    for (auto dart : thrownDarts_flat) ++dart_counts[dart];
    QVector<int> allScores = Player->get_total_scores();
    std::map<int, int> score_counts;
    for (auto score : allScores) ++score_counts[score];
    QBarSeries *series = new QBarSeries();
    QBarSeries *series2 = new QBarSeries();
    series->append(setScores);
    series2->append(setDarts);
    Chart *chart = new Chart();
    Chart *chart2 = new Chart();
    chart->addSeries(series);
    chart2->addSeries(series2);
    chart->setTitle("Scoring statistics");
    chart2->setTitle("Single dart statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart2->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    QStringList categories2;
    std::map<int, int>::iterator it;
    for (it = score_counts.begin(); it != score_counts.end(); it++)
    {
        setScores->append(it->second);
        categories.append(QString::number(it->first));
    }
    std::map<QString, int>::iterator it2;
    int idx = 0;
    for (it2 = dart_counts.begin(); it2 != dart_counts.end(); it2++)
    {
        setDarts->append(it2->second);
        categories2.append(it2->first);
        idx++;
    }
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
    if (categories.size()) axisX->setMin(categories.first());
    axisX->append(categories);
    if (categories2.size()) axisX2->setMin(categories2.first());
    axisX2->append(categories2);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chart2->addAxis(axisX2, Qt::AlignBottom);
    series2->attachAxis(axisX2);
    QValueAxis *axisY = new QValueAxis();
    QValueAxis *axisY2 = new QValueAxis();
    std::map<int, int>::iterator best = std::max_element(score_counts.begin(), score_counts.end(), [] (const std::pair<int, int>& a, const std::pair<int, int>& b)->bool{return a.second < b.second;});
    qreal max = static_cast<qreal>(best->second);
    axisY->setRange(0, max);
    axisY->setTickType(QValueAxis::TicksFixed);
    axisY->setTickCount(std::min(std::max(2,static_cast<int>(max)+1),10));
    axisY->setLabelFormat("%i");
    std::map<QString, int>::iterator best2 = std::max_element(dart_counts.begin(), dart_counts.end(), [] (const std::pair<QString, int>& a, const std::pair<QString, int>& b)->bool{return a.second < b.second;});
    qreal max2 = static_cast<qreal>(best2->second);
    axisY2->setRange(0, max2);
    axisY2->setTickType(QValueAxis::TicksFixed);
    axisY2->setTickCount(std::min(std::max(2,static_cast<int>(max2)+1),10));
    axisY2->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    chart2->addAxis(axisY2, Qt::AlignLeft);
    series->attachAxis(axisY);
    series2->attachAxis(axisY2);
    stats->setChart(chart, chart2);
    stats->setLabel1DartAvg(Player->get_avg1dart());
    stats->setLabel3DartAvg(Player->get_avg3dart());
    stats->setLabelCheckout(Player->get_checkout());
    QVector<int> legscores = Player->get_LegScores();
    QVector<QVector<int>> totalscores = Player->get_TotalScores();
    if (legscores.size() == 0 && totalscores.size() > 0) {
        legscores = totalscores.last();
    }
    for (int i = 0; i < legscores.size(); i++) {
        QString line = QString::number(i+1) + ": " + QString::number(legscores[i]);
        stats->setText(line);
    }
    stats->show();
    QObject::connect(setScores, &QBarSet::hovered,this,[setScores](bool status, int index){
        QPoint p = QCursor::pos();
        if (status) {
            QToolTip::showText(p,QString::number(setScores->at(index)));
        }
    });
    QObject::connect(setDarts, &QBarSet::hovered,this,[setDarts](bool status, int index){
        QPoint p = QCursor::pos();
        if (status) {
            QToolTip::showText(p,QString::number(setDarts->at(index)));
        }
    });
}

void GroupBox_player::on_pushButton_undo_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Undo",
                                                               tr("Are you sure you want to undo your last score?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::No);
    if (resBtn == QMessageBox::Yes)
    {
        performUndo();
    }
}

bool GroupBox_player::legstarted = false;
bool GroupBox_player::setstarted = false;
