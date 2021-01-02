#include "submitbutton.h"

SubmitButton::SubmitButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{

}

void SubmitButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    SubmitButton::SubmitButtonPressed();
}

void SubmitButton::SubmitButtonPressed() {
    emit signalSubmitButtonPressed();
}
