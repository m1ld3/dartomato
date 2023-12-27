#include "myqspinbox.h"
#include <QKeyEvent>
#include <QLineEdit>

CMyQSpinBox::CMyQSpinBox(QWidget * iParent)
  : QSpinBox(iParent)
{}

void CMyQSpinBox::keyPressEvent(QKeyEvent * iEvent)
{
  iEvent->ignore();
}

void CMyQSpinBox::stepBy(int iSteps)
{
  QSpinBox::stepBy(iSteps);
  lineEdit()->deselect();
}
