#include "dartboard_view.h"

CDartBoardView::CDartBoardView(QWidget * iParent)
  : QGraphicsView(iParent)
{}

void CDartBoardView::resizeEvent(QResizeEvent * iEvent)
{
  fitInView(0, 0, 800, 800, Qt::KeepAspectRatio);
  QGraphicsView::resizeEvent(iEvent);
}

