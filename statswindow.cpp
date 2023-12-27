#include "statswindow.h"
#include "ui_statswindow.h"

CStatsWindow::CStatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStatsWindow)
{
    ui->setupUi(this);
    ui->chartView1->setMouseTracking(true);
    ui->chartView2->setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->chartView1->setRubberBand( QChartView::HorizontalRubberBand);
    ui->chartView2->setRubberBand( QChartView::HorizontalRubberBand);
}

CStatsWindow::~CStatsWindow()
{
    delete ui;
}

void CStatsWindow::set_label_1dart_avg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_1dartInput->setText(avgstr);
}

void CStatsWindow::set_label_3dart_avg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_3dartInput->setText(avgstr);
}

void CStatsWindow::set_label_checkout(double checkout)
{
    QString checkstr = QString::number(checkout, 'f', 2) + "%";
    ui->label_checkoutInput->setText(checkstr);
}

void CStatsWindow::set_label_leg_1dart_avg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_legavg1_input->setText(avgstr);
}

void CStatsWindow::set_label_leg_3dart_avg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_legavg3_input->setText(avgstr);
}

void CStatsWindow::set_label_best_leg(int numberOfDarts)
{
    ui->label_best_leg_input->setText(QString::number(numberOfDarts));
}

void CStatsWindow::set_label_worst_leg(int numberOfDarts)
{
    ui->label_worst_leg_input->setText(QString::number(numberOfDarts));
}

void CStatsWindow::set_label_leg_avg(double avg)
{
    QString avgstr = QString::number(avg, 'f', 2);
    ui->label_leg_avg_input->setText(avgstr);
}

void CStatsWindow::set_label_checkout_attempts(QString attempts)
{
    ui->label_checkout_attempts_input->setText(attempts);
}

void CStatsWindow::set_label_highest_checkout(int checkout)
{
    ui->label_highest_checkout_input->setText(QString::number(checkout));
}

void CStatsWindow::set_label_180s(int count)
{
    ui->label_180_input->setText(QString::number(count));
}

void CStatsWindow::set_label_160s(int count)
{
    ui->label_160p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_140s(int count)
{
    ui->label_140p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_120s(int count)
{
    ui->label_120p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_100s(int count)
{
    ui->label_100p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_80s(int count)
{
    ui->label_80p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_60s(int count)
{
    ui->label_60p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_40s(int count)
{
    ui->label_40p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_20s(int count)
{
    ui->label_20p_input->setText(QString::number(count));
}

void CStatsWindow::set_label_0s(int count)
{
    ui->label_0p_input->setText(QString::number(count));
}

void CStatsWindow::set_chart(CChart *chart1, CChart *chart2)
{
    ui->chartView1->setChart(chart1);
    ui->chartView2->setChart(chart2);
}

void CStatsWindow::set_text(QString text)
{
    ui->textBrowser->append(text);
}

void CStatsWindow::clear_text()
{
    ui->textBrowser->clear();
}

void CStatsWindow::init_leg_selector(int numberOfLegs)
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

void CStatsWindow::on_leg_selector_current_index_changed(int index)
{
    emit signal_update_leg_history(index, this);
}
