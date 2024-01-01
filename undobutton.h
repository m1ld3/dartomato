#ifndef UNDOBUTTON_H
#define UNDOBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class CUndoButton : public QPushButton
{
  Q_OBJECT

public:

  CUndoButton(const QString & iText, QWidget * iParent = nullptr);
  virtual void mouseReleaseEvent(QMouseEvent * iEvent) override;

private slots:

  void undo_button_pressed_slot();

signals:

  void signal_undo_button_pressed();
};

#endif  // UNDOBUTTON_H
