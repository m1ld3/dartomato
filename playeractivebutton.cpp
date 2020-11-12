#include "playeractivebutton.h"

PlayerActiveButton::PlayerActiveButton(QWidget *parent) :
    QLabel(parent)
{}

void PlayerActiveButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    PlayerActiveButton::PlayerActiveButtonPressed();
}

void PlayerActiveButton::PlayerActiveButtonPressed() {
    emit signalPlayerActiveButtonPressed();
}

