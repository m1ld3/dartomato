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
#include <QDebug>

GroupBox_player::GroupBox_player(QWidget *parent, int player_nr, int game, int sets, int legs, bool singleIn,
                                 bool singleOut, bool doubleIn, bool doubleOut, bool masterIn,
                                 bool masterOut, bool offensive, PlayerClass *player) :
    QGroupBox(parent), ui(new Ui::GroupBox_player), Player(player), StartVal(game),
    Sets(sets), Legs(legs), Score(game), mCurrentScore(0), SingleIn(singleIn), SingleOut(singleOut),
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
    displayFinishes();
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
//    delete anotherone;
//    delete sexy69;
//    delete sound1;
//    delete sound2;
//    delete sound3;
//    delete sound4;
//    delete sound5;
//    delete sound6;
//    delete sound7;
//    delete sound8;
//    delete sound9;
//    delete sound10;
//    delete sound11;
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
    Score = StartVal;
    ui->lcdNumber->display(Score);
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

void GroupBox_player::closeScoreInput()
{
    this->scoreinput->close();
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

void GroupBox_player::signalSubmitButtonPressed2(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts)
{
    mCurrentScore = score;
    GroupBox_player::legstarted = true;
    GroupBox_player::setstarted = true;
    bool newset = false;
    int score3 = 0;
    int score7 = 0;
    int score11 = 0;
    int score26 = 0;
    int score180 = 0;
    int plus100 = 0;
    int plus120 = 0;
    int plus140 = 0;
    int plus160 = 0;
    switch (mCurrentScore)
    {
    case 0: numberofdarts = 3; break;
    case 3: score3 += 1; break;
    case 7: score7 += 1; break;
    case 11: score11 += 1; break;
    case 26: score26 += 1; break;
    case 180: score180 += 1; break;
    }
    if (mCurrentScore >= 100 && mCurrentScore < 120) {
        plus100 += 1;
    } else if (mCurrentScore >= 120 && mCurrentScore < 140) {
        plus120 += 1;
    } else if (mCurrentScore >= 140 && mCurrentScore < 160) {
        plus140 += 1;
    } else if (mCurrentScore >= 160 && mCurrentScore < 180) {
        plus160 += 1;
    }
    int t20 = std::count(darts.begin(),darts.end(),"t60");
    int t19 = std::count(darts.begin(),darts.end(),"t57");
    int t18 = std::count(darts.begin(),darts.end(),"t54");
    int t17 = std::count(darts.begin(),darts.end(),"t51");
    int t16 = std::count(darts.begin(),darts.end(),"t48");
    int t15 = std::count(darts.begin(),darts.end(),"t45");
    int bull = std::count(darts.begin(),darts.end(),"d50");
    int ones = std::count(darts.begin(),darts.end(),"s1");
    int miss = std::count(darts.begin(),darts.end(),"s0");
    miss += std::count(darts.begin(),darts.end(),"d0");
    miss += std::count(darts.begin(),darts.end(),"t0");
    Score = Player->set_score(mCurrentScore);
    Player->set_darts(darts);
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << mCurrentScore;
    std::string digits = ss.str();
    std::string strpath = "qrc:/resources/sounds/" + digits +".wav";
    QString filepath = QString::fromStdString(strpath);
    scoresound.setSource(filepath);
    Player->compute_averages(numberofdarts);
    Player->updateStats(score180, plus160, plus140, plus120, plus100, score26, score11, score7,
                        score3, t20, t19, t18, t17, t16, t15, bull, ones, miss);
    if (Score == 0) {
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
        ui->lcdNumber->display(Score);
        if (Active) {
            emit signalUpdatePlayer("default");
        }
    }
    if (!Finished) {
        closeScoreInput();
    }
    QString avg1dart = QString::number(Player->get_avg1dart(),'f',3);
    QString avg3dart = QString::number(Player->get_avg3dart(),'f',3);
    QString checkout = QString::number(Player->get_checkout(),'f',3) + "%";
    ui->label_1dartInput->setText(avg1dart);
    ui->label_3dartInput->setText(avg3dart);
    ui->label_checkoutInput->setText(checkout);
    displayFinishes();
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
        }
    }
}

