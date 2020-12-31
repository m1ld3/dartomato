#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "groupbox_player.h"
#include "groupbox_cricket.h"
#include "playerclass.h"
#include "cricketclass.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

GameWindow::GameWindow(QWidget *parent, int numberofplayers, int game, int sets, int legs, bool singleIn,
                       bool singleOut, bool doubleIn, bool doubleOut, bool masterIn, bool masterOut, bool cutthroat, bool offensive) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    NumberOfPlayers(numberofplayers), Game(game), Sets(sets), Legs(legs), SingleIn(singleIn),
    SingleOut(singleOut), DoubleIn(doubleIn), DoubleOut(doubleOut),
    MasterIn(masterIn), MasterOut(masterOut), CutThroat(cutthroat), Offensive(offensive),
    mglayout(nullptr), mplayer(NumberOfPlayers, nullptr), mcricketplayer(NumberOfPlayers, nullptr), mDartBoard(nullptr)
{
    ui->setupUi(this);
    QString text = QString::number(game);

    QWidget::setWindowTitle(game != 0 ? text : "Cricket");
    if (game > 0)
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            mplayer[i] = new PlayerClass(game,sets,legs, i+1);
            playerbox.push_back(new GroupBox_player(this, i+1, game, Sets, Legs, SingleIn,
                                               SingleOut, DoubleIn, DoubleOut, MasterIn,
                                               MasterOut, Offensive, mplayer[i]));
            playerbox[i]->setAttribute(Qt::WA_DeleteOnClose);
            playerbox[i]->setInactive();
            if ((i % 2) == 0)
            {
                ui->gridForLeftHandPlayers->addWidget(playerbox[i]);
            }
            else
            {
                ui->gridForRightHandPlayers->addWidget(playerbox[i]);
            }
            connect(playerbox[i],SIGNAL(signalUpdatePlayer(QString)),this,SLOT(signalUpdatePlayer(QString)));
            connect(playerbox[i],SIGNAL(signalResetScores()),this,SLOT(signalResetScores()));
            connect(mplayer[i],SIGNAL(signalGameWon(int)),this,SLOT(signalGameWon(int)));
            connect(playerbox[i],SIGNAL(signalInactivatePlayers(int, bool, bool)),this,SLOT(signalInactivatePlayers(int, bool, bool)));
            connect(playerbox[i],SIGNAL(signalUpdateHistory()),this,SLOT(signalUpdateHistory()));
        }
        ActivePlayer = 0;
        playerbox[ActivePlayer]->setSetBegin();
        playerbox[ActivePlayer]->setLegBegin();
        playerbox[ActivePlayer]->setActive();
        mDartBoard = new DartBoard(ui->graphicsViewDartBoard, game, game, singleIn, SingleOut, doubleIn, doubleOut, masterIn, masterOut);
        mDartBoard->initDartBoard(game);
        connect(mDartBoard, SIGNAL (signalSubmitButtonPressed2GameWindow(int&, int&, int&, QVector<QString>)), this, SLOT (submitButtonPressedSlot(int&, int&, int&, QVector<QString>)));
    } else
    {
        mglayout = new QGridLayout;
        ui->centralwidget->setLayout(mglayout);
        for (int i = 0;i < NumberOfPlayers; i++)
        {
            mcricketplayer[i] = new cricketclass(this, sets,legs, i+1, CutThroat);
            cricketbox.push_back(new groupbox_cricket(this, i+1, Sets, Legs, mcricketplayer[i], CutThroat, Offensive));
            cricketbox[i]->setAttribute(Qt::WA_DeleteOnClose);
            cricketbox[i]->setInactive();
            mglayout->addWidget(cricketbox[i],i<4 ? 0 : 1,i%4);
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
}

GameWindow::~GameWindow()
{
    delete ui;
    if (mglayout) delete mglayout;
    if (mDartBoard) delete mDartBoard;
    for (auto player : mplayer)
    {
        delete player;
    }
    for (auto player : mcricketplayer)
    {
        delete player;
    }
    for (auto box : playerbox)
    {
        delete box;
    }
    for (auto box : cricketbox)
    {
        delete box;
    }
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Quit game",
                                                                tr("Are you sure you want to quit the game?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void GameWindow::setActivePlayer(int player)
{
    ActivePlayer = player;
}

void GameWindow::updatePlayer()
{
    ActivePlayer = (ActivePlayer + 1) % NumberOfPlayers;
}

void GameWindow::signalUpdatePlayer(QString type)
{
    if (playerbox.size() > 0) {
        if (type == "default") {
            updatePlayer();
            for (int i = 0; i < NumberOfPlayers; i++) {
                playerbox[i]->setInactive();
            }
            playerbox[ActivePlayer]->setActive();
        } else if (type == "leg") {
            for (int i = 0; i < NumberOfPlayers; i++) {
                if (playerbox[i]->hasBegunLeg()) {
                    playerbox[i]->unsetLegBegin();
                    ActivePlayer = i;
                    updatePlayer();
                    for (int i = 0; i < NumberOfPlayers; i++) {
                        playerbox[i]->setInactive();
                    }
                    playerbox[ActivePlayer]->setActive();
                    playerbox[ActivePlayer]->setLegBegin();
                    break;
                }
            }
        } else if (type == "set") {
            for (int i = 0; i < NumberOfPlayers; i++) {
                if (playerbox[i]->hasBegunSet()) {
                    playerbox[i]->unsetSetBegin();
                    ActivePlayer = i;
                    updatePlayer();
                    for (int i = 0; i < NumberOfPlayers; i++) {
                        playerbox[i]->setInactive();
                        playerbox[i]->unsetLegBegin();
                    }
                    playerbox[ActivePlayer]->setActive();
                    playerbox[ActivePlayer]->setSetBegin();
                    playerbox[ActivePlayer]->setLegBegin();
                    break;
                }
            }
            for (auto box: playerbox)
            {
                box->resetLegs();
                box->setLcdLegs();
            }
        }
        mDartBoard->initDartBoard(playerbox[ActivePlayer]->getRemaining());
    } else if (cricketbox.size() > 0) {
        if (type == "default") {
            updatePlayer();
            for (int i = 0; i < NumberOfPlayers; i++) {
                cricketbox[i]->setInactive();
            }
            cricketbox[ActivePlayer]->setActive();
        } else if (type == "leg") {
            for (int i = 0; i < NumberOfPlayers; i++) {
                if (cricketbox[i]->hasBegunLeg()) {
                    cricketbox[i]->unsetLegBegin();
                    ActivePlayer = i;
                    updatePlayer();
                    for (int i = 0; i < NumberOfPlayers; i++) {
                        cricketbox[i]->setInactive();
                    }
                    cricketbox[ActivePlayer]->setActive();
                    cricketbox[ActivePlayer]->setLegBegin();
                    break;
                }
            }
        } else if (type == "set") {
            for (int i = 0; i < NumberOfPlayers; i++) {
                if (cricketbox[i]->hasBegunSet()) {
                    cricketbox[i]->unsetSetBegin();
                    ActivePlayer = i;
                    updatePlayer();
                    for (int i = 0; i < NumberOfPlayers; i++) {
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
}

void GameWindow::signalResetScores()
{
    if (playerbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            playerbox[i]->reset();
            playerbox[i]->displayFinishes();
        }
    } else if (cricketbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            cricketbox[i]->reset();
        }
    }

}

void GameWindow::signalGameWon(int playernumber)
{
    if (playerbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            playerbox[i]->setFinished();
        }
        QString name = playerbox[playernumber]->getPlayerName();
        QString text = name + " has won the game. Congratulations!. ";
        QMessageBox::about(this,"Game finished", text);
    } else if (cricketbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            cricketbox[i]->setFinished();
        }
        cricketbox[playernumber]->closeCricketInput();
        QString name = cricketbox[playernumber]->getPlayerName();
        QString text = name + " has won the game. Congratulations!. ";
        QMessageBox::about(this,"Game finished", text);
    }
}

void GameWindow::signalInactivatePlayers(int player, bool legstarted, bool setstarted)
{
    if (playerbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            playerbox[i]->setInactive();
        }
        setActivePlayer(player);
        if (!legstarted) {
            for (int i = 0; i < NumberOfPlayers; i++) {
                playerbox[i]->unsetLegBegin();
            }
            playerbox[player]->setLegBegin();
        }
        if (!setstarted) {
            for (int i = 0; i < NumberOfPlayers; i++) {
                playerbox[i]->unsetSetBegin();
                playerbox[i]->unsetLegBegin();
            }
            playerbox[player]->setLegBegin();
            playerbox[player]->setSetBegin();
        }
    } else if (cricketbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            cricketbox[i]->setInactive();
        }
        setActivePlayer(player);
        if (!legstarted) {
            for (int i = 0; i < NumberOfPlayers; i++) {
                cricketbox[i]->unsetLegBegin();
            }
            cricketbox[player]->setLegBegin();
        }
        if (!setstarted) {
            for (int i = 0; i < NumberOfPlayers; i++) {
                cricketbox[i]->unsetSetBegin();
                cricketbox[i]->unsetLegBegin();
            }
            cricketbox[player]->setLegBegin();
            cricketbox[player]->setSetBegin();
        }
    }
}

void GameWindow::signalUpdateHistory()
{
    if (playerbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            playerbox[i]->updateHistory();
        }
    } else if (cricketbox.size() > 0) {
        for (int i = 0; i < NumberOfPlayers; i++) {
            cricketbox[i]->updateHistory();
        }
    }

}

void GameWindow::submitButtonPressedSlot(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts)
{
    playerbox[ActivePlayer]->submitScore(score, numberofdarts, checkoutattempts, darts);
}

bool GameWindow::isSlot15free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot15() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot16free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot16() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot17free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot17() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot18free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot18() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot19free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot19() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot20free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot20() < 3;
        }
    }
    return free;
}

