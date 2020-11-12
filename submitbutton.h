#ifndef SUBMITBUTTON_H
#define SUBMITBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class SubmitButton : public QPushButton
{
    Q_OBJECT

public:
    SubmitButton(const QString &text, QWidget *parent = nullptr);
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void SubmitButtonPressed();
signals:
    void signalSubmitButtonPressed();
};

#endif // SUBMITBUTTON_H
