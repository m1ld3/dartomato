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
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QToolTip>
#include <QtCharts/QValueAxis>
#include <xo1mainwindow.h>
#include <checkoutlists.h>

CX01GroupBox::CX01GroupBox(QWidget * iParent, const CSettings & ipSettings,
                           uint32_t iPlayerNumber, CX01Class * iPlayer, CDartBoardX01 * iDartBoard)
  : QGroupBox(iParent)
  , mUi(new Ui::CX01GroupBox)
  , mPlayer(iPlayer)
  , mDartBoard(iDartBoard)
  , mScoreSound(this)
  , mpSettings(ipSettings)
{
  mUi->setupUi(this);
  mUi->lcdNumber->setDigitCount(3);
  mUi->lcdNumber->display(static_cast<int>(mpSettings.mGame));
  mUi->lcdNumber->setPalette(Qt::darkBlue);
  QString text = "Player " + QString::number(iPlayerNumber);
  mUi->label_playername->setText(text);
  QString avg1dart = QString::number(mPlayer->get_avg1dart(), 'f', 3);
  QString avg3dart = QString::number(mPlayer->get_avg3dart(), 'f', 3);
  QString checkout = QString::number(mPlayer->get_checkout(), 'f', 3) + "%";
  mUi->label_1dartInput->setText(avg1dart);
  mUi->label_3dartInput->setText(avg3dart);
  mUi->label_checkoutInput->setText(checkout);
  connect(mUi->label_pic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CX01GroupBox::player_active_button_pressed_slot);
  display_finishes(mRemaining, 3);
  mGameWindow = dynamic_cast<CX01MainWindow*>(iParent);

  connect(mUi->label_pic, &CPlayerActiveButton::signal_player_active_button_pressed, this, &CX01GroupBox::player_active_button_pressed_slot);
  connect(mUi->pushButton_name, &QPushButton::clicked, this, &CX01GroupBox::push_button_name_clicked_slot);
  connect(mUi->pushButton_undo, &QPushButton::clicked, this, &CX01GroupBox::push_button_undo_clicked_slot);
  connect(mUi->pushButton_stats, &QPushButton::clicked, this, &CX01GroupBox::push_button_stats_clicked_slot);
}

CX01GroupBox::~CX01GroupBox()
{
  delete mUi;
}

void CX01GroupBox::set_active()
{
  mActive = true;
  uint32_t w = 80;
  uint32_t h = 80;
  mUi->label_pic->setPixmap(mPixMap.scaled(w,h,Qt::KeepAspectRatio));
}

void CX01GroupBox::set_inactive()
{
  mActive = false;
  mUi->label_pic->clear();
}

void CX01GroupBox::reset()
{
  mRemaining = static_cast<uint32_t>(mpSettings.mGame);
  mUi->lcdNumber->display(static_cast<int>(mRemaining));
  mPlayer->reset_score();
}

void CX01GroupBox::set_finished()
{
  mFinished = true;
}

void CX01GroupBox::unset_finished()
{
  mFinished = false;
}

QString CX01GroupBox::get_player_number() const
{
  return mUi->label_playername->text();
}

void CX01GroupBox::push_button_name_clicked_slot()
{
  QPointer<CDialogNameInput> dn = new CDialogNameInput(this, mUi->label_playername->text());
  dn->setAttribute(Qt::WA_DeleteOnClose);
  connect(dn, &CDialogNameInput::signal_ok_button_clicked, this, &CX01GroupBox::ok_button_clicked_slot);
  dn->show();
}

void CX01GroupBox::ok_button_clicked_slot(const QString & iName)
{
  mPlayerName = iName;
  mUi->label_playername->setText(mPlayerName);
}

