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

  void on_pushButton_ok_clicked();

signals:

  void ok_button_clicked(QString && iName);

private:

  Ui::CDialogNameInput * mUi;
};

#endif  // DIALOGNAMEINPUT_H
