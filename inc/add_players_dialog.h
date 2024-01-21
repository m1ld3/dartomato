#ifndef ADD_PLAYERS_DIALOG_H
#define ADD_PLAYERS_DIALOG_H

#include <QDialog>

class CDartomatMain;
class CGameDataHandler;
class CGameDataModel;

namespace Ui
{
  class CAddPlayersDialog;
}

class CAddPlayersDialog : public QDialog
{
  Q_OBJECT

public:

  explicit CAddPlayersDialog(CGameDataModel & iGameDataModel, QWidget * iParent = nullptr);
  ~CAddPlayersDialog();

private slots:

  void push_button_add_clicked_slot();
  void push_button_submit_clicked_slot();
  void push_button_cancel_clicked_slot();

private:

  Ui::CAddPlayersDialog * mUi;
  QVector<QString> mNewPlayerNames;
  CDartomatMain * const mMainWindow;
  CGameDataModel & mGameDataModel;
};

#endif  // ADD_PLAYERS_DIALOG_H
