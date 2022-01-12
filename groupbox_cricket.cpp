#include "groupbox_cricket.h"
#include "ui_groupbox_cricket.h"
#include <QMessageBox>
#include "dialognameinput.h"
#include <QDebug>
#include <QSoundEffect>
#include <string>
#include <QString>
#include <algorithm>

groupbox_cricket::groupbox_cricket(QWidget *parent, int player_nr, int sets, int legs,
                                   cricketclass *player, bool cutthroat, bool offensive)
    : QGroupBox(parent), ui(new Ui::groupbox_cricket), Player(player), Player_Nr(player_nr - 1), Sets(sets)
    , Legs(legs), Score(0), Finished(false), SetBegin(false), LegBegin(false), CutThroat(cutthroat)
    , Offensive(offensive), mTotalHits(0), mSlot15(0), mSlot16(0), mSlot17(0), mSlot18(0), mSlot19(0)
    , mSlot20(0), mSlot25(0), mExtra15(0), mExtra16(0), mExtra17(0), mExtra18(0), mExtra19(0), mExtra20(0), mExtra25(0)
    , sound1(this)
    , sound2(this)
    , sound3(this)
    , sound4(this)
    , sound5(this)
    , sound6(this)
    , sound7(this)
    , sound8(this)
    , sound9(this)
    , sound10(this)
    , sound11(this)
{
    ui->setupUi(this);
    ui->lcdNumber->setDigitCount(4);
    ui->lcdNumber->display(Score);
    ui->lcdNumber->setPalette(Qt::darkBlue);
    setLabelExtra15(Player->getExtra15());
    setLabelExtra16(Player->getExtra16());
    setLabelExtra17(Player->getExtra17());
    setLabelExtra18(Player->getExtra18());
    setLabelExtra19(Player->getExtra19());
    setLabelExtra20(Player->getExtra20());
    setLabelExtra25(Player->getExtra25());
    QString text = "Player " + QString::number(player_nr);
    ui->label_playername->setText(text);
    QString hitsPerRound = QString::number(Player->get_hits_per_round(),'f',3);
    ui->label_hitsPerRound->setText(hitsPerRound);
    int w = 80;
    int h = 80;
    if (Active)
    {
        ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
    else
    {
        ui->label_pic->clear();
    }
    connect(ui->label_pic,SIGNAL(signalPlayerActiveButtonPressed()),this,SLOT(signalPlayerActiveButtonPressed()));
    mGameWindow = dynamic_cast<CricketMainWindow*>(parent);

    sound1.setSource(QUrl("qrc:/resources/sounds/yousuck1.wav"));
    sound2.setSource(QUrl("qrc:/resources/sounds/yousuck2.wav"));
    sound3.setSource(QUrl("qrc:/resources/sounds/yousuck3.wav"));
    sound4.setSource(QUrl("qrc:/resources/sounds/youfuckedup.wav"));
    sound5.setSource(QUrl("qrc:/resources/sounds/poorjob.wav"));
    sound6.setSource(QUrl("qrc:/resources/sounds/nichtgut.wav"));
    sound7.setSource(QUrl("qrc:/resources/sounds/newwaytosuck.wav"));
    sound8.setSource(QUrl("qrc:/resources/sounds/loser.wav"));
    sound9.setSource(QUrl("qrc:/resources/sounds/littlegirl.wav"));
    sound10.setSource(QUrl("qrc:/resources/sounds/gutschlecht.wav"));
    sound11.setSource(QUrl("qrc:/resources/sounds/daswarscheisse.wav"));
}

groupbox_cricket::~groupbox_cricket()
{
    delete ui;
}

void groupbox_cricket::setActive()
{
    Active = true;
    int w = 80;
    int h = 80;
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void groupbox_cricket::setInactive()
{
    Active = false;
    ui->label_pic->clear();
}

void groupbox_cricket::setFinished()
{
    Finished = true;
}

void groupbox_cricket::unsetFinished()
{
    Finished = false;
}

void groupbox_cricket::closeCricketInput()
{
    this->scoreinput->close();
}

QString groupbox_cricket::getPlayerName()
{
    QString name = ui->label_playername->text();
    return name;
}

void groupbox_cricket::on_pushButton_name_clicked()
{
    QString pretext = ui->label_playername->text();
    DialogNameinput *dn = new DialogNameinput(this, pretext);
    connect(dn,SIGNAL(okButtonClicked(QString&)),this,SLOT(okButtonClicked(QString&)));
    dn->show();
}

void groupbox_cricket::okButtonClicked(QString& name)
{
    playername = name;
    ui->label_playername->setText(playername);
}

void groupbox_cricket::signalSubmitButtonPressed2(int &numberofdarts, QVector<QString> darts)
{
    groupbox_cricket::legstarted = true;
    groupbox_cricket::setstarted = true;
    bool newset = false;
    int hits = 0;
    int hits_old = 0;
    mSlot15 = Player->getSlot15();
    mSlot16 = Player->getSlot16();
    mSlot17 = Player->getSlot17();
    mSlot18 = Player->getSlot18();
    mSlot19 = Player->getSlot19();
    mSlot20 = Player->getSlot20();
    mSlot25 = Player->getSlot25();
    mExtra15 = Player->getExtra15();
    mExtra16 = Player->getExtra16();
    mExtra17 = Player->getExtra17();
    mExtra18 = Player->getExtra18();
    mExtra19 = Player->getExtra19();
    mExtra20 = Player->getExtra20();
    mExtra25 = Player->getExtra25();
    int newhits = mTotalHits;
    if (!CutThroat)
    {
        QString dart = "0";
        for (int i = 0; i < darts.size(); i++)
        {
            if (darts[i].size() > 0)
            {
                dart = darts[i];
            }
            QChar type = dart[0];
            if (type == 't')
            {
                hits = 3;
            }
            else if (type == 'd')
            {
                hits = 2;
            }
            else
            {
                hits = 1;
            }
            QString temp = dart.remove(0,1);
            int val = temp.toInt();
            switch (val)
            {
            case 15:
                if (mSlot15 < 3)
                {
                    if (mSlot15 + hits <= 3)
                    {
                        mSlot15 += hits;
                        setLabel15(mSlot15);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot15;
                        mSlot15 = 3;
                        setLabel15(mSlot15);
                        if (mGameWindow->isSlot15free(Player_Nr))
                        {
                            mExtra15 += hits * 15;
                            setLabelExtra15(mExtra15);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot15free(Player_Nr))
                    {
                        mExtra15 += hits * 15;
                        setLabelExtra15(mExtra15);
                        mTotalHits += hits;
                    }
                }
                break;
            case 16:
                if (mSlot16 < 3)
                {
                    if (mSlot16 + hits <= 3)
                    {
                        mSlot16 += hits;
                        setLabel16(mSlot16);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot16;
                        mSlot16 = 3;
                        setLabel16(mSlot16);
                        if (mGameWindow->isSlot16free(Player_Nr))
                        {
                            mExtra16 += hits * 16;
                            setLabelExtra16(mExtra16);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot16free(Player_Nr))
                    {
                        mExtra16 += hits * 16;
                        setLabelExtra16(mExtra16);
                        mTotalHits += hits;
                    }
                }
                break;
            case 17:
                if (mSlot17 < 3)
                {
                    if (mSlot17 + hits <= 3)
                    {
                        mSlot17 += hits;
                        setLabel17(mSlot17);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot17;
                        mSlot17 = 3;
                        setLabel17(mSlot17);
                        if (mGameWindow->isSlot17free(Player_Nr))
                        {
                            mExtra17 += hits * 17;
                            setLabelExtra17(mExtra17);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot17free(Player_Nr))
                    {
                        mExtra17 += hits * 17;
                        setLabelExtra17(mExtra17);
                        mTotalHits += hits;
                    }
                }
                break;
            case 18:
                if (mSlot18 < 3)
                {
                    if (mSlot18 + hits <= 3)
                    {
                        mSlot18 += hits;
                        setLabel18(mSlot18);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot18;
                        mSlot18 = 3;
                        setLabel18(mSlot18);
                        if (mGameWindow->isSlot18free(Player_Nr))
                        {
                            mExtra18 += hits * 18;
                            setLabelExtra18(mExtra18);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot18free(Player_Nr))
                    {
                        mExtra18 += hits * 18;
                        setLabelExtra18(mExtra18);
                        mTotalHits += hits;
                    }
                }
                break;
            case 19:
                if (mSlot19 < 3)
                {
                    if (mSlot19 + hits <= 3)
                    {
                        mSlot19 += hits;
                        setLabel19(mSlot19);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot19;
                        mSlot19 = 3;
                        setLabel19(mSlot19);
                        if (mGameWindow->isSlot19free(Player_Nr))
                        {
                            mExtra19 += hits * 19;
                            setLabelExtra19(mExtra19);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot19free(Player_Nr))
                    {
                        mExtra19 += hits * 19;
                        setLabelExtra19(mExtra19);
                        mTotalHits += hits;
                    }
                }
                break;
            case 20:
                if (mSlot20 < 3)
                {
                    if (mSlot20 + hits <= 3)
                    {
                        mSlot20 += hits;
                        setLabel20(mSlot20);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot20;
                        mSlot20 = 3;
                        setLabel20(mSlot20);
                        if (mGameWindow->isSlot20free(Player_Nr))
                        {
                            mExtra20 += hits * 20;
                            setLabelExtra20(mExtra20);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot20free(Player_Nr))
                    {
                        mExtra20 += hits * 20;
                        setLabelExtra20(mExtra20);
                        mTotalHits += hits;
                    }
                }
                break;
            case 25:
                if (mSlot25 < 3)
                {
                    if (mSlot25 + hits <= 3)
                    {
                        mSlot25 += hits;
                        setLabel25(mSlot25);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot25;
                        mSlot25 = 3;
                        setLabel25(mSlot25);
                        if (mGameWindow->isSlot25free(Player_Nr))
                        {
                            mExtra25 += hits * 25;
                            setLabelExtra25(mExtra25);
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot25free(Player_Nr))
                    {
                        mExtra25 += hits * 25;
                        setLabelExtra25(mExtra25);
                        mTotalHits += hits;
                    }
                }
                break;
            default:{}
            }
        }
        Player->setSlot15(mSlot15);
        Player->setSlot16(mSlot16);
        Player->setSlot17(mSlot17);
        Player->setSlot18(mSlot18);
        Player->setSlot19(mSlot19);
        Player->setSlot20(mSlot20);
        Player->setSlot25(mSlot25);
        Player->setExtra15(mExtra15);
        Player->setExtra16(mExtra16);
        Player->setExtra17(mExtra17);
        Player->setExtra18(mExtra18);
        Player->setExtra19(mExtra19);
        Player->setExtra20(mExtra20);
        Player->setExtra25(mExtra25);
        Player->setScore();
        Score = Player->getScore();
    }
    else
    { // if (CutThroat)
        int Extra15CT = 0;
        int Extra16CT = 0;
        int Extra17CT = 0;
        int Extra18CT = 0;
        int Extra19CT = 0;
        int Extra20CT = 0;
        int Extra25CT = 0;
        QString dart = "0";
        for (int i = 0; i < darts.size(); i++)
        {
            if (darts[i].size() > 0)
            {
                dart = darts[i];
            }
            QChar type = dart[0];
            if (type == 't')
            {
                hits = 3;
            }
            else if (type == 'd')
            {
                hits = 2;
            }
            else
            {
                hits = 1;
            }
            QString temp = dart.remove(0,1);
            int val = temp.toInt();
            switch (val)
            {
            case 15:
                if (mSlot15 < 3)
                {
                    if (mSlot15 + hits <= 3)
                    {
                        mSlot15 += hits;
                        setLabel15(mSlot15);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot15;
                        mSlot15 = 3;
                        setLabel15(mSlot15);
                        if (mGameWindow->isSlot15free(Player_Nr))
                        {
                            Extra15CT += hits * 15;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot15free(Player_Nr))
                    {
                        Extra15CT += hits * 15;
                        mTotalHits += hits;
                    }
                }
                break;
            case 16:
                if (mSlot16 < 3)
                {
                    if (mSlot16 + hits <= 3)
                    {
                        mSlot16 += hits;
                        setLabel16(mSlot16);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot16;
                        mSlot16 = 3;
                        setLabel16(mSlot16);
                        if (mGameWindow->isSlot16free(Player_Nr))
                        {
                            Extra16CT += hits * 16;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot16free(Player_Nr))
                    {
                        Extra16CT += hits * 16;
                        mTotalHits += hits;
                    }
                }
                break;
            case 17:
                if (mSlot17 < 3)
                {
                    if (mSlot17 + hits <= 3)
                    {
                        mSlot17 += hits;
                        setLabel17(mSlot17);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot17;
                        mSlot17 = 3;
                        setLabel17(mSlot17);
                        if (mGameWindow->isSlot17free(Player_Nr))
                        {
                            Extra17CT += hits * 17;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot17free(Player_Nr))
                    {
                        Extra17CT += hits * 17;
                        mTotalHits += hits;
                    }
                }
                break;
            case 18:
                if (mSlot18 < 3)
                {
                    if (mSlot18 + hits <= 3)
                    {
                        mSlot18 += hits;
                        setLabel18(mSlot18);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot18;
                        mSlot18 = 3;
                        setLabel18(mSlot18);
                        if (mGameWindow->isSlot18free(Player_Nr))
                        {
                            Extra18CT += hits * 18;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot18free(Player_Nr))
                    {
                        Extra18CT += hits * 18;
                        mTotalHits += hits;
                    }
                }
                break;
            case 19:
                if (mSlot19 < 3)
                {
                    if (mSlot19 + hits <= 3)
                    {
                        mSlot19 += hits;
                        setLabel19(mSlot19);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot19;
                        mSlot19 = 3;
                        setLabel19(mSlot19);
                        if (mGameWindow->isSlot19free(Player_Nr))
                        {
                            Extra19CT += hits * 19;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot19free(Player_Nr))
                    {
                        Extra19CT += hits * 19;
                        mTotalHits += hits;
                    }
                }
                break;
            case 20:
                if (mSlot20 < 3)
                {
                    if (mSlot20 + hits <= 3)
                    {
                        mSlot20 += hits;
                        setLabel20(mSlot20);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot20;
                        mSlot20 = 3;
                        setLabel20(mSlot20);
                        if (mGameWindow->isSlot20free(Player_Nr))
                        {
                            Extra20CT += hits * 20;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot20free(Player_Nr))
                    {
                        Extra20CT += hits * 20;
                        mTotalHits += hits;
                    }
                }
                break;
            case 25:
                if (mSlot25 < 3)
                {
                    if (mSlot25 + hits <= 3)
                    {
                        mSlot25 += hits;
                        setLabel25(mSlot25);
                        mTotalHits += hits;
                    }
                    else
                    {
                        hits_old = hits;
                        hits -= 3 - mSlot25;
                        mSlot25 = 3;
                        setLabel25(mSlot25);
                        if (mGameWindow->isSlot25free(Player_Nr))
                        {
                            Extra25CT += hits * 25;
                            mTotalHits += hits_old;
                        }
                        else
                        {
                            mTotalHits += hits;
                        }
                    }
                }
                else
                {
                    if (mGameWindow->isSlot25free(Player_Nr))
                    {
                        Extra25CT += hits * 25;
                        mTotalHits += hits;
                    }
                }
                break;
            default:{}
            }
        }
        Player->setSlot15(mSlot15);
        Player->setSlot16(mSlot16);
        Player->setSlot17(mSlot17);
        Player->setSlot18(mSlot18);
        Player->setSlot19(mSlot19);
        Player->setSlot20(mSlot20);
        Player->setSlot25(mSlot25);
        mGameWindow->increaseScore15(Extra15CT);
        mGameWindow->increaseScore16(Extra16CT);
        mGameWindow->increaseScore17(Extra17CT);
        mGameWindow->increaseScore18(Extra18CT);
        mGameWindow->increaseScore19(Extra19CT);
        mGameWindow->increaseScore20(Extra20CT);
        mGameWindow->increaseScore25(Extra25CT);
        mGameWindow->setScores();
        Score = Player->getScore();
    }
    if (mTotalHits == newhits && Offensive)
    {
        playFailSounds();
    }
    Player->compute_hits_per_round(numberofdarts, mTotalHits);
    Player->update_darts(darts);
    QString hpr = QString::number(Player->get_hits_per_round(),'f',3);
    ui->label_hitsPerRound->setText(hpr);
    if (!CutThroat)
    {
        if (scoreinput->areSlotsFull() && mGameWindow->isScoreBigger(Score))
        {  // leg won
            newset = Player->increase_setslegs();
            Player->setLegWinArray(true);
            emit signalUpdateHistory();
            emit signalResetScores();
            if (Active && !newset)
            {
                emit signalUpdatePlayer("leg");
                groupbox_cricket::legstarted = false;
            }
            else if (Active && newset)
            {
                emit signalUpdatePlayer("set");
                groupbox_cricket::setstarted = false;
                groupbox_cricket::legstarted = false;
            }
            ui->lcdNumber_legs->display(Player->get_legs());
            ui->lcdNumber_sets->display(Player->get_sets());
        }
        else
        {
            if (Active)
            {
                emit signalUpdatePlayer("default");
            }
            Player->setLegWinArray(false);
            mGameWindow->updateLabels();
        }
    }
    else
    {
        mGameWindow->updateDarts(Player->get_player_name());
        if (scoreinput->areSlotsFull() && mGameWindow->isScoreSmaller(Score))
        {  // leg won
            newset = Player->increase_setslegs();
            Player->setLegWinArray(true);
            emit signalUpdateHistory();
            emit signalResetScores();
            if (Active && !newset)
            {
                emit signalUpdatePlayer("leg");
                groupbox_cricket::legstarted = false;
            }
            else if (Active && newset)
            {
                emit signalUpdatePlayer("set");
                groupbox_cricket::setstarted = false;
                groupbox_cricket::legstarted = false;
            }
            ui->lcdNumber_legs->display(Player->get_legs());
            ui->lcdNumber_sets->display(Player->get_sets());
        }
        else
        {
            if (Active)
            {
                emit signalUpdatePlayer("default");
            }
            Player->setLegWinArray(false);
            mGameWindow->updateLabels();
        }
    }
    setLegHistory();
    closeCricketInput();
}


void groupbox_cricket::signalPlayerActiveButtonPressed()
{
    if (!Active)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                              QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            emit signalInactivatePlayers(Player->get_player_name(),groupbox_cricket::legstarted, groupbox_cricket::setstarted);
            setActive();
        }
    }
}

void groupbox_cricket::on_pushButton_score_clicked()
{
    if (Active && !Finished)
    {
        scoreinput = new cricketinput(this, Sets, Legs, Player, mGameWindow, CutThroat);
        scoreinput->setAttribute(Qt::WA_DeleteOnClose);
        connect(scoreinput, SIGNAL (signalSubmitButtonPressed2(int&, QVector<QString>)), this, SLOT (signalSubmitButtonPressed2(int&, QVector<QString>)));
        scoreinput->show();
    }
    else if (Finished)
    {
        QMessageBox::about(this, "Warning", "Game already finished!");
    }
    else
    {
        QMessageBox::about(this, "Warning", "It's not your turn!");
    }
 }

void groupbox_cricket::setSetBegin()
{
    SetBegin = true;
}

void groupbox_cricket::unsetSetBegin()
{
    SetBegin = false;
}

void groupbox_cricket::setLegBegin()
{
    LegBegin = true;
}

void groupbox_cricket::unsetLegBegin()
{
    LegBegin = false;
}

bool groupbox_cricket::hasBegunLeg()
{
    return LegBegin;
}

bool groupbox_cricket::hasBegunSet()
{
    return SetBegin;
}

void groupbox_cricket::updateHistory()
{
    Player->update_history();
}

void groupbox_cricket::resetLegs()
{
    Player->reset_legs();
}

void groupbox_cricket::reset()
{
    Score = 0;
    Player->reset_score();
    ui->lcdNumber->display(Score);
    ui->lcdNumber_legs->display(Player->get_legs());
    ui->lcdNumber_sets->display(Player->get_sets());
    setLabel15(0);
    setLabel16(0);
    setLabel17(0);
    setLabel18(0);
    setLabel19(0);
    setLabel20(0);
    setLabel25(0);
    setLabelExtra15(0);
    setLabelExtra16(0);
    setLabelExtra17(0);
    setLabelExtra18(0);
    setLabelExtra19(0);
    setLabelExtra20(0);
    setLabelExtra25(0);
}

void groupbox_cricket::setLegStarted()
{
    legstarted = true;
}

void groupbox_cricket::setSetStarted()
{
    setstarted = true;
}

void groupbox_cricket::unsetLegStarted()
{
    legstarted = false;
}

void groupbox_cricket::unsetSetStarted()
{
    setstarted = false;
}

int groupbox_cricket::getSlot15()
{
    return Player->getSlot15();
}

int groupbox_cricket::getSlot16()
{
    return Player->getSlot16();
}

int groupbox_cricket::getSlot17()
{
    return Player->getSlot17();
}

int groupbox_cricket::getSlot18()
{
    return Player->getSlot18();
}

int groupbox_cricket::getSlot19()
{
    return Player->getSlot19();
}

int groupbox_cricket::getSlot20()
{
    return Player->getSlot20();
}

int groupbox_cricket::getSlot25()
{
    return Player->getSlot25();
}

void groupbox_cricket::setSlot15(int hits)
{
    Player->setSlot15(hits);
}

void groupbox_cricket::setSlot16(int hits)
{
    Player->setSlot16(hits);
}

void groupbox_cricket::setSlot17(int hits)
{
    Player->setSlot17(hits);
}

void groupbox_cricket::setSlot18(int hits)
{
    Player->setSlot18(hits);
}

void groupbox_cricket::setSlot19(int hits)
{
    Player->setSlot19(hits);
}

void groupbox_cricket::setSlot20(int hits)
{
    Player->setSlot20(hits);
}

void groupbox_cricket::setSlot25(int hits)
{
    Player->setSlot25(hits);
}

void groupbox_cricket::setExtra15(int points)
{
    Player->setExtra15(points);
}

void groupbox_cricket::setExtra16(int points)
{
    Player->setExtra16(points);
}

void groupbox_cricket::setExtra25(int points)
{
    Player->setExtra25(points);
}

int groupbox_cricket::getExtra15()
{
    return Player->getExtra15();
}

int groupbox_cricket::getExtra16()
{
    return Player->getExtra16();
}

int groupbox_cricket::getExtra17()
{
    return Player->getExtra17();
}

int groupbox_cricket::getExtra18()
{
    return Player->getExtra18();
}

int groupbox_cricket::getExtra19()
{
    return Player->getExtra19();
}

int groupbox_cricket::getExtra20()
{
    return Player->getExtra20();
}

int groupbox_cricket::getExtra25()
{
    return Player->getExtra25();
}

void groupbox_cricket::setLabelExtra15(int points)
{
    ui->label_extra15->setNum(points);
}

void groupbox_cricket::setLabelExtra16(int points)
{
    ui->label_extra16->setNum(points);
}

void groupbox_cricket::setLabelExtra17(int points)
{
    ui->label_extra17->setNum(points);
}

void groupbox_cricket::setLabelExtra18(int points)
{
    ui->label_extra18->setNum(points);
}

void groupbox_cricket::setLabelExtra19(int points)
{
    ui->label_extra19->setNum(points);
}

void groupbox_cricket::setLabelExtra20(int points)
{
    ui->label_extra20->setNum(points);
}

void groupbox_cricket::setLabelExtra25(int points)
{
    ui->label_extra25->setNum(points);
}

void groupbox_cricket::setLabel15(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_15slot1->clear();
        ui->label_15slot2->clear();
        ui->label_15slot3->clear();
        break;
    case 1:
        ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_15slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_15slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_15slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel16(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_16slot1->clear();
        ui->label_16slot2->clear();
        ui->label_16slot3->clear();
        break;
    case 1:
        ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_16slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_16slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_16slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel17(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_17slot1->clear();
        ui->label_17slot2->clear();
        ui->label_17slot3->clear();
        break;
    case 1:
        ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_17slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_17slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_17slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel18(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_18slot1->clear();
        ui->label_18slot2->clear();
        ui->label_18slot3->clear();
        break;
    case 1:
        ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_18slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_18slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_18slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel19(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_19slot1->clear();
        ui->label_19slot2->clear();
        ui->label_19slot3->clear();
        break;
    case 1:
        ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_19slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_19slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_19slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel20(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_20slot1->clear();
        ui->label_20slot2->clear();
        ui->label_20slot3->clear();
        break;
    case 1:
        ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_20slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_20slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_20slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

void groupbox_cricket::setLabel25(int hits)
{
    int w = 25;
    int h = 25;
    switch (hits)
    {
    default:
        ui->label_25slot1->clear();
        ui->label_25slot2->clear();
        ui->label_25slot3->clear();
        break;
    case 1:
        ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 2:
        ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_25slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        break;
    case 3:
        ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_25slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->label_25slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    }
}

int groupbox_cricket::getScore()
{
    return Player->getScore();
}

void groupbox_cricket::setLegHistory()
{
    QVector<QVector<QString>> legscores = Player->getMScoreLegs();
    QVector<QVector<QVector<QString>>> totalscores = Player->getmScoringHistory();
    if (legscores.size() == 0 && totalscores.size() > 0)
    {
        legscores = totalscores.last();
    }
    if (CutThroat)
    {
        QVector<QVector<QString>> temp = {};
        for (int i = 0; i < legscores.size(); i++)
        {
            if (legscores[i].last() != "")
            {
                temp.push_back(legscores[i]);
            }
        }
        legscores = {};
        legscores = temp;
    }
    ui->textBrowser->clear();
    for (int i = 0; i < legscores.size(); i++)
    {
        for (int j = 0; j < legscores[i].size(); j++)
        {
            QString temp = legscores[i][j];
            if (temp.size() > 1)
            {
                if (temp[1] == '0')
                {
                    legscores[i][j] = " X";
                }
                else
                {
                    legscores[i][j][0] = legscores[i][j][0].toUpper();
                }
            }
        }
        if (legscores[i].size() == 3)
        {
            QString line = QString::number(i+1) + ": " + legscores[i][0] + "  " + legscores[i][1] + "  " + legscores[i][2];
            ui->textBrowser->append(line);
        }
        else if (legscores[i].size() == 2)
        {
            QString line = QString::number(i+1) + ": " + legscores[i][0] + "  " + legscores[i][1];
            ui->textBrowser->append(line);
        }
        else
        {
            QString line = QString::number(i+1) + ": " + legscores[i][0];
            ui->textBrowser->append(line);
        }
    }
}

void groupbox_cricket::setExtra17(int points)
{
    Player->setExtra17(points);
}

void groupbox_cricket::setExtra18(int points)
{
    Player->setExtra18(points);
}

void groupbox_cricket::setExtra19(int points)
{
    Player->setExtra19(points);
}

void groupbox_cricket::setExtra20(int points)
{
    Player->setExtra20(points);
}

void groupbox_cricket::on_pushButton_undo_clicked()
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

void groupbox_cricket::performUndo()
{
    Player->undo();
    Score = Player->getScore();
    ui->lcdNumber->display(Score);
    mTotalHits = Player->getTotalHits();
    setLabel15(getSlot15());
    setLabel16(getSlot16());
    setLabel17(getSlot17());
    setLabel18(getSlot18());
    setLabel19(getSlot19());
    setLabel20(getSlot20());
    setLabel25(getSlot25());
    setLabelExtra15(getExtra15());
    setLabelExtra16(getExtra16());
    setLabelExtra17(getExtra17());
    setLabelExtra18(getExtra18());
    setLabelExtra19(getExtra19());
    setLabelExtra20(getExtra20());
    setLabelExtra25(getExtra25());
    ui->lcdNumber_legs->display(Player->get_legs());
    ui->lcdNumber_sets->display(Player->get_sets());
    QString hpr = QString::number(Player->get_hits_per_round(),'f',3);
    ui->label_hitsPerRound->setText(hpr);
    setLegHistory();
    if (Finished)
    {
        unsetFinished();
    }
}

void groupbox_cricket::signalSetLabelSlot(int hits, int slot)
{
    int w = 25;
    int h = 25;
    switch (slot)
    {
    case 15:
        switch (hits)
        {
        default:
            ui->label_15slot1->clear();
            ui->label_15slot2->clear();
            ui->label_15slot3->clear();
            break;
        case 1:
            ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_15slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_15slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_15slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_15slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 16:
        switch (hits)
        {
        default:
            ui->label_16slot1->clear();
            ui->label_16slot2->clear();
            ui->label_16slot3->clear();
            break;
        case 1:
            ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_16slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_16slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_16slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_16slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 17:
        switch (hits)
        {
        default:
            ui->label_17slot1->clear();
            ui->label_17slot2->clear();
            ui->label_17slot3->clear();
            break;
        case 1:
            ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_17slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_17slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_17slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_17slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 18:
        switch (hits)
        {
        default:
            ui->label_18slot1->clear();
            ui->label_18slot2->clear();
            ui->label_18slot3->clear();
            break;
        case 1:
            ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_18slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_18slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_18slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_18slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 19:
        switch (hits)
        {
        default:
            ui->label_19slot1->clear();
            ui->label_19slot2->clear();
            ui->label_19slot3->clear();
            break;
        case 1:
            ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_19slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_19slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_19slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_19slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 20:
        switch (hits)
        {
        default:
            ui->label_20slot1->clear();
            ui->label_20slot2->clear();
            ui->label_20slot3->clear();
            break;
        case 1:
            ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_20slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_20slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_20slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_20slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
        break;
    case 25:
        switch (hits)
        {
        default:
            ui->label_25slot1->clear();
            ui->label_25slot2->clear();
            ui->label_25slot3->clear();
            break;
        case 1:
            ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 2:
            ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_25slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            break;
        case 3:
            ui->label_25slot1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_25slot2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
            ui->label_25slot3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }
    default:;
    }
}

void groupbox_cricket::increaseExtra15(int points)
{
    Player->setExtra15(points);
}

void groupbox_cricket::increaseExtra16(int points)
{
    Player->setExtra16(points);
}

void groupbox_cricket::increaseExtra17(int points)
{
    Player->setExtra17(points);
}

void groupbox_cricket::increaseExtra18(int points)
{
    Player->setExtra18(points);
}

void groupbox_cricket::increaseExtra19(int points)
{
    Player->setExtra19(points);
}

void groupbox_cricket::increaseExtra20(int points)
{
    Player->setExtra20(points);
}

void groupbox_cricket::increaseExtra25(int points)
{
    Player->setExtra25(points);
}

void groupbox_cricket::setScore()
{
    Player->setScore();
}

void groupbox_cricket::updateLabels()
{
    setLabelExtra15(Player->getExtra15());
    setLabelExtra16(Player->getExtra16());
    setLabelExtra17(Player->getExtra17());
    setLabelExtra18(Player->getExtra18());
    setLabelExtra19(Player->getExtra19());
    setLabelExtra20(Player->getExtra20());
    setLabelExtra25(Player->getExtra25());
    ui->lcdNumber->display(Player->getScore());
}

void groupbox_cricket::updateDarts(QVector<QString> darts)
{
    Player->update_darts(darts);
    Player->setSlot15(Player->getSlot15());
    Player->setSlot16(Player->getSlot16());
    Player->setSlot17(Player->getSlot17());
    Player->setSlot18(Player->getSlot18());
    Player->setSlot19(Player->getSlot19());
    Player->setSlot20(Player->getSlot20());
    Player->setSlot25(Player->getSlot25());
    Player->setTotalHits(Player->getTotalHits());
    Player->setTotalDarts(Player->getTotalDarts());
    Player->setLegWinArray(false);
}

void groupbox_cricket::playFailSounds()
{
    std::srand(static_cast<unsigned> (std::time(0)));
    int rnd = 1 + (std::rand() % 12);
    switch (rnd)
    {
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

void groupbox_cricket::setLcdLegs()
{
    ui->lcdNumber_legs->display(Player->get_legs());
}

bool groupbox_cricket::legstarted = false;
bool groupbox_cricket::setstarted = false;
