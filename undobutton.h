#ifndef UNDOBUTTON_H
#define UNDOBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class CUndoButton : public QPushButton
{
  Q_OBJECT

public:

  CUndoButton(const QString &text, QWidget *parent = nullptr);
  virtual void mouseReleaseEvent(QMouseEvent *event) override;

private slots:

  void undo_button_pressed();

signals:

  void signal_undo_button_pressed();
};

#endif  // UNDOBUTTON_H
