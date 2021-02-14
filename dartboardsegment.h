#ifndef DARTBOARDSEGMENT_H
#define DARTBOARDSEGMENT_H

#include <QGraphicsItem>
#include <QPainter>

class DartboardSegment : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    DartboardSegment(const QPainterPath &path, const int value = 0, QString clr = "black", QChar type = 's', QGraphicsPathItem *parent = nullptr);
    ~DartboardSegment();
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual QPainterPath shape() const override;
    QRectF boundingRect() const override;
private slots:
    void segmentPressed();
signals:
    void signalSegmentPressed(int &segmentval, QChar &type);
private:
    int Value;
    QString Color;
    bool Pressed;
    bool Hover;
    bool Dragging;
    QPainterPath Path;
    QChar Type;
};


#endif // DARTBOARDSEGMENT_H
