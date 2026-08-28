#include "spinbox.h"
#include <QKeyEvent>
#include <QLineEdit>

CSpinBox::CSpinBox(QWidget * iParent)
  : QSpinBox(iParent)
{}

void CSpinBox::keyPressEvent(QKeyEvent * iEvent)
{
  iEvent->ignore();
}

void CSpinBox::stepBy(const int iSteps)
{
  QSpinBox::stepBy(iSteps);
  lineEdit()->deselect();
}