void CX01GroupBox::submit_score(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts)
{
  mCurrentScore = iScore;
  CX01GroupBox::mLegStarted = true;
  CX01GroupBox::mSetStarted = true;
  bool newset = false;
  mRemaining = mPlayer->set_score(mCurrentScore);
  mPlayer->set_darts(iDarts);
  std::stringstream ss;
  ss << std::setw(3) << std::setfill('0') << mCurrentScore;
  std::string digits = ss.str();
  std::string strpath = "qrc:/resources/sounds/" + digits + ".wav";
  QString filepath = QString::fromStdString(strpath);
  mScoreSound.setSource(filepath);
  mPlayer->compute_averages(iNumberOfDarts);

  if (mRemaining == 0)
  {
    mPlayer->update_checkout(iCheckoutAttempts, 1);
    newset = mPlayer->increase_setslegs();
    emit signal_update_history();
    mScoreSound.play();
    emit signal_reset_scores();

    if (mActive && !newset)
    {
      emit signal_update_player(EUpdateType::LEG);
      CX01GroupBox::mLegStarted = false;
    }
    else if (mActive && newset)
    {
      emit signal_update_player(EUpdateType::SET);
      CX01GroupBox::mSetStarted = false;
      CX01GroupBox::mLegStarted = false;
    }

    mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
    mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
  }
  else
  {
    mPlayer->update_checkout(iCheckoutAttempts, 0);
    mScoreSound.play();
    mUi->lcdNumber->display(static_cast<int>(mRemaining));

    if (mActive)
    {
      emit signal_update_player(EUpdateType::DEFAULT);
    }
  }

  QString avg1dart = QString::number(mPlayer->get_avg1dart(), 'f', 3);
  QString avg3dart = QString::number(mPlayer->get_avg3dart(), 'f', 3);
  QString checkout = QString::number(mPlayer->get_checkout(), 'f', 3) + "%";
  mUi->label_1dartInput->setText(avg1dart);
  mUi->label_3dartInput->setText(avg3dart);
  mUi->label_checkoutInput->setText(checkout);
  display_finishes(mRemaining, 3);
}

void CX01GroupBox::player_active_button_pressed_slot()
{
  if (!mActive)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Change player order", "Do you really want to change the player order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
      emit signal_inactivate_players(mPlayer->get_player_number(), CX01GroupBox::mLegStarted, CX01GroupBox::mSetStarted);
      set_active();
      mDartBoard->init_dartboard(mRemaining);
    }
  }
}

void CX01GroupBox::set_set_begin()
{
  mSetBegin = true;
}

void CX01GroupBox::unset_set_begin()
{
  mSetBegin = false;
}

void CX01GroupBox::set_leg_begin()
{
  mLegBegin = true;
  mUi->label_pic->setStyleSheet("#label_pic { border: 5px solid red; }");
}

void CX01GroupBox::unset_leg_begin()
{
  mLegBegin = false;
  mUi->label_pic->setStyleSheet("");
}

bool CX01GroupBox::has_begun_leg() const
{
  return mLegBegin;
}

bool CX01GroupBox::has_begun_set() const
{
  return mSetBegin;
}

void CX01GroupBox::update_history()
{
  mPlayer->update_history();
}

void CX01GroupBox::reset_legs()
{
  mPlayer->reset_legs();
}

