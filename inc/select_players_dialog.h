#ifndef SELECT_PLAYERS_DIALOG_H
#define SELECT_PLAYERS_DIALOG_H

#include <QDialog>

namespace Ui {
  class CSelectPlayersDialog;
}

class CSelectPlayersDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CSelectPlayersDialog(QWidget *parent = nullptr);
  ~CSelectPlayersDialog();

private:
  Ui::CSelectPlayersDialog *ui;
};

#endif // SELECT_PLAYERS_DIALOG_H
