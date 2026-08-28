#ifndef PLAYER_ACTIVE_BUTTON_H
#define PLAYER_ACTIVE_BUTTON_H

#include <QLabel>

class CPlayerActiveButton : public QLabel
{
  Q_OBJECT

public:
  explicit CPlayerActiveButton(QWidget * iParent = nullptr);

private:

  void player_active_button_pressed();

signals:

  void signal_player_active_button_pressed();

protected:

  void mouseReleaseEvent(QMouseEvent * iEvent) override;
};

#endif  // PLAYER_ACTIVE_BUTTON_H
