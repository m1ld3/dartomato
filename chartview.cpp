#include "chartview.h"
#include <QtGui/QMouseEvent>

CChartView::CChartView(QWidget *parent)
  : QChartView(parent)
{
  setRubberBand(QChartView::HorizontalRubberBand);
}

void CChartView::wheelEvent(QWheelEvent * iEvent)
{
  int numDegrees = iEvent->angleDelta().y() / 8;
  int numSteps = numDegrees / 15;
  chart()->zoom(numSteps);
  QGraphicsView::wheelEvent(iEvent);
}

void CChartView::keyPressEvent(QKeyEvent * iEvent)
{
  switch (iEvent->key())
  {
  case Qt::Key_Plus:
    chart()->zoomIn();
    break;
  case Qt::Key_Minus:
    chart()->zoomOut();
    break;
  case Qt::Key_Left:
    chart()->scroll(-10, 0);
    break;
  case Qt::Key_Right:
    chart()->scroll(10, 0);
    break;
  case Qt::Key_Up:
    chart()->scroll(0, 10);
    break;
  case Qt::Key_Down:
    chart()->scroll(0, -10);
    break;
  default:
    QGraphicsView::keyPressEvent(iEvent);
    break;
  }
}
