#include "undobutton.h"

CUndoButton::CUndoButton(const QString & iText, QWidget * iParent)
  : QPushButton(iText, iParent)
{}

void CUndoButton::mouseReleaseEvent(QMouseEvent * iEvent)
{
  Q_UNUSED(iEvent);
  update();
  CUndoButton::undo_button_pressed_slot();
}

void CUndoButton::undo_button_pressed_slot()
{
  emit signal_undo_button_pressed();
}
