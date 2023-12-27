#include "ui_dialognameinput.h"
#include <QString>
#include <QDebug>
#include "groupbox_player.h"
#include "dialognameinput.h"

CDialogNameInput::CDialogNameInput(QWidget * iParent, QString && iText)
  : QDialog(iParent)
  , mUi(new Ui::CDialogNameInput)
{
  mUi->setupUi(this);
  mUi->lineEdit_name->setText(iText);
  mUi->lineEdit_name->selectAll();
  setAttribute(Qt::WA_DeleteOnClose);
}

CDialogNameInput::~CDialogNameInput()
{
  delete mUi;
}

void CDialogNameInput::on_pushButton_ok_clicked()
{
  emit ok_button_clicked(mUi->lineEdit_name->text());
}
