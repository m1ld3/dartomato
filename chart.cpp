#include <QtCharts/QChart>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include "chart.h"

CChart::CChart(QGraphicsItem * iParent, Qt::WindowFlags iWindowFlags)
  : QChart(QChart::ChartTypeCartesian, iParent, iWindowFlags)
{
  // Seems that QGraphicsView (QChartView) does not grab gestures.
  // They can only be grabbed here in the QGraphicsWidget (QChart).
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::PinchGesture);
}

bool CChart::sceneEvent(QEvent * iEvent)
{
  if (iEvent->type() == QEvent::Gesture) return gesture_event(static_cast<QGestureEvent*>(iEvent));
  return QChart::event(iEvent);
}

bool CChart::gesture_event(QGestureEvent * iEvent)
{
  if (QGesture * gesture = iEvent->gesture(Qt::PanGesture))
  {
    QPanGesture * pan = static_cast<QPanGesture*>(gesture);
    QChart::scroll(-(pan->delta().x()), pan->delta().y());
  }

  if (QGesture * gesture = iEvent->gesture(Qt::PinchGesture))
  {
    QPinchGesture * pinch = static_cast<QPinchGesture*>(gesture);
    if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged) QChart::zoom(pinch->scaleFactor());
  }

  return true;
}
