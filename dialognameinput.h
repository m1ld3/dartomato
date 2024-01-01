#ifndef DIALOGNAMEINPUT_H
#define DIALOGNAMEINPUT_H

#include <QDialog>

namespace Ui
{
  class CDialogNameInput;
}

class CDialogNameInput : public QDialog
{
  Q_OBJECT

public:

  explicit CDialogNameInput(QWidget * iParent = nullptr, QString && iText = "");
  ~CDialogNameInput() override;

private slots:

  void push_button_ok_clicked_slot();

signals:

  void signal_ok_button_clicked(const QString & iName);

private:

  Ui::CDialogNameInput * mUi;
};

#endif  // DIALOGNAMEINPUT_H
