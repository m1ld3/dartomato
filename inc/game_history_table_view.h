#ifndef CGAMEHISTORYTABLEVIEW_H
#define CGAMEHISTORYTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>

class CGameHistoryTableView : public QTableView
{
  Q_OBJECT

public:
  explicit CGameHistoryTableView(QWidget * iParent)
    : QTableView(iParent)
  {}

protected:
  void mouseDoubleClickEvent(QMouseEvent * iEvent) override
  {
    if (const QModelIndex index = indexAt(iEvent->pos()); index.isValid())
    {
      emit signal_row_double_clicked_or_pressed(index.row());
    }
    QTableView::mouseDoubleClickEvent(iEvent);
  }

  void keyPressEvent(QKeyEvent * iEvent) override
  {
    if (iEvent->key() == Qt::Key_Enter || iEvent->key() == Qt::Key_Return)
    {
      if (const QModelIndex index = currentIndex(); index.isValid())
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
