#ifndef CGAMEHISTORYTABLEVIEW_H
#define CGAMEHISTORYTABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>
#include <QKeyEvent>

class CGameHistoryTableView : public QTableView
{
  Q_OBJECT

public:

  CGameHistoryTableView(QWidget * iParent)
    : QTableView(iParent)
  {}

  void mouseDoubleClickEvent(QMouseEvent * iEvent) override
  {
    QModelIndex index = indexAt(iEvent->pos());
    if (index.isValid())
    {
      emit signal_row_double_clicked_or_pressed(index.row());
    }
    QTableView::mouseDoubleClickEvent(iEvent);
  }

  void keyPressEvent(QKeyEvent * iEvent) override
  {
    if (iEvent->key() == Qt::Key_Enter || iEvent->key() == Qt::Key_Return)
    {
      QModelIndex index = currentIndex();
      if (index.isValid())
      {
        emit signal_row_double_clicked_or_pressed(index.row());
      }
    }
    else
    {
      QTableView::keyPressEvent(iEvent);
    }
  }

signals:

  void signal_row_double_clicked_or_pressed(int iRowIdx);
};

#endif // CGAMEHISTORYTABLEVIEW_H
