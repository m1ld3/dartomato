#ifndef UNDOBUTTON_H
#define UNDOBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

class UndoButton : public QPushButton
{
    Q_OBJECT

public:
    UndoButton(const QString &text, QWidget *parent = nullptr);
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void UndoButtonPressed();
signals:
    void signalUndoButtonPressed();
};

#endif // UNDOBUTTON_H
