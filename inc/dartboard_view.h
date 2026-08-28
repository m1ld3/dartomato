#ifndef DARTBOARD_VIEW_H
#define DARTBOARD_VIEW_H

#include "QGraphicsView"


class CDartBoardView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit CDartBoardView(QWidget * iParent = nullptr);
  ~CDartBoardView() override = default;

protected:
  void resizeEvent(QResizeEvent * iEvent) override;
};

#endif  // DARTBOARD_VIEW_H
