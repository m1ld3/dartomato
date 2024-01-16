#include "select_players_dialog.h"
#include "ui_select_players_dialog.h"

CSelectPlayersDialog::CSelectPlayersDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CSelectPlayersDialog)
{
  ui->setupUi(this);
}

CSelectPlayersDialog::~CSelectPlayersDialog()
{
  delete ui;
}
