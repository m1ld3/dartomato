#include "game_history_dialog.h"
#include "ui_game_history_dialog.h"
#include "game_history_table_view.h"

CGameHistoryDialog::CGameHistoryDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CGameHistoryDialog)
  , mGameDataHandler(iGameDataHandler)
  , mGameHistoryModel(new CGameHistoryModel(iGameDataHandler, this))
{
  mUi->setupUi(this);
  mUi->tableViewGameHistory->setModel(mGameHistoryModel);
  mUi->tableViewGameHistory->resizeColumnsToContents();
  connect(mUi->tableViewGameHistory, &CGameHistoryTableView::signal_row_double_clicked_or_pressed, this, &CGameHistoryDialog::switch_to_game_results_page);
}

CGameHistoryDialog::~CGameHistoryDialog()
{
  delete mUi;
  delete mGameHistoryModel;
}

void CGameHistoryDialog::switch_to_game_results_page(int iRowIdx)
{
  todo: find mGameData.at(iRowIdx) and create new CGameResultsModel and setModel of
      tableViewGameResults
      switch to game results page
      todo, store gamedata in this class and only reference it from the gamehistory and game results model!
}
