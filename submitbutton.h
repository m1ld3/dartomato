#ifndef SUBMITBUTTON_H
#define SUBMITBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class CSubmitButton : public QPushButton
{
  Q_OBJECT

public:

  CSubmitButton(const QString & iText, QWidget * iParent = nullptr);
  void mouseReleaseEvent(QMouseEvent * iEvent) override;

private slots:

  void submit_button_pressed_slot();

signals:

  void signal_submit_button_pressed();
};

#endif  // SUBMITBUTTON_H
