#include "add_players_dialog.h"
#include "ui_add_players_dialog.h"
#include <QMessageBox>
#include "dartomat_mainwindow.h"

CAddPlayersDialog::CAddPlayersDialog(QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CAddPlayersDialog)
  , mpMainWindow(static_cast<CDartomatMain*>(iParent))
{
  mUi->setupUi(this);

  connect(mUi->pushButtonAdd, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_ok_clicked_slot);
  connect(mUi->pushButtonSubmit, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_submit_clicked_slot);
  connect(mUi->pushButtonCancel, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_cancel_clicked_slot);

}

CAddPlayersDialog::~CAddPlayersDialog()
{
  delete mUi;
}

void CAddPlayersDialog::push_button_ok_clicked_slot()
{
  QString name = mUi->lineEditAddPlayer->text().trimmed();
  if (!name.size()) return;
  if (!mUi->listWidgetPlayers->findItems(name, Qt::MatchExactly).empty())
  {
    QMessageBox::warning(this, "Player name already exists!", "Please choose another name.");
    mUi->lineEditAddPlayer->clear();
    return;
  }
  else
  {
    mUi->listWidgetPlayers->addItem(name);
    mNewPlayerNames.append(name);
  }

  mUi->lineEditAddPlayer->clear();
}

void CAddPlayersDialog::push_button_submit_clicked_slot()
{
  mpMainWindow->add_new_players(mNewPlayerNames);
  close();
}

void CAddPlayersDialog::push_button_cancel_clicked_slot()
{
  close();
}
