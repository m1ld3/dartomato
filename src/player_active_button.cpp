#include "player_active_button.h"

CPlayerActiveButton::CPlayerActiveButton(QWidget * iParent)
  : QLabel(iParent)
{}

void CPlayerActiveButton::mouseReleaseEvent(QMouseEvent * iEvent)
{
  Q_UNUSED(iEvent);
  CPlayerActiveButton::player_active_button_pressed();
}

void CPlayerActiveButton::player_active_button_pressed()
{
  emit signal_player_active_button_pressed();
}
