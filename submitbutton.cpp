#include "submitbutton.h"

CSubmitButton::CSubmitButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{

}

void CSubmitButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    CSubmitButton::submit_button_pressed();
}

void CSubmitButton::submit_button_pressed() {
    emit signal_submit_button_pressed();
}
