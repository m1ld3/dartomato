#include "dartboardsegment.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QString>

CDartBoardSegment::CDartBoardSegment(const QPainterPath & iPath, const uint32_t iVal, QString && iColor, QChar iType, QGraphicsPathItem * iParent)
  : QObject()
  , QGraphicsPathItem(iPath, iParent)
  , mValue(iVal)
  , mColor(iColor)
  , mPath(iPath)
  , mType(iType)
{
  setAcceptHoverEvents(true);
}

void CDartBoardSegment::mouseMoveEvent(QGraphicsSceneMouseEvent * iEvent)
{
  if (!contains(mapToItem(this, iEvent->pos())) && mPressed)
  {
    mDragging = true;
  }
  else
  {
    mDragging = false;
  }
  update();
  QGraphicsItem::mouseMoveEvent(iEvent);
}

void CDartBoardSegment::mousePressEvent(QGraphicsSceneMouseEvent * iEvent)
{
  if (iEvent->button() == Qt::LeftButton)
  {
    mPressed = true;
    update();
  }
}

void CDartBoardSegment::mouseReleaseEvent(QGraphicsSceneMouseEvent * iEvent)
{
  if (iEvent->button() == Qt::LeftButton)
  {
    mPressed = false;
    if (mDragging)
    {
      mDragging = false;
    }
    else
    {
      update();
      CDartBoardSegment::segment_pressed_slot();
    }
  }
}

void CDartBoardSegment::hoverEnterEvent(QGraphicsSceneHoverEvent * iEvent)
{
  mHover = true;
  update();
  QGraphicsItem::hoverEnterEvent(iEvent);
}

void CDartBoardSegment::hoverLeaveEvent(QGraphicsSceneHoverEvent * iEvent)
{
  mHover = false;
  update();
  QGraphicsItem::hoverLeaveEvent(iEvent);
}

void CDartBoardSegment::paint(QPainter * iPainter, const QStyleOptionGraphicsItem * iOption, QWidget * iWidget)
{
  Q_UNUSED(iOption);
  Q_UNUSED(iWidget);
  QBrush brush(Qt::SolidPattern);

  if (mPressed && !mDragging)
  {
    iPainter->setPen(QPen(Qt::blue, 4));
    setZValue(1);
  } else
  {
    iPainter->setPen(QPen(Qt::gray, 4));
    setZValue(0);
  }

  if (mHover)
  {
    if (mColor == "black")       brush.setColor(QColor::fromRgb(80,80,80));
    else if (mColor == "beige")  brush.setColor(QColor::fromRgb(165,165,140));
    else if (mColor == "red")    brush.setColor(Qt::red);
    else if (mColor == "green")  brush.setColor(Qt::green);
    else                         brush.setColor(Qt::white);
  }
  else
  {
    if (mColor == "black")       brush.setColor(Qt::black);
    else if (mColor == "beige")  brush.setColor(QColor::fromRgb(215,215,190));
    else if (mColor == "red")    brush.setColor(QColor::fromRgb(180,50,0));
    else if (mColor == "green")  brush.setColor(Qt::darkGreen);
    else                         brush.setColor(Qt::white);
  }

  iPainter->fillPath(mPath,brush);
  iPainter->drawPath(mPath);
}

QPainterPath CDartBoardSegment::shape() const
{
  return mPath;
}

void CDartBoardSegment::segment_pressed_slot()
{
  emit signal_segment_pressed(mValue, mType);
}

QRectF CDartBoardSegment::boundingRect() const
{
  QPainterPathStroker stroker;
  stroker.setWidth(4.0);
  QPainterPath boundingPath = stroker.createStroke(mPath);
  return boundingPath.boundingRect();
}

void CDartBoardSegment::set_value(const uint32_t iVal)
{
  mValue = iVal;
}

