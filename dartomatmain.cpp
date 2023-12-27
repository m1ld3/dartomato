#include "dartomatmain.h"
#include "ui_dartomatmain.h"
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#include <QSoundEffect>
#include <QDebug>

CDartomatMain::CDartomatMain(QWidget * iParent)
  : QMainWindow(iParent)
  , mUi(new Ui::CDartomatMain)
  , mGameOnSound(this)
{
  mUi->setupUi(this);
  mUi->radioButton_Sin->setChecked(true);
  mUi->radioButton_Dout->setChecked(true);
  mUi->comboBox_game->setCurrentIndex(1);
  mUi->checkBoxCutThroat->setVisible(false);
  mGameOnSound.setSource(QUrl("qrc:/resources/sounds/gameon.wav"));
}


CDartomatMain::~CDartomatMain()
{
  delete mUi;
}


void CDartomatMain::on_pushButton_startgame_clicked()
{
  uint32_t game = (mUi->comboBox_game->itemText(mUi->comboBox_game->currentIndex())).toInt();
  uint32_t numberofplayers = mUi->comboBox_players->currentIndex() + 1;
  uint32_t sets = mUi->spinBox_sets->value();
  uint32_t legs = mUi->spinBox_legs->value();
  bool singleIn = mUi->radioButton_Sin->isChecked();
  bool singleOut = mUi->radioButton_Sout->isChecked();
  bool doubleIn = mUi->radioButton_Din->isChecked();
  bool doubleOut = mUi->radioButton_Dout->isChecked();
  bool masterIn = mUi->radioButton_Min->isChecked();
  bool masterOut = mUi->radioButton_Mout->isChecked();
  bool cutThroat = mUi->checkBoxCutThroat->isChecked();
  bool offensive = mUi->checkBoxOffensive->isChecked();

  if (game > 0)
  {
    mX01MainWindow = new CX01MainWindow(this, numberofplayers, game, sets, legs, singleIn, singleOut, doubleIn, doubleOut, masterIn, masterOut, offensive);
    mX01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mX01MainWindow->show();
  }
  else
  {
    mCricketMainWindow = new CCricketMainWindow(this, numberofplayers, sets, legs, cutThroat, offensive);
    mCricketMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mCricketMainWindow->show();
  }
  mGameOnSound.play();
}
