#include "dartboardsegment.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QString>

DartboardSegment::DartboardSegment(const QPainterPath &path, const int value, QString color, QChar type, QGraphicsPathItem *parent)
    : QObject(), QGraphicsPathItem(path, parent)
{
    Value = value;
    Pressed = false;
    Hover = false;
    Dragging = false;
    Color = color;
    Path = path;
    Type = type;
    setAcceptHoverEvents(true);
}

DartboardSegment::~DartboardSegment()
{}

void DartboardSegment::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!contains(mapToItem(this, event->pos())) && Pressed)
    {
    //if (Pressed)  {
        Dragging = true;
    }
    else
    {
        Dragging = false;
    }
    update();
    QGraphicsItem::mouseMoveEvent(event);
}

void DartboardSegment::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)  {
        Pressed = true;
        update();
    }
}

void DartboardSegment::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->button() == Qt::LeftButton)
   {
       Pressed = false;
       if (Dragging)
       {
           Dragging = false;
       }
       else
       {
           update();
           DartboardSegment::segmentPressed();
       }
   }
}

void DartboardSegment::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Hover = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void DartboardSegment::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Hover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void DartboardSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QBrush brush(Qt::SolidPattern);

    if (Pressed && !Dragging) {
        painter->setPen(QPen(Qt::blue,4));
        setZValue(1);
    } else {
        painter->setPen(QPen(Qt::gray,4));
        setZValue(0);
    }

    if (Hover) {
        if (Color == "black") {
            brush.setColor(QColor::fromRgb(80,80,80));
        } else if (Color == "beige") {
            brush.setColor(QColor::fromRgb(165,165,140));
        } else if (Color == "red") {
            brush.setColor(Qt::red);
        } else if (Color == "green") {
            brush.setColor(Qt::green);
        } else
            brush.setColor(Qt::white);
    } else {
        if (Color == "black") {
            brush.setColor(Qt::black);
        } else if (Color == "beige") {
            brush.setColor(QColor::fromRgb(215,215,190));
        } else if (Color == "red") {
            brush.setColor(QColor::fromRgb(180,50,0));
        } else if (Color == "green") {
            brush.setColor(Qt::darkGreen);
        } else {
            brush.setColor(Qt::white);
        }
    }
    painter->fillPath(Path,brush);
    painter->drawPath(Path);
}

void DartboardSegment::segmentPressed() {
    emit signalSegmentPressed(Value,Type);

}

QRectF DartboardSegment::boundingRect() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(4.0);
    QPainterPath boundingPath = stroker.createStroke(Path);
    return boundingPath.boundingRect();
}

