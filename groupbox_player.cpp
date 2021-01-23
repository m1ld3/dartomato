#include "groupbox_player.h"
#include "ui_groupbox_player.h"
#include "dialognameinput.h"
#include "QPixmap"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <QMessageBox>
#include <QString>
#include <algorithm>
#include <QBarSet>
#include <QtCharts>
#include <xo1mainwindow.h>
#include <checkoutlists.h>

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
    mGameWindow = dynamic_cast<X01MainWindow*>(parent);
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
    ui->textBrowser->clear();
    ui->textBrowser->setText("Checkouts:");
    if (SingleOut)
    {
        if (singleoutSingleDartCheckoutList.find(remaining) != singleoutSingleDartCheckoutList.end())
        {
            QVector<QString> checkouts = singleoutSingleDartCheckoutList.find(remaining).value();
            for (auto checkout : checkouts)
            {
                ui->textBrowser->append(checkout);
            }
        }
        if (numberOfDarts > 1)
        {
            if (singleoutTwoDartCheckoutList.find(remaining) != singleoutTwoDartCheckoutList.end())
            {
                QVector<QString> checkouts = singleoutTwoDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
                }
            }
        }
        if (numberOfDarts >2)
        {
            if (singleoutThreeDartCheckoutList.find(remaining) != singleoutThreeDartCheckoutList.end())
            {
                QVector<QString> checkouts = singleoutThreeDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
                }
            }
        }
    }
    else if (DoubleOut)
    {
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
    }
    else if (MasterOut)
    {
        if (masteroutSingleDartCheckoutList.find(remaining) != masteroutSingleDartCheckoutList.end())
        {
            QVector<QString> checkouts = masteroutSingleDartCheckoutList.find(remaining).value();
            for (auto checkout : checkouts)
            {
                ui->textBrowser->append(checkout);
            }
        }
        if (numberOfDarts > 1)
        {
            if (masteroutTwoDartCheckoutList.find(remaining) != masteroutTwoDartCheckoutList.end())
            {
                QVector<QString> checkouts = masteroutTwoDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
                }
            }
        }
        if (numberOfDarts >2)
        {
            if (masteroutThreeDartCheckoutList.find(remaining) != masteroutThreeDartCheckoutList.end())
            {
                QVector<QString> checkouts = masteroutThreeDartCheckoutList.find(remaining).value();
                for (auto checkout : checkouts)
                {
                    ui->textBrowser->append(checkout);
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
    if (Active)
    {
        mDartBoard->initDartBoard(Remaining);
    }
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

void GroupBox_player::slotUpdateLegHistory(int index, StatsWindow *stats)
{
    stats->clearText();
    stats->setLabelLeg1DartAvg(0.0);
    stats->setLabelLeg3DartAvg(0.0);
    QVector<int> legscores = Player->get_LegScores();
    QVector<QVector<int>> totalscores = Player->get_TotalScores();
    if (legscores.size()) totalscores.append(legscores);
    if (totalscores.size() >= index + 1)
    {
        for (int i = 0; i < totalscores.at(index).size(); i++)
        {
            QString line = QString::number(i+1) + ": " + QString::number(totalscores.at(index)[i]);
            stats->setText(line);
        }
        QVector<QVector<QString>> legdarts = Player->get_LegDarts();
        QVector<QVector<QVector<QString>>> totaldarts = Player->getThrownDartsOfAllLegs();
        if (legdarts.size()) totaldarts.append(legdarts);
        int numberOfDarts = (totaldarts.at(index).size() - 1) * 3 + totaldarts.at(index).back().size();
        double avg1dart = std::accumulate(totalscores.at(index).begin(), totalscores.at(index).end(), 0.0)/numberOfDarts;
        double avg3dart = 3 * avg1dart;
        stats->setLabelLeg1DartAvg(avg1dart);
        stats->setLabelLeg3DartAvg(avg3dart);
    }
}

double GroupBox_player::computeAverage(QVector<int> vector)
{
    double avg;
    double n = static_cast<double>(vector.size());
    if (n > 0) {
        avg = std::accumulate(vector.begin(), vector.end(), 0.0)/n;
    } else {
        avg = 0.0;
    }
    return avg;
}

void GroupBox_player::on_pushButton_stats_clicked()
{
    StatsWindow *stats = new StatsWindow;
    connect(stats,SIGNAL(signalUpdateLegHistory(int,StatsWindow*)),this,SLOT(slotUpdateLegHistory(int, StatsWindow*)));
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
    int the180s = 0, plus160 = 0, plus140 = 0, plus120 = 0, plus100 = 0, plus80 = 0, plus60 = 0, plus40 = 0, plus20 = 0;
    if (allScores.size() > 0)
    {
        series->append(setScores);
        std::map<int, int>::iterator it;
        for (it = score_counts.begin(); it != score_counts.end(); it++)
        {
            if (it->first >= 20 && it->first < 40)   plus20 += it->second;
            if (it->first >= 40 && it->first < 60)   plus40 += it->second;
            if (it->first >= 60 && it->first < 80)   plus60 += it->second;
            if (it->first >= 80 && it->first < 100)  plus80 += it->second;
            if (it->first >= 100 && it->first < 120) plus100 += it->second;
            if (it->first >= 120 && it->first < 140) plus120 += it->second;
            if (it->first >= 140 && it->first < 160) plus140 += it->second;
            if (it->first >= 160 && it->first < 180) plus160 += it->second;
            if (it->first == 180) the180s += it->second;
            setScores->append(it->second);
            categories.append(QString::number(it->first));
        }
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        if (categories.size()) axisX->setMin(categories.first());
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        QValueAxis *axisY = new QValueAxis();
        std::map<int, int>::iterator best = std::max_element(score_counts.begin(), score_counts.end(), [] (const std::pair<int, int>& a, const std::pair<int, int>& b)->bool{return a.second < b.second;});
        qreal max = static_cast<qreal>(best->second);
        axisY->setRange(0, max);
        axisY->setTickType(QValueAxis::TicksFixed);
        axisY->setTickCount(std::min(std::max(2,static_cast<int>(max)+1),10));
        axisY->setLabelFormat("%i");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        QObject::connect(setScores, &QBarSet::hovered,this,[setScores](bool status, int index){
            QPoint p = QCursor::pos();
            if (status) {
                QToolTip::showText(p,QString::number(setScores->at(index)));
            }
        });
    }
    if (thrownDarts.size() > 0)
    {
        series2->append(setDarts);
        std::map<QString, int>::iterator it2;
        for (it2 = dart_counts.begin(); it2 != dart_counts.end(); it2++)
        {
            setDarts->append(it2->second);
            categories2.append(it2->first);
        }
        QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
        if (categories2.size()) axisX2->setMin(categories2.first());
        axisX2->append(categories2);
        chart2->addAxis(axisX2, Qt::AlignBottom);
        series2->attachAxis(axisX2);
        QValueAxis *axisY2 = new QValueAxis();
        std::map<QString, int>::iterator best2 = std::max_element(dart_counts.begin(), dart_counts.end(), [] (const std::pair<QString, int>& a, const std::pair<QString, int>& b)->bool{return a.second < b.second;});
        qreal max2 = static_cast<qreal>(best2->second);
        axisY2->setRange(0, max2);
        axisY2->setTickType(QValueAxis::TicksFixed);
        axisY2->setTickCount(std::min(std::max(2,static_cast<int>(max2)+1),10));
        axisY2->setLabelFormat("%i");
        chart2->addAxis(axisY2, Qt::AlignLeft);
        series2->attachAxis(axisY2);
        QObject::connect(setDarts, &QBarSet::hovered,this,[setDarts](bool status, int index){
            QPoint p = QCursor::pos();
            if (status) {
                QToolTip::showText(p,QString::number(setDarts->at(index)));
            }
        });
    }
    stats->setChart(chart, chart2);
    stats->setLabel1DartAvg(Player->get_avg1dart());
    stats->setLabel3DartAvg(Player->get_avg3dart());
    stats->setLabelCheckout(Player->get_checkout());
    stats->setLabelCheckoutAttempts(Player->getCheckoutAttempts());
    stats->setLabel20s(plus20);
    stats->setLabel40s(plus40);
    stats->setLabel60s(plus60);
    stats->setLabel80s(plus80);
    stats->setLabel100s(plus100);
    stats->setLabel120s(plus120);
    stats->setLabel140s(plus140);
    stats->setLabel160s(plus160);
    stats->setLabel180s(the180s);
    QVector<int> legscores = Player->get_LegScores();
    QVector<QVector<int>> totalscores = Player->get_TotalScores();
    int numberOfLegs = 0;
    if (legscores.size() == 0 && totalscores.size() > 0) {
        legscores = totalscores.last();
    }
    numberOfLegs = legscores.size() > 0 ? totalscores.size() + 1 : totalscores.size();
    stats->initLegSelector(numberOfLegs);
    legscores = Player->get_LegScores();
    totalscores = Player->get_TotalScores();
    QVector<QVector<QString>> legdarts = Player->get_LegDarts();
    QVector<QVector<QVector<QString>>> darts = Player->getThrownDartsOfAllLegs();
    QVector<int> legremaining = Player->get_LegRemaining();
    QVector<QVector<int>> totalremaining = Player->get_RemainingOfAllLegs();
    if (legscores.size()) totalscores.append(legscores);
    if (legremaining.size()) totalremaining.append(legremaining);
    if (legdarts.size()) darts.append(legdarts);
    QVector<int> dartCountOfWonLegs = {};
    QVector<int> allCheckouts = {};
    for (int idx = 0; idx < totalremaining.size(); idx++)
    {
        if (totalremaining.at(idx).back() == 0)
        {
            dartCountOfWonLegs.append((darts.at(idx).size() - 1) * 3 + darts.at(idx).back().size());
            allCheckouts.append(totalscores.at(idx).back());
        }
    }
    stats->setLabelLegAvg(computeAverage(dartCountOfWonLegs));
    int bestLeg = *std::max_element(dartCountOfWonLegs.begin(), dartCountOfWonLegs.end());
    int worstLeg = *std::min_element(dartCountOfWonLegs.begin(), dartCountOfWonLegs.end());
    int highestCheckout = *std::max_element(allCheckouts.begin(), allCheckouts.end());
    stats->setLabelHighestCheckout(highestCheckout);
    stats->setLabelBestLeg(bestLeg);
    stats->setLabelWorstLeg(worstLeg);

    stats->show();
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
