#ifndef DARTBOARDVIEW_H
#define DARTBOARDVIEW_H

#include "QGraphicsView"


class CDartBoardView : public QGraphicsView
{
  Q_OBJECT

public:

  CDartBoardView(QWidget * iParent = 0);
  void resizeEvent(QResizeEvent * iEvent) override;
  ~CDartBoardView() override = default;
};

#endif  // DARTBOARDVIEW_H
