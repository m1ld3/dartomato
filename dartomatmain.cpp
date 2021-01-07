#include "dartomatmain.h"
#include "ui_dartomatmain.h"
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#include <QSound>
#include <QDebug>

DartomatMain::DartomatMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DartomatMain)
{
    ui->setupUi(this);
    ui->radioButton_Sin->setChecked(true);
    ui->radioButton_Dout->setChecked(true);
    ui->comboBox_game->setCurrentIndex(1);
    ui->checkBoxCutThroat->setVisible(false);
}

DartomatMain::~DartomatMain()
{
    delete ui;
}


void DartomatMain::on_pushButton_startgame_clicked()
{
    QSound *gameon = new QSound(":/resources/sounds/gameon.wav");
    gameon->play();
    int game = (ui->comboBox_game->itemText(ui->comboBox_game->currentIndex())).toInt();
    int numberofplayers = ui->comboBox_players->currentIndex() + 1;
    int sets = ui->spinBox_sets->value();
    int legs = ui->spinBox_legs->value();
    bool singleIn = ui->radioButton_Sin->isChecked();
    bool singleOut = ui->radioButton_Sout->isChecked();
    bool doubleIn = ui->radioButton_Din->isChecked();
    bool doubleOut = ui->radioButton_Dout->isChecked();
    bool masterIn = ui->radioButton_Min->isChecked();
    bool masterOut = ui->radioButton_Mout->isChecked();
    bool cutthroat = ui->checkBoxCutThroat->isChecked();
    bool offensive = ui->checkBoxOffensive->isChecked();
    if (game > 0)
    {
        x01MainWindow = new X01MainWindow(this, numberofplayers, game, sets, legs, singleIn, singleOut, doubleIn, doubleOut, masterIn, masterOut, offensive);
        x01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
        x01MainWindow->show();
    }
    else
    {
        cricketMainWindow = new CricketMainWindow(this, numberofplayers, sets, legs, cutthroat, offensive);
        cricketMainWindow->setAttribute(Qt::WA_DeleteOnClose);
        cricketMainWindow->show();
    }
    if (gameon->isFinished()) delete gameon;
}

void DartomatMain::on_comboBox_game_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Cricket")
    {
        ui->radioButton_Sin->setVisible(false);
        ui->radioButton_Din->setVisible(false);
        ui->radioButton_Min->setVisible(false);
        ui->radioButton_Sout->setVisible(false);
        ui->radioButton_Dout->setVisible(false);
        ui->radioButton_Mout->setVisible(false);
        ui->checkBoxCutThroat->setVisible(true);
    }
    else
    {
        ui->radioButton_Sin->setVisible(true);
        ui->radioButton_Din->setVisible(true);
        ui->radioButton_Min->setVisible(true);
        ui->radioButton_Sout->setVisible(true);
        ui->radioButton_Dout->setVisible(true);
        ui->radioButton_Mout->setVisible(true);
        ui->checkBoxCutThroat->setVisible(false);
    }
}
