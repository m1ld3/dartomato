#include "cricketmainwindow.h"
#include "ui_cricketmainwindow.h"
#include "groupbox_cricket.h"
#include "cricketclass.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>

CricketMainWindow::CricketMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CricketMainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

CricketMainWindow::CricketMainWindow(QWidget *parent, int numberofplayers, int sets, int legs, bool cutthroat, bool offensive)
    : QMainWindow(parent), ui(new Ui::CricketMainWindow), NumberOfPlayers(numberofplayers), Sets(sets), Legs(legs)
    , CutThroat(cutthroat), Offensive(offensive), mcricketplayer(NumberOfPlayers, nullptr)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Cricket");
    for (int i = 0;i < NumberOfPlayers; i++)
    {
        mcricketplayer[i] = new cricketclass(this, sets,legs, i+1, CutThroat);
        cricketbox.push_back(new groupbox_cricket(this, i+1, Sets, Legs, mcricketplayer[i], CutThroat, Offensive));
        cricketbox[i]->setAttribute(Qt::WA_DeleteOnClose);
        cricketbox[i]->setInactive();
        ui->gridLayoutCricket->addWidget(cricketbox[i],i<4 ? 0 : 1,i%4);
        connect(cricketbox[i],SIGNAL(signalUpdatePlayer(QString)),this,SLOT(signalUpdatePlayer(QString)));
        connect(cricketbox[i],SIGNAL(signalResetScores()),this,SLOT(signalResetScores()));
        connect(mcricketplayer[i],SIGNAL(signal_game_won(int)),this,SLOT(signalGameWon(int)));
        connect(cricketbox[i],SIGNAL(signalInactivatePlayers(int, bool, bool)),this,SLOT(signalInactivatePlayers(int, bool, bool)));
        connect(cricketbox[i],SIGNAL(signalUpdateHistory()),this,SLOT(signalUpdateHistory()));
    }
    ActivePlayer = 0;
    cricketbox[ActivePlayer]->setSetBegin();
    cricketbox[ActivePlayer]->setLegBegin();
    cricketbox[ActivePlayer]->setActive();
}

CricketMainWindow::~CricketMainWindow()
{
    delete ui;
    for (auto player : mcricketplayer)
    {
        delete player;
    }
    for (auto box : cricketbox)
    {
        delete box;
    }
}

void CricketMainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Quit game",
                                                                tr("Are you sure you want to quit the game?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void CricketMainWindow::setActivePlayer(int player)
{
    ActivePlayer = player;
}

void CricketMainWindow::updatePlayer()
{
    ActivePlayer = (ActivePlayer + 1) % NumberOfPlayers;
}

void CricketMainWindow::signalUpdatePlayer(QString type)
{
    if (type == "default")
    {
        updatePlayer();
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            cricketbox[i]->setInactive();
        }
        cricketbox[ActivePlayer]->setActive();
    }
    else if (type == "leg")
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            if (cricketbox[i]->hasBegunLeg())
            {
                cricketbox[i]->unsetLegBegin();
                ActivePlayer = i;
                updatePlayer();
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    cricketbox[i]->setInactive();
                }
                cricketbox[ActivePlayer]->setActive();
                cricketbox[ActivePlayer]->setLegBegin();
                break;
            }
        }
    }
    else if (type == "set")
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            if (cricketbox[i]->hasBegunSet())
            {
                cricketbox[i]->unsetSetBegin();
                ActivePlayer = i;
                updatePlayer();
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    cricketbox[i]->setInactive();
                    cricketbox[i]->unsetLegBegin();
                }
                cricketbox[ActivePlayer]->setActive();
                cricketbox[ActivePlayer]->setLegBegin();
                cricketbox[ActivePlayer]->setSetBegin();
                break;
            }
        }
        for (auto box: cricketbox)
        {
            box->resetLegs();
            box->setLcdLegs();
        }
    }
}

void CricketMainWindow::signalResetScores()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->reset();
    }
}

void CricketMainWindow::signalGameWon(int playernumber)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->setFinished();
    }
    cricketbox[playernumber]->closeCricketInput();
    QString name = cricketbox[playernumber]->getPlayerName();
    QString text = name + " has won the game. Congratulations!. ";
    QMessageBox::about(this,"Game finished", text);
}

void CricketMainWindow::signalInactivatePlayers(int player, bool legstarted, bool setstarted)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->setInactive();
    }
    setActivePlayer(player);
    if (!legstarted)
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            cricketbox[i]->unsetLegBegin();
        }
        cricketbox[player]->setLegBegin();
    }
    if (!setstarted)
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            cricketbox[i]->unsetSetBegin();
            cricketbox[i]->unsetLegBegin();
        }
        cricketbox[player]->setLegBegin();
        cricketbox[player]->setSetBegin();
    }
}

