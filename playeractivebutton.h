#ifndef PLAYERACTIVEBUTTON_H
#define PLAYERACTIVEBUTTON_H

#include <QLabel>

class CPlayerActiveButton : public QLabel
{
  Q_OBJECT

public:

  CPlayerActiveButton(QWidget * iParent = nullptr);

private:

  void player_active_button_pressed();

signals:

  void signal_player_active_button_pressed();

protected:

  void mouseReleaseEvent(QMouseEvent * iEvent) override;
};

#endif  // PLAYERACTIVEBUTTON_H
