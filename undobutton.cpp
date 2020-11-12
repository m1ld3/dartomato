#include "undobutton.h"

UndoButton::UndoButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{

}

void UndoButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    update();
    UndoButton::UndoButtonPressed();
}

void UndoButton::UndoButtonPressed() {
    emit signalUndoButtonPressed();
}
