#ifndef DARTBOARDVIEW_H
#define DARTBOARDVIEW_H

#include "QGraphicsView"


class CDartBoardView : public QGraphicsView
{
  Q_OBJECT

public:

  CDartBoardView(QWidget * iParent = 0);
  ~CDartBoardView() override = default;
  void resizeEvent(QResizeEvent * iEvent) override;
};

#endif  // DARTBOARDVIEW_H
