#ifndef DIALOGNAMEINPUT_H
#define DIALOGNAMEINPUT_H

#include <QDialog>

namespace Ui {
class DialogNameinput;
}

class DialogNameinput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNameinput(QWidget *parent = nullptr, QString text = "");
    ~DialogNameinput();

private slots:
    void on_pushButton_ok_clicked();

signals:
    void okButtonClicked(QString& name);

private:
    Ui::DialogNameinput *ui;
};

#endif // DIALOGNAMEINPUT_H
