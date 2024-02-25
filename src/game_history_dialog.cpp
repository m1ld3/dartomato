#include "game_history_dialog.h"
#include "ui_game_history_dialog.h"

CGameHistoryDialog::CGameHistoryDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CGameHistoryDialog)
  , mGameHistoryModel(new CGameHistoryModel(iGameDataHandler, this))
{
  mUi->setupUi(this);
  mUi->tableView->setModel(mGameHistoryModel);
  mUi->tableView->resizeColumnsToContents();
}

CGameHistoryDialog::~CGameHistoryDialog()
{
  delete mUi;
  delete mGameHistoryModel;
}
