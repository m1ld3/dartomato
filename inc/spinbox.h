#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>

class CSpinBox : public QSpinBox
{
  Q_OBJECT

public:

  explicit CSpinBox(QWidget * iParent = nullptr);
  void keyPressEvent(QKeyEvent * iEvent) override;
  void stepBy(int iSteps) override;
};

#endif  // SPINBOX_H
