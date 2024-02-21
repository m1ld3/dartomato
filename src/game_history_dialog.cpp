#include "game_history_dialog.h"
#include "ui_game_history_dialog.h"

CGameHistoryDialog::CGameHistoryDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent)
  : QDialog(iParent)
  , mUi(new Ui::CGameHistoryDialog)
  , mGameDataHandler(iGameDataHandler)
{
  mUi->setupUi(this);
}

CGameHistoryDialog::~CGameHistoryDialog()
{
  delete mUi;
}
