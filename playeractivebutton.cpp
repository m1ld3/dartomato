#include "playeractivebutton.h"

CPlayerActiveButton::CPlayerActiveButton(QWidget *parent) :
    QLabel(parent)
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

