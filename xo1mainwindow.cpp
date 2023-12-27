#include "xo1mainwindow.h"
#include "ui_xo1mainwindow.h"
#include "groupbox_player.h"
#include "playerclass.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

CX01MainWindow::CX01MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CX01MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

CX01MainWindow::CX01MainWindow(QWidget *parent, int numberofplayers, int game, int sets, int legs, bool singleIn,
                             bool singleOut, bool doubleIn, bool doubleOut, bool masterIn, bool masterOut, bool offensive)
    : QMainWindow(parent), ui(new Ui::CX01MainWindow), NumberOfPlayers(numberofplayers), Game(game), Sets(sets), Legs(legs)
    , SingleIn(singleIn), SingleOut(singleOut), DoubleIn(doubleIn), DoubleOut(doubleOut), MasterIn(masterIn)
    , MasterOut(masterOut), Offensive(offensive), mplayer(NumberOfPlayers, nullptr), mDartBoard(nullptr)
{
    ui->setupUi(this);
    QString text = QString::number(game);
    QWidget::setWindowTitle(text);
    mDartBoard = new CDartBoard(ui->graphicsViewDartBoard, game, game, singleIn, singleOut, doubleIn, doubleOut, masterIn, masterOut);
    mDartBoard->init_dartboard(game);

    connect(mDartBoard, SIGNAL (signalSubmitButtonPressed2GameWindow(int&, int&, int&, QVector<QString>)), this, SLOT (submit_button_pressed_slot(int&, int&, int&, QVector<QString>)));
    connect(mDartBoard, SIGNAL (signalDisplayScore(int)), this, SLOT (display_score_slot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart1(int)), this, SLOT (display_dart1_slot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart2(int)), this, SLOT (displayDart2Slot(int)));
    connect(mDartBoard, SIGNAL (signalDisplayDart3(int)), this, SLOT (display_dart3_slot(int)));
    connect(mDartBoard, SIGNAL (signalEraseDart1()), this, SLOT (erase_dart1_slot()));
    connect(mDartBoard, SIGNAL (signalEraseDart2()), this, SLOT (erase_dart2_slot()));
    connect(mDartBoard, SIGNAL (signalEraseDart3()), this, SLOT (erase_dart3_slot()));
    connect(mDartBoard, SIGNAL (signalUpdateFinishes(int, int)), this, SLOT (update_finishes_slot(int, int)));
    connect(mDartBoard, SIGNAL (signalSetFocusToSubmitButton()), this, SLOT (set_focus_to_submit_button_slot()));

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        mplayer[i] = new CPlayerClass(game,sets,legs, i+1);
        playerbox.push_back(new CX01GroupBox(this, i+1, game, Sets, Legs, SingleIn,
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
        connect(playerbox[i],SIGNAL(signal_update_player(QString)),this,SLOT(signal_update_player(QString)));
        connect(playerbox[i],SIGNAL(signal_reset_scores()),this,SLOT(signal_reset_scores()));
        connect(mplayer[i],SIGNAL(signal_game_won(int)),this,SLOT(signal_game_won(int)));
        connect(playerbox[i],SIGNAL(signal_inactivate_players(int, bool, bool)),this,SLOT(signal_inactivate_players(int, bool, bool)));
        connect(playerbox[i],SIGNAL(signal_update_history()),this,SLOT(signal_update_history()));
    }

    ActivePlayer = 0;
    playerbox[ActivePlayer]->setSetBegin();
    playerbox[ActivePlayer]->setLegBegin();
    playerbox[ActivePlayer]->setActive();
    display_score_slot(playerbox[ActivePlayer]->getRemaining());
    erase_dart1_slot();
    erase_dart2_slot();
    erase_dart3_slot();
    ui->submitButton->setAutoDefault(true);
}

CX01MainWindow::~CX01MainWindow()
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

void CX01MainWindow::closeEvent(QCloseEvent *event)
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

void CX01MainWindow::set_active_player(int player)
{
    ActivePlayer = player;
}

void CX01MainWindow::update_player()
{
    ActivePlayer = (ActivePlayer + 1) % NumberOfPlayers;
}

void CX01MainWindow::signal_update_player(QString type)
{
    if (type == "default")
    {
        update_player();
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
                update_player();
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
                update_player();
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
    mDartBoard->init_dartboard(playerbox[ActivePlayer]->getRemaining());
}

void CX01MainWindow::signal_reset_scores()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->reset();
        playerbox[i]->displayFinishes(playerbox[i]->getRemaining(), 3);
    }
}

void CX01MainWindow::signal_game_won(int playernumber)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->setFinished();
    }
    mDartBoard->set_finished();
    QString name = playerbox[playernumber]->getPlayerName();
    QString text = name + " has won the game. Congratulations!. ";
    QMessageBox::about(this,"Game finished", text);
}

void CX01MainWindow::signal_inactivate_players(int player, bool legstarted, bool setstarted)
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->setInactive();
    }
    set_active_player(player);
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

void CX01MainWindow::signal_update_history()
{
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        playerbox[i]->updateHistory();
    }
}

void CX01MainWindow::submit_button_pressed_slot(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts)
{
    playerbox[ActivePlayer]->submit_score(score, numberofdarts, checkoutattempts, darts);
}

void CX01MainWindow::on_submit_button_clicked()
{
    mDartBoard->submit_score();
}

void CX01MainWindow::on_undo_button_clicked()
{
    mDartBoard->perform_undo();
}

void CX01MainWindow::display_score_slot(int score)
{
    ui->currentRemainingLCD->display(score);
}

void CX01MainWindow::display_dart1_slot(int val)
{
    ui->dart1LCD->display(val);
}

void CX01MainWindow::displayDart2Slot(int val)
{
    ui->dart2LCD->display(val);
}

void CX01MainWindow::display_dart3_slot(int val)
{
    ui->dart3LCD->display(val);
}

void CX01MainWindow::erase_dart1_slot()
{
    ui->dart1LCD->display("--");
}

void CX01MainWindow::erase_dart2_slot()
{
    ui->dart2LCD->display("--");
}

void CX01MainWindow::erase_dart3_slot()
{
    ui->dart3LCD->display("--");
}

void CX01MainWindow::update_finishes_slot(int score, int numberOfDarts)
{
    playerbox[ActivePlayer]->displayFinishes(score, numberOfDarts);
}

void CX01MainWindow::set_focus_to_submit_button_slot()
{
    ui->submitButton->setFocus();
}