bool GameWindow::isSlot25free(int player)
{
    bool free = false;
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            free |= cricketbox[i]->getSlot25() < 3;
        }
    }
    return free;
}

bool GameWindow::isScoreBigger(int score)
{
    bool result = true;
    for (int i = 0; i < NumberOfPlayers; i++) {
        result = result && cricketbox[i]->getScore() <= score;
    }
    return result;
}

bool GameWindow::isScoreSmaller(int score)
{
    bool result = true;
    for (int i = 0; i < NumberOfPlayers; i++) {
        result = result && cricketbox[i]->getScore() >= score;
    }
    return result;
}

void GameWindow::increaseScore15(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot15() != 3) {
            cricketbox[i]->increaseExtra15(points);
        } else {
            cricketbox[i]->increaseExtra15(0);
        }
    }
}

void GameWindow::increaseScore16(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot16() != 3) {
            cricketbox[i]->increaseExtra16(points);
        } else {
            cricketbox[i]->increaseExtra16(0);
        }
    }
}

void GameWindow::increaseScore17(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot17() != 3) {
            cricketbox[i]->increaseExtra17(points);
        } else {
            cricketbox[i]->increaseExtra17(0);
        }
    }
}

void GameWindow::increaseScore18(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot18() != 3) {
            cricketbox[i]->increaseExtra18(points);
        } else {
            cricketbox[i]->increaseExtra18(0);
        }
    }
}

