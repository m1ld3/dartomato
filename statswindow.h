#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QString>
#include <QtCharts/QBarSet>
#include "chart.h"
#include "chartview.h"

class CCallout;

namespace Ui
{
  class CStatsWindow;
}

class CStatsWindow : public QDialog
{
  Q_OBJECT

public:

  explicit CStatsWindow(QWidget * iParent = nullptr);
  ~CStatsWindow() override;
  void set_label_1dart_avg(double iAvg);
  void set_label_3dart_avg(double iAvg);
  void set_label_checkout(double iCheckout);
  void set_label_leg_1dart_avg(double iAvg);
  void set_label_leg_3dart_avg(double iAvg);
  void set_label_best_leg(uint32_t iNumberOfDarts);
  void set_label_worst_leg(uint32_t iNumberOfDarts);
  void set_label_leg_avg(double iAvg);
  void set_label_checkout_attempts(QString iAttempts);
  void set_label_highest_checkout(uint32_t iCheckout);
  void set_label_180s(uint32_t iCount);
  void set_label_160s(uint32_t iCount);
  void set_label_140s(uint32_t iCount);
  void set_label_120s(uint32_t iCount);
  void set_label_100s(uint32_t iCount);
  void set_label_80s(uint32_t iCount);
  void set_label_60s(uint32_t iCount);
  void set_label_40s(uint32_t iCount);
  void set_label_20s(uint32_t iCount);
  void set_label_0s(uint32_t iCount);
  void set_chart(CChart *iChart1, CChart *iChart2);
  void set_text(QString iText);
  void clear_text();
  void init_leg_selector(uint32_t iNumberOfLegs);

signals:

  void signal_update_leg_history(uint32_t iIndex, CStatsWindow * iStats);

private slots:

  void on_leg_selector_current_index_changed(uint32_t iIndex);

private:

  Ui::CStatsWindow *mUi;
};

#endif  // STATSWINDOW_H
