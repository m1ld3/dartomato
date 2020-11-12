#ifndef MYQSPINBOX_H
#define MYQSPINBOX_H

#include <QSpinBox>

class MyQSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit MyQSpinBox(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void stepBy(int steps) override;
};

#endif // MYQSPINBOX_H