void GameWindow::increaseScore19(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot19() != 3) {
            cricketbox[i]->increaseExtra19(points);
        } else {
            cricketbox[i]->increaseExtra19(0);
        }
    }
}

void GameWindow::increaseScore20(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot20() != 3) {
            cricketbox[i]->increaseExtra20(points);
        } else {
            cricketbox[i]->increaseExtra20(0);
        }
    }
}

void GameWindow::increaseScore25(int points)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (cricketbox[i]->getSlot25() != 3) {
            cricketbox[i]->increaseExtra25(points);
        } else {
            cricketbox[i]->increaseExtra25(0);
        }
    }
}

QVector<int> GameWindow::computeExtra15s(int points, int player)
{
    QVector<int> extra15s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot15() != 3) {
                extra = cricketbox[i]->getExtra15() + points;
            } else {
                extra = cricketbox[i]->getExtra15();
            }
            extra15s.push_back(extra);
        }
    }
    return extra15s;
}

QVector<int> GameWindow::computeExtra16s(int points, int player)
{
    QVector<int> extra16s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot16() != 3) {
                extra = cricketbox[i]->getExtra16() + points;
            } else {
                extra = cricketbox[i]->getExtra16();
            }
            extra16s.push_back(extra);
        }
    }
    return extra16s;
}

QVector<int> GameWindow::computeExtra17s(int points, int player)
{
    QVector<int> extra17s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot17() != 3) {
                extra = cricketbox[i]->getExtra17() + points;
            } else {
                extra = cricketbox[i]->getExtra17();
            }
            extra17s.push_back(extra);
        }
    }
    return extra17s;
}

QVector<int> GameWindow::computeExtra18s(int points, int player)
{
    QVector<int> extra18s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot18() != 3) {
                extra = cricketbox[i]->getExtra18() + points;
            } else {
                extra = cricketbox[i]->getExtra18();
            }
            extra18s.push_back(extra);
        }
    }
    return extra18s;
}

QVector<int> GameWindow::computeExtra19s(int points, int player)
{
    QVector<int> extra19s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot19() != 3) {
                extra = cricketbox[i]->getExtra19() + points;
            } else {
                extra = cricketbox[i]->getExtra19();
            }
            extra19s.push_back(extra);
        }
    }
    return extra19s;
}

QVector<int> GameWindow::computeExtra20s(int points, int player)
{
    QVector<int> extra20s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot20() != 3) {
                extra = cricketbox[i]->getExtra20() + points;
            } else {
                extra = cricketbox[i]->getExtra20();
            }
            extra20s.push_back(extra);
        }
    }
    return extra20s;
}

QVector<int> GameWindow::computeExtra25s(int points, int player)
{
    QVector<int> extra25s = {};
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            int extra = 0;
            if (cricketbox[i]->getSlot25() != 3) {
                extra = cricketbox[i]->getExtra25() + points;
            } else {
                extra = cricketbox[i]->getExtra25();
            }
            extra25s.push_back(extra);
        }
    }
    return extra25s;
}

void GameWindow::setScores()
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        cricketbox[i]->setScore();
    }
}

void GameWindow::updateLabels()
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        cricketbox[i]->updateLabels();
    }
}

void GameWindow::updateDarts(int player)
{
    for (int i = 0; i < NumberOfPlayers; i++) {
        if (i != player) {
            cricketbox[i]->updateDarts({""});
        }
    }
}
