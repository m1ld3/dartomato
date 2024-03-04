#ifndef GAME_HISTORY_DIALOG_H
#define GAME_HISTORY_DIALOG_H

#include <QDialog>
#include "game_data_handler.h"
#include "game_history_model.h"
#include "QItemSelectionModel"

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

private slots:

  void switch_to_game_results_page(int iRowIdx);
  void delete_current_row();
  void show_stats();

private:

  Ui::CGameHistoryDialog * mUi;
  CGameDataHandler & mGameDataHandler;
  QVector<CGameDataHandler::SGameData> mGameData;
  CGameHistoryModel * mGameHistoryModel;
  uint32_t mGameResultsIdx = 0;
};

#endif  // GAME_HISTORY_DIALOG_H