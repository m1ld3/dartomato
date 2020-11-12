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
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::setLabel1DartAvg(double avg)
{
    QString avgstr = QString::number(avg,'f',3);
    ui->label_1dartInput->setText(avgstr);
}

void StatsWindow::setLabel3DartAvg(double avg)
{
    QString avgstr = QString::number(avg,'f',3);
    ui->label_3dartInput->setText(avgstr);
}

void StatsWindow::setLabelCheckout(double checkout)
{
    QString checkstr = QString::number(checkout,'f',3) + "%";
    ui->label_checkoutInput->setText(checkstr);
}

void StatsWindow::setChart(QChart *chart1, QChart *chart2)
{
    ui->chartView1->setChart(chart1);
    ui->chartView2->setChart(chart2);
}

void StatsWindow::setText(QString text)
{
    ui->textBrowser->append(text);
}
