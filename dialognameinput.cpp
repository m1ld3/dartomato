#include "ui_dialognameinput.h"
#include <QString>
#include <QDebug>
#include "groupbox_player.h"
#include "dialognameinput.h"

DialogNameinput::DialogNameinput(QWidget *parent, QString text) :
    QDialog(parent),
    ui(new Ui::DialogNameinput)
{
    ui->setupUi(this);
    ui->lineEdit_name->setText(text);
    ui->lineEdit_name->selectAll();
    setAttribute(Qt::WA_DeleteOnClose);
}

DialogNameinput::~DialogNameinput()
{
    delete ui;
}

void DialogNameinput::on_pushButton_ok_clicked()
{
    QString text = ui->lineEdit_name->text();
    emit okButtonClicked(text);
}
