#include "myqspinbox.h"
#include <QKeyEvent>
#include <QLineEdit>

MyQSpinBox::MyQSpinBox(QWidget *parent)
    : QSpinBox(parent)
{}

void MyQSpinBox::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void MyQSpinBox::stepBy(int steps)
{
    QSpinBox::stepBy(steps);
    lineEdit()->deselect();
}
