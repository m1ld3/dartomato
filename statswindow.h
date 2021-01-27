#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QString>
#include <QBarSet>
#include "chart.h"
#include "chartview.h"

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
    void setLabelLeg1DartAvg(double avg);
    void setLabelLeg3DartAvg(double avg);
    void setLabelBestLeg(int numberOfDarts);
    void setLabelWorstLeg(int numberOfDarts);
    void setLabelLegAvg(double avg);
    void setLabelCheckoutAttempts(QString attempts);
    void setLabelHighestCheckout(int checkout);
    void setLabel180s(int count);
    void setLabel160s(int count);
    void setLabel140s(int count);
    void setLabel120s(int count);
    void setLabel100s(int count);
    void setLabel80s(int count);
    void setLabel60s(int count);
    void setLabel40s(int count);
    void setLabel20s(int count);
    void setLabel0s(int count);
    void setChart(Chart *chart1, Chart *chart2);
    void setText(QString text);
    void clearText();
    void initLegSelector(int numberOfLegs);
signals:
    void signalUpdateLegHistory(int index, StatsWindow* stats);

private slots:
    void on_legSelector_currentIndexChanged(int index);

private:
    Ui::StatsWindow *ui;
};

#endif // STATSWINDOW_H
