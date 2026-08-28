#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>

class CSpinBox : public QSpinBox
{
  Q_OBJECT

public:

  explicit CSpinBox(QWidget * iParent = nullptr);
  void stepBy(int iSteps) override;

protected:
  void keyPressEvent(QKeyEvent * iEvent) override;
};

#endif  // SPINBOX_H
