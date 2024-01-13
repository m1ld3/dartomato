#ifndef DARTBOARD_SEGMENT_H
#define DARTBOARD_SEGMENT_H

#include <QGraphicsItem>
#include <QPainter>

class CDartBoard;

class CDartBoardSegment : public QObject, public QGraphicsPathItem
{
  Q_OBJECT

public:

  CDartBoardSegment(CDartBoard * ipDartBoard, const QPainterPath & iPath, const uint32_t iVal = 0,
                    QString && iColor = "black", QChar iType = 's',
                    QGraphicsPathItem * iParent = nullptr);
  ~CDartBoardSegment() override = default;
  void set_value(const uint32_t iVal);

protected:

  virtual void mousePressEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * iEvent) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * iEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * iEvent) override;
  virtual void paint(QPainter * iPainter, const QStyleOptionGraphicsItem * iOption, QWidget * iWidget = nullptr) override;
  virtual QPainterPath shape() const override;
  QRectF boundingRect() const override;

private:

  uint32_t mValue;
  QString mColor;
  QPainterPath mPath;
  QChar mType;
  bool mPressed = false;
  bool mHover = false;
  bool mDragging = false;
  CDartBoard * mpDartBoard;
};

#endif  // DARTBOARD_SEGMENT_H
