#include "game_history_dialog.h"
#include "ui_game_history_dialog.h"
#include "game_history_table_view.h"
#include "game_results_model.h"

CGameHistoryDialog::CGameHistoryDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CGameHistoryDialog)
  , mGameDataHandler(iGameDataHandler)
  , mGameData(iGameDataHandler.get_game_data())
  , mGameHistoryModel(new CGameHistoryModel(mGameData, this))
{
  mUi->setupUi(this);
  mUi->tableViewGameHistory->setModel(mGameHistoryModel);
  mUi->tableViewGameHistory->resizeColumnsToContents();
  connect(mUi->tableViewGameHistory, &CGameHistoryTableView::signal_row_double_clicked_or_pressed, this, &CGameHistoryDialog::switch_to_game_results_page);
  connect(mUi->backButtonPage1, &QToolButton::clicked, this, [this]{ mUi->stackedWidget->setCurrentIndex(0); });
  mUi->stackedWidget->setCurrentIndex(0);
}

CGameHistoryDialog::~CGameHistoryDialog()
{
  delete mUi;
  delete mGameHistoryModel;
}

void CGameHistoryDialog::switch_to_game_results_page(int iRowIdx)
{
  QPointer<CGameResultsModel> gameResultsModel = new CGameResultsModel(mGameData.at(iRowIdx), this);
  mUi->tableViewRanking->setModel(gameResultsModel);
  mUi->stackedWidget->setCurrentIndex(1);
}
