#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

class CChart : public QChart
{
public:

  explicit CChart(QGraphicsItem * iParent = nullptr, Qt::WindowFlags iWindowFlags = {});
  ~CChart() override = default;

protected:

  bool sceneEvent(QEvent * iEvent) override;

private:

  bool gesture_event(QGestureEvent * iEvent);
};

#endif  // CHART_H