void CX01GroupBox::display_finishes(uint32_t iRemaining, uint32_t iNumberOfDarts)
{
  mUi->textBrowser->clear();
  mUi->textBrowser->setText("Checkouts:");
  if (iNumberOfDarts == 0) return;

  if (mpSettings.mSingleOut)
  {
    if (singleOutSingleDartCheckoutList.find(iRemaining) != singleOutSingleDartCheckoutList.end())
    {
      QVector<QString> checkouts = singleOutSingleDartCheckoutList.find(iRemaining).value();
      for (auto checkout : checkouts)
      {
        mUi->textBrowser->append(checkout);
      }
    }
    if (iNumberOfDarts > 1)
    {
      if (singleOutTwoDartCheckoutList.find(iRemaining) != singleOutTwoDartCheckoutList.end())
      {
        QVector<QString> checkouts = singleOutTwoDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
    if (iNumberOfDarts >2)
    {
      if (singleOutThreeDartCheckoutList.find(iRemaining) != singleOutThreeDartCheckoutList.end())
      {
        QVector<QString> checkouts = singleOutThreeDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
  }
  else if (mpSettings.mDoubleOut)
  {
    if (doubleOutSingleDartCheckoutList.find(iRemaining) != doubleOutSingleDartCheckoutList.end())
    {
      QVector<QString> checkouts = doubleOutSingleDartCheckoutList.find(iRemaining).value();
      for (auto checkout : checkouts)
      {
        mUi->textBrowser->append(checkout);
      }
    }
    if (iNumberOfDarts > 1)
    {
      if (doubleOutTwoDartCheckoutList.find(iRemaining) != doubleOutTwoDartCheckoutList.end())
      {
        QVector<QString> checkouts = doubleOutTwoDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
    if (iNumberOfDarts >2)
    {
      if (doubleOutThreeDartCheckoutList.find(iRemaining) != doubleOutThreeDartCheckoutList.end())
      {
        QVector<QString> checkouts = doubleOutThreeDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
  }
  else if (mpSettings.mMasterOut)
  {
    if (masterOutSingleDartCheckoutList.find(iRemaining) != masterOutSingleDartCheckoutList.end())
    {
      QVector<QString> checkouts = masterOutSingleDartCheckoutList.find(iRemaining).value();
      for (auto checkout : checkouts)
      {
        mUi->textBrowser->append(checkout);
      }
    }
    if (iNumberOfDarts > 1)
    {
      if (masterOutTwoDartCheckoutList.find(iRemaining) != masterOutTwoDartCheckoutList.end())
      {
        QVector<QString> checkouts = masterOutTwoDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
    if (iNumberOfDarts >2)
    {
      if (masterOutThreeDartCheckoutList.find(iRemaining) != masterOutThreeDartCheckoutList.end())
      {
        QVector<QString> checkouts = masterOutThreeDartCheckoutList.find(iRemaining).value();
        for (auto checkout : checkouts)
        {
          mUi->textBrowser->append(checkout);
        }
      }
    }
  }
  QTextCursor cursor = mUi->textBrowser->textCursor();
  cursor.setPosition(0);
  mUi->textBrowser->setTextCursor(cursor);
}

void CX01GroupBox::set_lcd_legs()
{
  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
}

uint32_t CX01GroupBox::get_remaining() const
{
  return mPlayer->get_remaining();
}

void CX01GroupBox::perform_undo()
{
  mPlayer->undo();
  mRemaining = mPlayer->get_remaining();
  mUi->lcdNumber->display(static_cast<int>(mRemaining));

  if (mActive)
  {
    mDartBoard->init_dartboard(mRemaining);
  }

  mUi->lcdNumber_legs->display(static_cast<int>(mPlayer->get_legs()));
  mUi->lcdNumber_sets->display(static_cast<int>(mPlayer->get_sets()));
  QString avg1dart = QString::number(mPlayer->get_avg1dart(), 'f', 3);
  QString avg3dart = QString::number(mPlayer->get_avg3dart(), 'f', 3);
  QString checkout = QString::number(mPlayer->get_checkout(), 'f', 3) + "%";
  mUi->label_1dartInput->setText(avg1dart);
  mUi->label_3dartInput->setText(avg3dart);
  mUi->label_checkoutInput->setText(checkout);

  if (mFinished)
  {
    unset_finished();
    mDartBoard->unset_finished();
  }
  display_finishes(mRemaining, 3);
}

void CX01GroupBox::update_leg_history_slot(uint32_t iIndex, CStatsWindow * iStats)
{
  iStats->clear_text();
  iStats->set_label_leg_1dart_avg(0.0);
  iStats->set_label_leg_3dart_avg(0.0);
  QVector<uint32_t> legscores = mPlayer->get_leg_scores();
  QVector<QVector<uint32_t>> totalscores = mPlayer->get_total_scores();

  if (legscores.size()) totalscores.append(legscores);
  if (totalscores.size() >= iIndex + 1)
  {
    for (int i = 0; i < totalscores.at(iIndex).size(); i++)
    {
      QString line = QString::number(i+1) + ": " + QString::number(totalscores.at(iIndex)[i]);
      iStats->set_text(line);
    }
    QVector<QVector<QString>> legdarts = mPlayer->get_leg_darts();
    QVector<QVector<QVector<QString>>> totaldarts = mPlayer->get_thrown_darts_of_all_legs();
    if (legdarts.size()) totaldarts.append(legdarts);
    uint32_t numberOfDarts = (totaldarts.at(iIndex).size() - 1) * 3 + totaldarts.at(iIndex).back().size();
    double avg1dart = std::accumulate(totalscores.at(iIndex).begin(), totalscores.at(iIndex).end(), 0.0)/numberOfDarts;
    double avg3dart = 3 * avg1dart;
    iStats->set_label_leg_1dart_avg(avg1dart);
    iStats->set_label_leg_3dart_avg(avg3dart);
  }
}

double CX01GroupBox::compute_average(QVector<uint32_t> vector)
{
  double avg;
  double n = static_cast<double>(vector.size());
  if (n > 0) avg = std::accumulate(vector.begin(), vector.end(), 0.0)/n;
  else avg = 0.0;
  return avg;
}

void CX01GroupBox::push_button_stats_clicked_slot()
{
  QPointer<CStatsWindow> stats = new CStatsWindow();
  connect(stats, &CStatsWindow::signal_update_leg_history, this, &CX01GroupBox::update_leg_history_slot);
  stats->setAttribute(Qt::WA_DeleteOnClose);
  stats->setModal(true);
  QPointer<QBarSet> setScores = new QBarSet("Scores");
  QPointer<QBarSet> setDarts = new QBarSet("Single Darts");
  QVector<QVector<QString>> thrownDarts = mPlayer->get_darts();
  QVector<QString> thrownDarts_flat;

  for (auto darts : thrownDarts)
  {
    thrownDarts_flat.append(darts);
  }

  for (auto &dart : thrownDarts_flat)
  {
    if (dart[0] == 'd')
    {
      QString temp = dart.remove(0,1);
      dart = "D" + QString::number(temp.toInt() / 2);
    }
    else if (dart[0] == 't')
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

  std::map<QString, uint32_t> dart_counts;
  for (auto dart : thrownDarts_flat) ++dart_counts[dart];
  QVector<uint32_t> allScores = mPlayer->get_total_scores_flat();
  std::map<uint32_t, uint32_t> score_counts;
  for (auto score : allScores) ++score_counts[score];
  QPointer<QBarSeries> series = new QBarSeries();
  QPointer<QBarSeries> series2 = new QBarSeries();
  QPointer<CChart> chart = new CChart();
  QPointer<CChart> chart2 = new CChart();
  chart->addSeries(series);
  chart2->addSeries(series2);
  chart->setTitle("Scoring statistics");
  chart2->setTitle("Single dart statistics");
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart2->setAnimationOptions(QChart::SeriesAnimations);
  QStringList categories;
  QStringList categories2;
  uint32_t the180s = 0, plus160 = 0, plus140 = 0, plus120 = 0, plus100 = 0, plus80 = 0, plus60 = 0, plus40 = 0, plus20 = 0, plus0 = 0;

  if (allScores.size() > 0)
  {
    series->append(setScores);
    std::map<uint32_t, uint32_t>::iterator it;

    for (it = score_counts.begin(); it != score_counts.end(); it++)
    {
      if (it->first < 20)                      plus0  += it->second;
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

    QPointer<QBarCategoryAxis> axisX = new QBarCategoryAxis();
    if (categories.size()) axisX->setMin(categories.first());
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QPointer<QValueAxis> axisY = new QValueAxis();
    std::map<uint32_t, uint32_t>::iterator best = std::max_element(score_counts.begin(), score_counts.end(), [] (const std::pair<int, int>& a, const std::pair<int, int>& b)->bool{return a.second < b.second;});
    qreal max = static_cast<qreal>(best->second);
    axisY->setRange(0, max);
    axisY->setTickType(QValueAxis::TicksFixed);
    axisY->setTickCount(std::min(std::max(2,static_cast<int>(max)+1),10));
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QObject::connect(setScores, &QBarSet::hovered, this, [setScores](bool status, int index)
    {
      QPoint p = QCursor::pos();
      if (status)
      {
        QToolTip::showText(p,QString::number(setScores->at(index)));
      }
    });
  }

  if (thrownDarts.size() > 0)
  {
    series2->append(setDarts);
    std::map<QString, uint32_t>::iterator it2;

    for (it2 = dart_counts.begin(); it2 != dart_counts.end(); it2++)
    {
      setDarts->append(it2->second);
      categories2.append(it2->first);
    }

    QPointer<QBarCategoryAxis> axisX2 = new QBarCategoryAxis();
    if (categories2.size()) axisX2->setMin(categories2.first());
    axisX2->append(categories2);
    chart2->addAxis(axisX2, Qt::AlignBottom);
    series2->attachAxis(axisX2);
    QPointer<QValueAxis> axisY2 = new QValueAxis();
    std::map<QString, uint32_t>::iterator best2 = std::max_element(dart_counts.begin(), dart_counts.end(), [] (const std::pair<QString, int>& a, const std::pair<QString, int>& b)->bool{return a.second < b.second;});
    qreal max2 = static_cast<qreal>(best2->second);
    axisY2->setRange(0, max2);
    axisY2->setTickType(QValueAxis::TicksFixed);
    axisY2->setTickCount(std::min(std::max(2,static_cast<int>(max2)+1),10));
    axisY2->setLabelFormat("%i");
    chart2->addAxis(axisY2, Qt::AlignLeft);
    series2->attachAxis(axisY2);

    QObject::connect(setDarts, &QBarSet::hovered, this, [setDarts](bool status, int index)
    {
      QPoint p = QCursor::pos();
      if (status)
      {
        QToolTip::showText(p,QString::number(setDarts->at(index)));
      }
    });
  }

  stats->set_chart(chart, chart2);
  stats->set_label_1dart_avg(mPlayer->get_avg1dart());
  stats->set_label_3dart_avg(mPlayer->get_avg3dart());
  stats->set_label_checkout(mPlayer->get_checkout());
  stats->set_label_checkout_attempts(mPlayer->get_checkout_attempts());
  stats->set_label_0s(plus0);
  stats->set_label_20s(plus20);
  stats->set_label_40s(plus40);
  stats->set_label_60s(plus60);
  stats->set_label_80s(plus80);
  stats->set_label_100s(plus100);
  stats->set_label_120s(plus120);
  stats->set_label_140s(plus140);
  stats->set_label_160s(plus160);
  stats->set_label_180s(the180s);
  QVector<uint32_t> legscores = mPlayer->get_leg_scores();
  QVector<QVector<uint32_t>> totalscores = mPlayer->get_total_scores();
  uint32_t numberOfLegs = 0;

  if (legscores.size() == 0 && totalscores.size() > 0)
  {
    legscores = totalscores.last();
  }

  numberOfLegs = legscores.size() > 0 ? totalscores.size() + 1 : totalscores.size();
  stats->init_leg_selector(numberOfLegs);
  legscores = mPlayer->get_leg_scores();
  totalscores = mPlayer->get_total_scores();
  QVector<QVector<QString>> legdarts = mPlayer->get_leg_darts();
  QVector<QVector<QVector<QString>>> darts = mPlayer->get_thrown_darts_of_all_legs();
  QVector<uint32_t> legremaining = mPlayer->get_leg_remaining();
  QVector<QVector<uint32_t>> totalremaining = mPlayer->get_remaining_of_all_legs();
  if (legscores.size()) totalscores.append(legscores);
  if (legremaining.size()) totalremaining.append(legremaining);
  if (legdarts.size()) darts.append(legdarts);
  QVector<uint32_t> dartCountOfWonLegs = {};
  QVector<uint32_t> allCheckouts = {};

  for (uint32_t idx = 0; idx < totalremaining.size(); idx++)
  {
    if (totalremaining.at(idx).back() == 0)
    {
      dartCountOfWonLegs.append((darts.at(idx).size() - 1) * 3 + darts.at(idx).back().size());
      allCheckouts.append(totalscores.at(idx).back());
    }
  }

  stats->set_label_leg_avg(compute_average(dartCountOfWonLegs));

  uint32_t bestLeg = 0;
  uint32_t worstLeg = 0;
  if (dartCountOfWonLegs.size() > 0)
  {
    bestLeg = *std::min_element(dartCountOfWonLegs.begin(), dartCountOfWonLegs.end());
    worstLeg = *std::max_element(dartCountOfWonLegs.begin(), dartCountOfWonLegs.end());
    stats->set_label_best_leg(bestLeg);
    stats->set_label_worst_leg(worstLeg);
  }

  uint32_t highestCheckout = 0;
  if (allCheckouts.size() > 0)
  {
    highestCheckout = *std::max_element(allCheckouts.begin(), allCheckouts.end());
    stats->set_label_highest_checkout(highestCheckout);
  }

  stats->show();
}

void CX01GroupBox::push_button_undo_clicked_slot()
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Undo",
                                                             tr("Are you sure you want to undo your last score?\n"),
                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
  if (resBtn == QMessageBox::Yes)
  {
    perform_undo();
  }
}

bool CX01GroupBox::mLegStarted = false;
bool CX01GroupBox::mSetStarted = false;
