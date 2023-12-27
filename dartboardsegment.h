#ifndef DARTBOARDSEGMENT_H
#define DARTBOARDSEGMENT_H

#include <QGraphicsItem>
#include <QPainter>

class CDartBoardSegment : public QObject, public QGraphicsPathItem
{
  Q_OBJECT

public:

  CDartBoardSegment(const QPainterPath & iPath, const uint32_t iVal = 0, QString && iColor = "black", QChar iType = 's', QGraphicsPathItem * iParent = nullptr);
  ~CDartBoardSegment() override = default;

protected:

  virtual void mousePressEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * iEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * iEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void paint(QPainter * iPainter, const QStyleOptionGraphicsItem * iOption, QWidget * iWidget = nullptr) override;
  virtual QPainterPath shape() const override;
  QRectF boundingRect() const override;

private slots:

  void segment_pressed();

signals:

  void signal_segment_pressed(uint32_t & iSegmentVal, QChar & iType);

private:

  uint32_t mValue;
  QString mColor;
  bool mPressed;
  bool mHover;
  bool mDragging;
  QPainterPath mPath;
  QChar mType;
};

#endif  // DARTBOARDSEGMENT_H
