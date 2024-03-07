#include "game_history_dialog.h"
#include "ui_game_history_dialog.h"
#include "game_history_table_view.h"
#include "game_results_model.h"
#include <QMessageBox>
#include <QErrorMessage>
#include "stats_window_x01.h"
#include "stats_window_cricket.h"
#include "dartomat_mainwindow.h"

CGameHistoryDialog::CGameHistoryDialog(CGameDataHandler & iGameDataHandler, CDartomatMain * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CGameHistoryDialog)
  , mGameDataHandler(iGameDataHandler)
  , mGameData(iGameDataHandler.get_game_data())
  , mGameHistoryModel(new CGameHistoryModel(mGameData, this))
  , mMainWindow(iParent)
{
  mUi->setupUi(this);
  mUi->tableViewGameHistory->setModel(mGameHistoryModel);
  mUi->tableViewGameHistory->resizeColumnsToContents();
  connect(mUi->tableViewGameHistory, &CGameHistoryTableView::signal_row_double_clicked_or_pressed, this, &CGameHistoryDialog::switch_to_game_results_page);
  connect(mUi->backButtonPage1, &QToolButton::clicked, this, [this]{ mUi->stackedWidget->setCurrentIndex(0); });
  connect(mUi->deleteButton, &QToolButton::clicked, this, &CGameHistoryDialog::delete_current_row);
  connect(mUi->statsButton, &QToolButton::clicked, this, &CGameHistoryDialog::show_stats);
  connect(mUi->pushButtonPlayGame, &QPushButton::clicked, this, &CGameHistoryDialog::start_game);
  mUi->stackedWidget->setCurrentIndex(0);
  mUi->deleteButton->setIcon(QIcon(":/resources/img/delete_icon.svg"));
  mUi->statsButton->setIcon(QIcon(":/resources/img/stats_icon.svg"));
}

CGameHistoryDialog::~CGameHistoryDialog()
{
  delete mUi;
  delete mGameHistoryModel;
}

void CGameHistoryDialog::switch_to_game_results_page(int iRowIdx)
{
  mGameResultsIdx = iRowIdx;
  QPointer<CGameResultsModel> gameResultsModel = new CGameResultsModel(mGameData.at(mGameResultsIdx), this);
  mUi->tableViewRanking->setModel(gameResultsModel);
  if (!mGameData.at(mGameResultsIdx).Finished) mUi->pushButtonPlayGame->setText("Resume Game");
  else                                         mUi->pushButtonPlayGame->setText("Play again");
  mUi->stackedWidget->setCurrentIndex(1);
}

void CGameHistoryDialog::delete_current_row()
{
  if (mGameData.size() == 0)
  {
    QMessageBox::warning(this, "No game data.", "Nothing to delete.");
    return;
  }

  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Delete Game.",
                                                             tr("Do you want delete to this game?"),
                                                             QMessageBox::Yes | QMessageBox::No);
  if (resBtn == QMessageBox::Yes)
  {
    QItemSelectionModel * model = mUi->tableViewGameHistory->selectionModel();

    if (model->hasSelection())
    {
      int rowIdx = model->currentIndex().row();
      if (!mGameDataHandler.delete_game_from_db(mGameData.at(rowIdx).TimeStamp))
      {
        auto error = QErrorMessage(this);
        error.showMessage("Game could not be deleted!");
        return;
      }
      mGameHistoryModel->delete_row(rowIdx);
    }
    else
    {
      QMessageBox::warning(this, "No game selected.", "Please select a row to delete.");
    }
  }
}

void CGameHistoryDialog::show_stats()
{
  QItemSelectionModel * model = mUi->tableViewRanking->selectionModel();

  if (model->hasSelection())
  {
    int rowIdx = model->currentIndex().row();
    auto gameDataX01 = mGameData.at(mGameResultsIdx).GameDataX01.at(rowIdx);
    auto gameDataCricket = mGameData.at(mGameResultsIdx).GameDataCricket.at(rowIdx);
    if (gameDataX01.size() > 0)
    {
      auto stats = CStatsWindow::create(gameDataX01.back(), this);
      stats->setAttribute(Qt::WA_DeleteOnClose);
      stats->setModal(true);
      stats->show();
    }
    else
    {
      auto stats = CStatsWindow::create(gameDataCricket.back(), this);
      stats->setAttribute(Qt::WA_DeleteOnClose);
      stats->setModal(true);
      stats->show();
    }
  }
  else
  {
    QMessageBox::warning(this, "No player selected.", "Please select a player.");
    return;
    }
}

void CGameHistoryDialog::start_game()
{
  if (mGameData.at(mGameResultsIdx).Finished)
  {
    mMainWindow->start_game(mGameData.at(mGameResultsIdx).Settings);
  }
  else
  {
    mMainWindow->resume_game(mGameData.at(mGameResultsIdx));
  }
}


