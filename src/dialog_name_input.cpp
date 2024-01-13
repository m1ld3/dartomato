#include "ui_dialog_name_input.h"
#include <QString>
#include "dialog_name_input.h"

CDialogNameInput::CDialogNameInput(QWidget * iParent, QString && iText)
  : QDialog(iParent)
  , mUi(new Ui::CDialogNameInput)
{
  mUi->setupUi(this);
  mUi->lineEditName->setText(iText);
  mUi->lineEditName->selectAll();
  setAttribute(Qt::WA_DeleteOnClose);
  connect(mUi->pushButtonOk, &QPushButton::clicked, this, &CDialogNameInput::push_button_ok_clicked_slot);
}

CDialogNameInput::~CDialogNameInput()
{
  delete mUi;
}

void CDialogNameInput::push_button_ok_clicked_slot()
{
  emit signal_ok_button_clicked(mUi->lineEditName->text());
}
