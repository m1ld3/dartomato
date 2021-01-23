#include "statswindow.h"
#include "ui_statswindow.h"

StatsWindow::StatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsWindow)
{
    ui->setupUi(this);
    ui->chartView1->setMouseTracking(true);
    ui->chartView2->setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->chartView1->setRubberBand( QChartView::HorizontalRubberBand);
    ui->chartView2->setRubberBand( QChartView::HorizontalRubberBand);
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::setLabel1DartAvg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_1dartInput->setText(avgstr);
}

void StatsWindow::setLabel3DartAvg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_3dartInput->setText(avgstr);
}

void StatsWindow::setLabelCheckout(double checkout)
{
    QString checkstr = QString::number(checkout, 'f', 2) + "%";
    ui->label_checkoutInput->setText(checkstr);
}

void StatsWindow::setLabelLeg1DartAvg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_legavg1_input->setText(avgstr);
}

void StatsWindow::setLabelLeg3DartAvg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_legavg3_input->setText(avgstr);
}

void StatsWindow::setLabelBestLeg(int numberOfDarts)
{
    ui->label_best_leg_input->setText(QString::number(numberOfDarts));
}

void StatsWindow::setLabelWorstLeg(int numberOfDarts)
{
    ui->label_worst_leg_input->setText(QString::number(numberOfDarts));
}

void StatsWindow::setLabelLegAvg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_leg_avg_input->setText(avgstr);
}

void StatsWindow::setLabelCheckoutAttempts(QString attempts)
{
    ui->label_checkout_attempts_input->setText(attempts);
}

void StatsWindow::setLabelHighestCheckout(int checkout)
{
    ui->label_highest_checkout_input->setText(QString::number(checkout));
}

void StatsWindow::setLabel180s(int count)
{
    ui->label_180_input->setText(QString::number(count));
}

void StatsWindow::setLabel160s(int count)
{
    ui->label_160p_input->setText(QString::number(count));
}

void StatsWindow::setLabel140s(int count)
{
    ui->label_140p_input->setText(QString::number(count));
}

void StatsWindow::setLabel120s(int count)
{
    ui->label_120p_input->setText(QString::number(count));
}

void StatsWindow::setLabel100s(int count)
{
    ui->label_100p_input->setText(QString::number(count));
}

void StatsWindow::setLabel80s(int count)
{
    ui->label_80p_input->setText(QString::number(count));
}

void StatsWindow::setLabel60s(int count)
{
    ui->label_60p_input->setText(QString::number(count));
}

void StatsWindow::setLabel40s(int count)
{
    ui->label_40p_input->setText(QString::number(count));
}

void StatsWindow::setLabel20s(int count)
{
    ui->label_20p_input->setText(QString::number(count));
}

void StatsWindow::setChart(Chart *chart1, Chart *chart2)
{
    ui->chartView1->setChart(chart1);
    ui->chartView2->setChart(chart2);
}

void StatsWindow::setText(QString text)
{
    ui->textBrowser->append(text);
}

void StatsWindow::clearText()
{
    ui->textBrowser->clear();
}

void StatsWindow::initLegSelector(int numberOfLegs)
{
    if (numberOfLegs == 0)
    {
        ui->legSelector->addItem("1");
        ui->legSelector->setCurrentIndex(0);
    }
    else
    {
        for (int i = 1; i < numberOfLegs + 1; i++)
        {
            ui->legSelector->addItem(QString::number(i));
        }
        ui->legSelector->setCurrentIndex(numberOfLegs - 1);
    }
}

void StatsWindow::on_legSelector_currentIndexChanged(int index)
{
    emit signalUpdateLegHistory(index, this);
}
