#include "submitbutton.h"

CSubmitButton::CSubmitButton(const QString & iText, QWidget * iParent)
  : QPushButton(iText, iParent)
{}

void CSubmitButton::mouseReleaseEvent(QMouseEvent * iEvent)
{
  Q_UNUSED(iEvent);
  CSubmitButton::submit_button_pressed_slot();
}

void CSubmitButton::submit_button_pressed_slot()
{
  emit signal_submit_button_pressed();
}
