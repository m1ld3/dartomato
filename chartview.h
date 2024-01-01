#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>


class CChartView : public QChartView
{
public:

  CChartView(QWidget * iParent = 0);

protected:

  void wheelEvent(QWheelEvent * iEvent) override;
  void keyPressEvent(QKeyEvent * iEvent) override;
};

#endif
