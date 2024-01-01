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
  connect(mUi->pushButton_ok, &QPushButton::clicked, this, &CDialogNameInput::push_button_ok_clicked_slot);
}

CDialogNameInput::~CDialogNameInput()
{
  delete mUi;
}

void CDialogNameInput::push_button_ok_clicked_slot()
{
  emit signal_ok_button_clicked(mUi->lineEdit_name->text());
}
