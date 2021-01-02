#include "dartboardview.h"

DartBoardView::DartBoardView(QWidget * parent)
    : QGraphicsView(parent)
{}



void DartBoardView::resizeEvent(QResizeEvent *event)
{
    fitInView(0, 0, 800, 800, Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}

DartBoardView::~DartBoardView()
{}