void CricketMainWindow::signalUpdateHistory()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->updateHistory();
    }
}

bool CricketMainWindow::isSlot15free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot15() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot16free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot16() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot17free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot17() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot18free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot18() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot19free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot19() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot20free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot20() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isSlot25free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            free |= cricketbox[i]->getSlot25() < 3;
        }
    }
    return free;
}

bool CricketMainWindow::isScoreBigger(int score)
{
    bool result = true;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        result = result && cricketbox[i]->getScore() <= score;
    }
    return result;
}

bool CricketMainWindow::isScoreSmaller(int score)
{
    bool result = true;
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        result = result && cricketbox[i]->getScore() >= score;
    }
    return result;
}

void CricketMainWindow::increaseScore15(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot15() != 3)
        {
            cricketbox[i]->increaseExtra15(points);
        }
        else
        {
            cricketbox[i]->increaseExtra15(0);
        }
    }
}

void CricketMainWindow::increaseScore16(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot16() != 3)
        {
            cricketbox[i]->increaseExtra16(points);
        }
        else
        {
            cricketbox[i]->increaseExtra16(0);
        }
    }
}

void CricketMainWindow::increaseScore17(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot17() != 3)
        {
            cricketbox[i]->increaseExtra17(points);
        }
        else
        {
            cricketbox[i]->increaseExtra17(0);
        }
    }
}

void CricketMainWindow::increaseScore18(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot18() != 3)
        {
            cricketbox[i]->increaseExtra18(points);
        }
        else
        {
            cricketbox[i]->increaseExtra18(0);
        }
    }
}

void CricketMainWindow::increaseScore19(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot19() != 3)
        {
            cricketbox[i]->increaseExtra19(points);
        }
        else
        {
            cricketbox[i]->increaseExtra19(0);
        }
    }
}

void CricketMainWindow::increaseScore20(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot20() != 3)
        {
            cricketbox[i]->increaseExtra20(points);
        }
        else
        {
            cricketbox[i]->increaseExtra20(0);
        }
    }
}

void CricketMainWindow::increaseScore25(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (cricketbox[i]->getSlot25() != 3)
        {
            cricketbox[i]->increaseExtra25(points);
        }
        else
        {
            cricketbox[i]->increaseExtra25(0);
        }
    }
}

QVector<int> CricketMainWindow::computeExtra15s(int points, int player)
{
    QVector<int> extra15s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot15() != 3)
            {
                extra = cricketbox[i]->getExtra15() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra15();
            }
            extra15s.push_back(extra);
        }
    }
    return extra15s;
}

QVector<int> CricketMainWindow::computeExtra16s(int points, int player)
{
    QVector<int> extra16s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot16() != 3)
            {
                extra = cricketbox[i]->getExtra16() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra16();
            }
            extra16s.push_back(extra);
        }
    }
    return extra16s;
}

QVector<int> CricketMainWindow::computeExtra17s(int points, int player)
{
    QVector<int> extra17s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot17() != 3)
            {
                extra = cricketbox[i]->getExtra17() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra17();
            }
            extra17s.push_back(extra);
        }
    }
    return extra17s;
}

QVector<int> CricketMainWindow::computeExtra18s(int points, int player)
{
    QVector<int> extra18s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot18() != 3)
            {
                extra = cricketbox[i]->getExtra18() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra18();
            }
            extra18s.push_back(extra);
        }
    }
    return extra18s;
}

QVector<int> CricketMainWindow::computeExtra19s(int points, int player)
{
    QVector<int> extra19s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot19() != 3)
            {
                extra = cricketbox[i]->getExtra19() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra19();
            }
            extra19s.push_back(extra);
        }
    }
    return extra19s;
}

QVector<int> CricketMainWindow::computeExtra20s(int points, int player)
{
    QVector<int> extra20s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot20() != 3)
            {
                extra = cricketbox[i]->getExtra20() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra20();
            }
            extra20s.push_back(extra);
        }
    }
    return extra20s;
}

QVector<int> CricketMainWindow::computeExtra25s(int points, int player)
{
    QVector<int> extra25s = {};
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            int extra = 0;
            if (cricketbox[i]->getSlot25() != 3)
            {
                extra = cricketbox[i]->getExtra25() + points;
            }
            else
            {
                extra = cricketbox[i]->getExtra25();
            }
            extra25s.push_back(extra);
        }
    }
    return extra25s;
}

void CricketMainWindow::setScores()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->setScore();
    }
}

void CricketMainWindow::updateLabels()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        cricketbox[i]->updateLabels();
    }
}

void CricketMainWindow::updateDarts(int player)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (i != player)
        {
            cricketbox[i]->updateDarts({""});
        }
    }
}
