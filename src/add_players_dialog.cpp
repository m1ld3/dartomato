#include "add_players_dialog.h"
#include "ui_add_players_dialog.h"
#include <QMessageBox>
#include "dartomat_mainwindow.h"
#include "game_data_model.h"

CAddPlayersDialog::CAddPlayersDialog(CGameDataModel & iGameDataModel, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CAddPlayersDialog)
  , mpMainWindow(static_cast<CDartomatMain*>(iParent))
  , mGameDataModel(iGameDataModel)
{
  mUi->setupUi(this);
  mUi->listViewPlayers->setModel(&mGameDataModel);
  mUi->listViewPlayers->setSelectionMode(QAbstractItemView::MultiSelection);

  connect(mUi->pushButtonAdd, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_add_clicked_slot);
  connect(mUi->pushButtonSubmit, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_submit_clicked_slot);
  connect(mUi->pushButtonCancel, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_cancel_clicked_slot);
}

CAddPlayersDialog::~CAddPlayersDialog()
{
  delete mUi;
}

void CAddPlayersDialog::push_button_add_clicked_slot()
{
  QString newPlayerName = mUi->lineEditAddPlayer->text().trimmed();
  if (newPlayerName.isEmpty()) return;

  if (!mGameDataModel.add_player(newPlayerName))
  {
    QMessageBox::warning(this, "Player name already exists!", "Please choose another name.");
  }

  mUi->lineEditAddPlayer->clear();
}

void CAddPlayersDialog::push_button_submit_clicked_slot()
{
  QModelIndexList selectedIndexes = mUi->listViewPlayers->selectionModel()->selectedIndexes();
  QStringList selectedPlayers;


  for (const auto & idx : selectedIndexes)
  {
    selectedPlayers.append(idx.data().toString());
  }

  if (selectedPlayers.size() > 8)
  {
    QMessageBox::warning(this, "Too many players selected!", "You can only select eight players.");
    return;
  }
  if (selectedPlayers.isEmpty())
  {
    QMessageBox::warning(this, "No players selected!", "Please select at least one player.");
  }

  mpMainWindow->handle_selected_players(selectedPlayers);

  close();
}

void CAddPlayersDialog::push_button_cancel_clicked_slot()
{
  close();
}
