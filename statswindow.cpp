#include "statswindow.h"
#include "ui_statswindow.h"

CStatsWindow::CStatsWindow(QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CStatsWindow)
{
  mUi->setupUi(this);
  mUi->chartView1->setMouseTracking(true);
  mUi->chartView2->setMouseTracking(true);
  setAttribute(Qt::WA_DeleteOnClose);
  mUi->chartView1->setRubberBand( QChartView::HorizontalRubberBand);
  mUi->chartView2->setRubberBand( QChartView::HorizontalRubberBand);

  connect(mUi->legSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
  {
    leg_selector_current_index_changed_slot(index);
  });
}

CStatsWindow::~CStatsWindow()
{
  delete mUi;
}

void CStatsWindow::set_label_1dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_1dartInput->setText(avgstr);
}

void CStatsWindow::set_label_3dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_3dartInput->setText(avgstr);
}

void CStatsWindow::set_label_checkout(double iCheckout)
{
  QString checkstr = QString::number(iCheckout, 'f', 2) + "%";
  mUi->label_checkoutInput->setText(checkstr);
}

void CStatsWindow::set_label_leg_1dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_legavg1_input->setText(avgstr);
}

void CStatsWindow::set_label_leg_3dart_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_legavg3_input->setText(avgstr);
}

void CStatsWindow::set_label_best_leg(uint32_t iNumberOfDarts)
{
  mUi->label_best_leg_input->setText(QString::number(iNumberOfDarts));
}

void CStatsWindow::set_label_worst_leg(uint32_t iNumberOfDarts)
{
  mUi->label_worst_leg_input->setText(QString::number(iNumberOfDarts));
}

void CStatsWindow::set_label_leg_avg(double iAvg)
{
  QString avgstr = QString::number(iAvg, 'f', 2);
  mUi->label_leg_avg_input->setText(avgstr);
}

void CStatsWindow::set_label_checkout_attempts(QString iAttempts)
{
  mUi->label_checkout_attempts_input->setText(iAttempts);
}

void CStatsWindow::set_label_highest_checkout(uint32_t iCheckout)
{
  mUi->label_highest_checkout_input->setText(QString::number(iCheckout));
}

void CStatsWindow::set_label_180s(uint32_t iCount)
{
  mUi->label_180_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_160s(uint32_t iCount)
{
  mUi->label_160p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_140s(uint32_t iCount)
{
  mUi->label_140p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_120s(uint32_t iCount)
{
  mUi->label_120p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_100s(uint32_t iCount)
{
  mUi->label_100p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_80s(uint32_t iCount)
{
  mUi->label_80p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_60s(uint32_t iCount)
{
  mUi->label_60p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_40s(uint32_t iCount)
{
  mUi->label_40p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_20s(uint32_t iCount)
{
  mUi->label_20p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_label_0s(uint32_t iCount)
{
  mUi->label_0p_input->setText(QString::number(iCount));
}

void CStatsWindow::set_chart(CChart * iChart1, CChart * iChart2)
{
  mUi->chartView1->setChart(iChart1);
  mUi->chartView2->setChart(iChart2);
}

void CStatsWindow::set_text(QString iText)
{
  mUi->textBrowser->append(iText);
}

void CStatsWindow::clear_text()
{
  mUi->textBrowser->clear();
}

void CStatsWindow::init_leg_selector(uint32_t iNumberOfLegs)
{
  if (iNumberOfLegs == 0)
  {
    mUi->legSelector->addItem("1");
    mUi->legSelector->setCurrentIndex(0);
  }
  else
  {
    for (uint32_t i = 1; i < iNumberOfLegs + 1; i++)
    {
      mUi->legSelector->addItem(QString::number(i));
    }
    mUi->legSelector->setCurrentIndex(iNumberOfLegs - 1);
  }
}

void CStatsWindow::leg_selector_current_index_changed_slot(uint32_t iIndex)
{
  emit signal_update_leg_history(iIndex, this);
}
