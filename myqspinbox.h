#ifndef MYQSPINBOX_H
#define MYQSPINBOX_H

#include <QSpinBox>

class CMyQSpinBox : public QSpinBox
{
  Q_OBJECT

public:

  explicit CMyQSpinBox(QWidget * iParent = nullptr);
  void keyPressEvent(QKeyEvent * iEvent) override;
  void stepBy(int iSteps) override;
};

#endif  // MYQSPINBOX_H
