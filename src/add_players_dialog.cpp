#include "add_players_dialog.h"
#include "ui_add_players_dialog.h"
#include <QMessageBox>
#include "dartomat_mainwindow.h"
#include "player_list_model.h"
#include <QErrorMessage>

CAddPlayersDialog::CAddPlayersDialog(CPlayerListModel & iPlayerListModel, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CAddPlayersDialog)
  , mMainWindow(static_cast<CDartomatMain*>(iParent))
  , mPlayerListModel(iPlayerListModel)
{
  mUi->setupUi(this);
  mUi->listViewPlayers->setModel(&mPlayerListModel);
  mUi->listViewPlayers->setSelectionMode(QAbstractItemView::MultiSelection);
  mUi->deleteButton->setIcon(QIcon(":/resources/img/delete_icon.svg"));

  connect(mUi->pushButtonAdd, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_add_clicked_slot);
  connect(mUi->pushButtonSubmit, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_submit_clicked_slot);
  connect(mUi->pushButtonCancel, &QPushButton::clicked, this, &CAddPlayersDialog::push_button_cancel_clicked_slot);
  connect(mUi->deleteButton, &QToolButton::clicked, this, &CAddPlayersDialog::delete_player_data);
}

CAddPlayersDialog::~CAddPlayersDialog()
{
  delete mUi;
}

void CAddPlayersDialog::push_button_add_clicked_slot()
{
  QString newPlayerName = mUi->lineEditAddPlayer->text().trimmed();
  if (newPlayerName.isEmpty()) return;

  if (!mPlayerListModel.add_player(newPlayerName))
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
    return;
  }

  mMainWindow->handle_selected_players(selectedPlayers);

  close();
}

void CAddPlayersDialog::push_button_cancel_clicked_slot()
{
  close();
}

void CAddPlayersDialog::delete_player_data()
{
  QModelIndexList selectedIndexes = mUi->listViewPlayers->selectionModel()->selectedIndexes();
  QStringList selectedPlayers;

  for (const auto & idx : selectedIndexes)
  {
    selectedPlayers.append(idx.data().toString());
  }

  if (selectedPlayers.isEmpty())
  {
    QMessageBox::warning(this, "No players selected!", "Please select at least one player for deletion.");
    return;
  }

  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Delete Player Data",
                                                             "Do you really want to delete the selected players? \nThis cannot be undone!",
                                                             QMessageBox::Yes | QMessageBox::No);
  if (resBtn == QMessageBox::Yes)
  {
    if (!mMainWindow->delete_player_data(selectedPlayers))
    {
      auto error = QErrorMessage(this);
      error.showMessage("Data could not be deleted!");
      return;
    }
    mPlayerListModel.remove_players(selectedPlayers);
  }
}
