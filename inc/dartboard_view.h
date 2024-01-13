#ifndef DARTBOARD_VIEW_H
#define DARTBOARD_VIEW_H

#include "QGraphicsView"


class CDartBoardView : public QGraphicsView
{
  Q_OBJECT

public:

  CDartBoardView(QWidget * iParent = 0);
  ~CDartBoardView() override = default;
  void resizeEvent(QResizeEvent * iEvent) override;
};

#endif  // DARTBOARD_VIEW_H
