#include "xo1mainwindow.h"
#include "ui_xo1mainwindow.h"
#include "groupbox_player.h"
#include "playerclass.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

X01MainWindow::X01MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::X01MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

X01MainWindow::X01MainWindow(QWidget *parent, int numberofplayers, int game, int sets, int legs, bool singleIn,
                             bool singleOut, bool doubleIn, bool doubleOut, bool masterIn, bool masterOut, bool offensive)
    : QMainWindow(parent), ui(new Ui::X01MainWindow), NumberOfPlayers(numberofplayers), Game(game), Sets(sets), Legs(legs)
    , SingleIn(singleIn), SingleOut(singleOut), DoubleIn(doubleIn), DoubleOut(doubleOut), MasterIn(masterIn)
    , MasterOut(masterOut), Offensive(offensive), mplayer(NumberOfPlayers, nullptr), mDartBoard(nullptr)
{
    ui->setupUi(this);
    QString text = QString::number(game);
    QWidget::setWindowTitle(text);
    mDartBoard = new DartBoard(ui->graphicsViewDartBoard, game, game, singleIn, singleOut, doubleIn, doubleOut, masterIn, masterOut);
    mDartBoard->initDartBoard(game);

    connect(mDartBoard, SIGNAL (signalSubmitButtonPressed2GameWindow(int&, int&, int&, QVector<QString>)), this, SLOT (submitButtonPressedSlot(int&, int&, int&, QVector<QString>)));
    connect(mDartBoard, SIGNAL (signalDisplayScore(int)), this, SLOT (displayScoreSlot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart1(int)), this, SLOT (displayDart1Slot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart2(int)), this, SLOT (displayDart2Slot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart3(int)), this, SLOT (displayDart3Slot(int)));
    connect(mDartBoard, SIGNAL (signalEraseDart1()), this, SLOT (eraseDart1Slot()));
    connect(mDartBoard, SIGNAL (signalEraseDart2()), this, SLOT (eraseDart2Slot()));
    connect(mDartBoard, SIGNAL (signalEraseDart3()), this, SLOT (eraseDart3Slot()));
    connect(mDartBoard, SIGNAL (signalUpdateFinishes(int, int)), this, SLOT (updateFinishesSlot(int, int)));
    connect(mDartBoard, SIGNAL (signalSetFocusToSubmitButton()), this, SLOT (setFocusToSubmitButtonSlot()));

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        mplayer[i] = new PlayerClass(game,sets,legs, i+1);
        playerbox.push_back(new GroupBox_player(this, i+1, game, Sets, Legs, SingleIn,
                                                SingleOut, DoubleIn, DoubleOut, MasterIn,
                                                MasterOut, Offensive, mplayer[i], mDartBoard));
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
    displayScoreSlot(playerbox[ActivePlayer]->getRemaining());
    eraseDart1Slot();
    eraseDart2Slot();
    eraseDart3Slot();
    ui->submitButton->setAutoDefault(true);
}

X01MainWindow::~X01MainWindow()
{
    delete ui;
    if (mDartBoard) delete mDartBoard;
    for (auto &player : mplayer)
    {
        delete player;
    }
    for (auto &box : playerbox)
    {
        delete box;
    }
}

void X01MainWindow::closeEvent(QCloseEvent *event)
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

void X01MainWindow::setActivePlayer(int player)
{
    ActivePlayer = player;
}

void X01MainWindow::updatePlayer()
{
    ActivePlayer = (ActivePlayer + 1) % NumberOfPlayers;
}

void X01MainWindow::signalUpdatePlayer(QString type)
{
    if (type == "default")
    {
        updatePlayer();
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            playerbox[i]->setInactive();
        }
        playerbox[ActivePlayer]->setActive();
    }
    else if (type == "leg")
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            if (playerbox[i]->hasBegunLeg())
            {
                playerbox[i]->unsetLegBegin();
                ActivePlayer = i;
                updatePlayer();
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    playerbox[i]->setInactive();
                }
                playerbox[ActivePlayer]->setActive();
                playerbox[ActivePlayer]->setLegBegin();
                break;
            }
        }
    }
    else if (type == "set")
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            if (playerbox[i]->hasBegunSet())
            {
                playerbox[i]->unsetSetBegin();
                ActivePlayer = i;
                updatePlayer();
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    playerbox[i]->setInactive();
                    playerbox[i]->unsetLegBegin();
                }
                playerbox[ActivePlayer]->setActive();
                playerbox[ActivePlayer]->setSetBegin();
                playerbox[ActivePlayer]->setLegBegin();
                break;
            }
        }
        for (auto &box: playerbox)
        {
            box->resetLegs();
            box->setLcdLegs();
        }
    }
    mDartBoard->initDartBoard(playerbox[ActivePlayer]->getRemaining());
}

void X01MainWindow::signalResetScores()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->reset();
        playerbox[i]->displayFinishes(playerbox[i]->getRemaining(), 3);
    }
}

void X01MainWindow::signalGameWon(int playernumber)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->setFinished();
    }
    mDartBoard->setFinished();
    QString name = playerbox[playernumber]->getPlayerName();
    QString text = name + " has won the game. Congratulations!. ";
    QMessageBox::about(this,"Game finished", text);
}

void X01MainWindow::signalInactivatePlayers(int player, bool legstarted, bool setstarted)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->setInactive();
    }
    setActivePlayer(player);
    if (!legstarted)
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            playerbox[i]->unsetLegBegin();
        }
        playerbox[player]->setLegBegin();
    }
    if (!setstarted)
    {
        for (int i = 0; i < NumberOfPlayers; i++)
        {
            playerbox[i]->unsetSetBegin();
            playerbox[i]->unsetLegBegin();
        }
        playerbox[player]->setLegBegin();
        playerbox[player]->setSetBegin();
    }
}

void X01MainWindow::signalUpdateHistory()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->updateHistory();
    }
}

void X01MainWindow::submitButtonPressedSlot(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts)
{
    playerbox[ActivePlayer]->submitScore(score, numberofdarts, checkoutattempts, darts);
}

void X01MainWindow::on_submitButton_clicked()
{
    mDartBoard->submitScore();
}

void X01MainWindow::on_undoButton_clicked()
{
    mDartBoard->performUndo();
}

void X01MainWindow::displayScoreSlot(int score)
{
    ui->currentRemainingLCD->display(score);
}

void X01MainWindow::displayDart1Slot(int val)
{
    ui->dart1LCD->display(val);
}

void X01MainWindow::displayDart2Slot(int val)
{
    ui->dart2LCD->display(val);
}

void X01MainWindow::displayDart3Slot(int val)
{
    ui->dart3LCD->display(val);
}

void X01MainWindow::eraseDart1Slot()
{
    ui->dart1LCD->display("--");
}

void X01MainWindow::eraseDart2Slot()
{
    ui->dart2LCD->display("--");
}

void X01MainWindow::eraseDart3Slot()
{
    ui->dart3LCD->display("--");
}

void X01MainWindow::updateFinishesSlot(int score, int numberOfDarts)
{
    playerbox[ActivePlayer]->displayFinishes(score, numberOfDarts);
}

void X01MainWindow::setFocusToSubmitButtonSlot()
{
    ui->submitButton->setFocus();
}
