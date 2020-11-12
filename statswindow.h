#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QString>
#include <QBarSet>
#include <QtCharts>

class Callout;

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    ~StatsWindow();
    void setLabel1DartAvg(double avg);
    void setLabel3DartAvg(double avg);
    void setLabelCheckout(double checkout);
    void setChart(QChart *chart1, QChart *chart2);
    void setText(QString text);

private:
    Ui::StatsWindow *ui;
};

#endif // STATSWINDOW_H
