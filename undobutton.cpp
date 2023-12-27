#include "undobutton.h"

CUndoButton::CUndoButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{

}

void CUndoButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    update();
    CUndoButton::undo_button_pressed();
}

void CUndoButton::undo_button_pressed() {
    emit signal_undo_button_pressed();
}
