#ifndef SUBMITBUTTON_H
#define SUBMITBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class CSubmitButton : public QPushButton
{
  Q_OBJECT

public:

  CSubmitButton(const QString &text, QWidget *parent = nullptr);
  void mouseReleaseEvent(QMouseEvent *event) override;

private slots:

  void submit_button_pressed();

signals:

  void signal_submit_button_pressed();
};

#endif  // SUBMITBUTTON_H
