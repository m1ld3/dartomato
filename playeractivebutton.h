#ifndef PLAYERACTIVEBUTTON_H
#define PLAYERACTIVEBUTTON_H

#include <QLabel>

class PlayerActiveButton : public QLabel
{
    Q_OBJECT
public:
    PlayerActiveButton(QWidget *parent = nullptr);
    void PlayerActiveButtonPressed();

signals:
    void signalPlayerActiveButtonPressed();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // PLAYERACTIVEBUTTON_H