void GroupBox_player::on_pushButton_score_clicked()
{
    if (Active && !Finished) {
        scoreinput = new ScoreInput(this, StartVal, Score, Sets, Legs, SingleIn, SingleOut,
                                                DoubleIn, DoubleOut, MasterIn, MasterOut);
        scoreinput->setAttribute(Qt::WA_DeleteOnClose);
        connect(scoreinput, SIGNAL (signalSubmitButtonPressed2(int&, int&, int&, QVector<QString>)), this, SLOT (signalSubmitButtonPressed2(int&, int&, int&, QVector<QString>)));
        scoreinput->show();
    } else if (Finished) {
        QMessageBox::about(this, "Warning", "Game already finished!");
    } else {
        QMessageBox::about(this, "Warning", "It's not your turn!");
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
}

void GroupBox_player::unsetLegBegin()
{
    LegBegin = false;
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

void GroupBox_player::displayFinishes()
{
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
    ui->textBrowser->setText("Possible finishes");
    if (SingleOut) {
        for (int i = 0; i < vals.size(); i++) {
            if (Score - vals[i] == 0) {
                QString text = valslabels[i];
                ui->textBrowser->append(text);
            }
        }
        for (int i = 0; i < vals.size(); i++) {
            for (int j = 0; j < vals.size(); j++) {
                if (Score - vals[i]-vals[j] == 0) {
                    QString text = valslabels[i] + "   " + valslabels[j];
                    ui->textBrowser->append(text);
                }
            }
        }
        if (Score > 50) {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    for (int k = 0; k < vals.size(); k++) {
                        if (Score - vals[i]-vals[j]-vals[k] == 0) {
                            QString text = valslabels[i] + "   " + valslabels[j] + "   " + valslabels[k];
                            ui->textBrowser->append(text);
                        }
                    }
                }
            }
        }
    } else if (DoubleOut) {
        for (int i = 0; i < doubles.size(); i++) {
            if (Score - doubles[i] == 0) {
                QString text = doubleslabels[i];
                ui->textBrowser->append(text);
            }
        }
        for (int i = vals.size()-1; i >= 0; i--) {
            for (int j = 0; j < doubles.size(); j++) {
                if (Score - vals[i]-doubles[j] == 0) {
                    QString text = valslabels[i] + "   " + doubleslabels[j];
                    ui->textBrowser->append(text);
                }
            }
        }
        if (Score > 50) {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    for (int k = 0; k < doubles.size(); k++) {
                        if (Score - vals[i]-vals[j]-doubles[k] == 0) {
                            QString text = valslabels[i] + "   " + valslabels[j] + "   " + doubleslabels[k];
                            ui->textBrowser->append(text);
                        }
                    }
                }
            }
        }
    } else if (MasterOut) {
        for (int i = 0; i < triples.size(); i++) {
            if (Score - vals[i] == 0) {
                QString text = tripleslabels[i];
                ui->textBrowser->append(text);
            }
        }
        for (int i = 0; i < vals.size(); i++) {
            for (int j = 0; j < triples.size(); j++) {
                if (Score - vals[i]-triples[j] == 0) {
                    QString text = valslabels[i] + "   " + tripleslabels[j];
                    ui->textBrowser->append(text);
                }
            }
        }
        if (Score > 50) {
            for (int i = 0; i < vals.size(); i++) {
                for (int j = 0; j < vals.size(); j++) {
                    for (int k = 0; k < triples.size(); k++) {
                        if (Score - vals[i]-vals[j]-triples[k] == 0) {
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
    if (!(Score == StartVal && mCurrentScore > 0))
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

    //*setScores << Player->get_180() << Player->get_160() << Player->get_140() << Player->get_120() <<
                  //Player->get_100() << Player->get_26() <<Player->get_11() << Player->get_7() << Player->get_3();
    //*setDarts << Player->get_t20() << Player->get_t19() << Player->get_t18() << Player->get_t17() <<
                 //Player->get_t16() << Player->get_t15() << Player->get_bull() << Player->get_ones() << Player->get_miss();
    QBarSeries *series = new QBarSeries();
    QBarSeries *series2 = new QBarSeries();
    series->append(setScores);
    series->setBarWidth(0.9);
    series2->append(setDarts);
    QChart *chart = new QChart();
    QChart *chart2 = new QChart();
    chart->addSeries(series);
    chart2->addSeries(series2);
    chart->setTitle("Scoring statistics");
    chart2->setTitle("Single dart statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart2->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    QStringList categories2;
    //categories << "180" << "160+" << "140+" << "120+" << "100+" << "26" << "11" << "7" << "3";
    //categories2 << "T20" << "T19" << "T18" << "T17" << "T16" << "T15" << "Bull" << "1" << "Miss";
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
    auto axis = qobject_cast<QBarCategoryAxis*>(axisX);
    auto categories_test = axis->categories();
//    QString min = categories.at(n);
//    QString max = categories.at(m);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chart2->addAxis(axisX2, Qt::AlignBottom);
    series2->attachAxis(axisX2);
    QValueAxis *axisY = new QValueAxis();
    QValueAxis *axisY2 = new QValueAxis();
    //qreal max = static_cast<qreal>(std::max({Player->get_180(), Player->get_160(), Player->get_140(),
    //                                         Player->get_120(), Player->get_100(), Player->get_26(),
    //                                         Player->get_11(), Player->get_7(), Player->get_7()}));
    std::map<int, int>::iterator best = std::max_element(score_counts.begin(), score_counts.end(), [] (const std::pair<int, int>& a, const std::pair<int, int>& b)->bool{return a.second < b.second;});
    qreal max = static_cast<qreal>(best->second);
    axisY->setRange(0, max);
    axisY->setTickType(QValueAxis::TicksFixed);
    axisY->setTickCount(std::min(std::max(2,static_cast<int>(max)+1),10));
    axisY->setLabelFormat("%i");
//    qreal max2 = static_cast<qreal>(std::max({Player->get_t20(), Player->get_t19(), Player->get_t18(),
//                                              Player->get_t17(), Player->get_t16(), Player->get_t15(),
//                                              Player->get_bull(), Player->get_ones(), Player->get_miss()}));
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
    Player->undo();
    ui->lcdNumber->display(Player->get_remaining());
    ui->lcdNumber_legs->display(Player->get_legs());
    ui->lcdNumber_sets->display(Player->get_sets());
    Score = Player->get_remaining();
    QString avg1dart = QString::number(Player->get_avg1dart(),'f',3);
    QString avg3dart = QString::number(Player->get_avg3dart(),'f',3);
    QString checkout = QString::number(Player->get_checkout(),'f',3) + "%";
    ui->label_1dartInput->setText(avg1dart);
    ui->label_3dartInput->setText(avg3dart);
    ui->label_checkoutInput->setText(checkout);
    if (Finished) {
        unsetFinished();
    }
    displayFinishes();
}

bool GroupBox_player::legstarted = false;
bool GroupBox_player::setstarted = false;
