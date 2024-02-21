#ifndef GAME_HISTORY_DIALOG_H
#define GAME_HISTORY_DIALOG_H

#include <QDialog>
#include "game_data_handler.h"

namespace Ui
{
  class CGameHistoryDialog;
}

class CGameHistoryDialog : public QDialog
{
  Q_OBJECT

public:

  explicit CGameHistoryDialog(CGameDataHandler & iGameDataHandler, QWidget * iParent = nullptr);
  ~CGameHistoryDialog();

private:

  Ui::CGameHistoryDialog * mUi;
  CGameDataHandler & mGameDataHandler;
};

#endif  // GAME_HISTORY_DIALOG_H
