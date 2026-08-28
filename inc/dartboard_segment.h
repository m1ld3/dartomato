#ifndef DARTBOARD_SEGMENT_H
#define DARTBOARD_SEGMENT_H

#include <QGraphicsItem>
#include <QPainter>

class CDartBoard;

class CDartBoardSegment : public QObject, public QGraphicsPathItem
{
  Q_OBJECT

public:

  CDartBoardSegment(CDartBoard * iDartBoard, const QPainterPath & iPath, uint32_t iVal = 0,
                    QString && iColor = "black", QChar iType = 's',
                    QGraphicsPathItem * iParent = nullptr);
  ~CDartBoardSegment() override = default;
  void set_value(uint32_t iVal);

  void paint(QPainter * iPainter, const QStyleOptionGraphicsItem * iOption, QWidget * iWidget) override;
  [[nodiscard]] QPainterPath shape() const override;
  [[nodiscard]] QRectF boundingRect() const override;

protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * iEvent) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * iEvent) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent * iEvent) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent * iEvent) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent * iEvent) override;

private:

  uint32_t mValue;
  QString mColor;
  QPainterPath mPath;
  QChar mType;
  bool mPressed = false;
  bool mHover = false;
  bool mDragging = false;
  CDartBoard * mDartBoard;
};

#endif  // DARTBOARD_SEGMENT_H
