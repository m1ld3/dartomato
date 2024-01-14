#ifndef ADD_PLAYERS_DIALOG_H
#define ADD_PLAYERS_DIALOG_H

#include <QDialog>

class CDartomatMain;

namespace Ui
{
  class CAddPlayersDialog;
}

class CAddPlayersDialog : public QDialog
{
  Q_OBJECT

public:

  explicit CAddPlayersDialog(QWidget *parent = nullptr);
  ~CAddPlayersDialog();

private slots:

  void push_button_ok_clicked_slot();
  void push_button_submit_clicked_slot();
  void push_button_cancel_clicked_slot();

private:

  Ui::CAddPlayersDialog * mUi;
  QVector<QString> mNewPlayerNames;
  CDartomatMain * const mpMainWindow;
};

#endif  // ADD_PLAYERS_DIALOG_H
