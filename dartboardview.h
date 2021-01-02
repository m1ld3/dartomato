#ifndef DARTBOARDVIEW_H
#define DARTBOARDVIEW_H

#include "QGraphicsView"


class DartBoardView : public QGraphicsView
{
    Q_OBJECT
public:
    DartBoardView(QWidget * parent = 0);
    void resizeEvent(QResizeEvent *event) override;
    ~DartBoardView();
};

#endif // DARTBOARDVIEW_H
