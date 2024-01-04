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

  connect(mUi->pushButton_startgame, &QPushButton::clicked, this, &CDartomatMain::push_button_startgame_clicked_slot);
  connect(mUi->comboBox_game, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
  {
    const QString game = mUi->comboBox_game->itemText(index);
    combo_box_game_current_index_changed_slot(game);
  });
}


CDartomatMain::~CDartomatMain()
{
  delete mUi;
}


void CDartomatMain::push_button_startgame_clicked_slot()
{
  uint32_t game = (mUi->comboBox_game->itemText(mUi->comboBox_game->currentIndex())).toInt();
  uint32_t numberOfPlayers = mUi->comboBox_players->currentIndex() + 1;
  uint32_t sets = mUi->spinBox_sets->value();
  uint32_t legs = mUi->spinBox_legs->value();
  bool singleIn = mUi->radioButton_Sin->isChecked();
  bool singleOut = mUi->radioButton_Sout->isChecked();
  bool doubleIn = mUi->radioButton_Din->isChecked();
  bool doubleOut = mUi->radioButton_Dout->isChecked();
  bool masterIn = mUi->radioButton_Min->isChecked();
  bool masterOut = mUi->radioButton_Mout->isChecked();
  bool cutThroat = mUi->checkBoxCutThroat->isChecked();

  mSettings = CSettings(static_cast<EGame>(game), numberOfPlayers, sets, legs, singleIn,
                        singleOut, doubleIn, doubleOut, masterIn, masterOut, cutThroat);

  if (game > 0)
  {
    mX01MainWindow = new CX01MainWindow(this, mSettings);
    mX01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mX01MainWindow->show();
  }
  else
  {
    mCricketMainWindow = new CCricketMainWindow(this, mSettings);
    mCricketMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mCricketMainWindow->show();
  }
  mGameOnSound.play();
}

void CDartomatMain::combo_box_game_current_index_changed_slot(const QString & iGame)
{
  if (iGame == "Cricket")
  {
    mUi->radioButton_Sin->setVisible(false);
    mUi->radioButton_Din->setVisible(false);
    mUi->radioButton_Min->setVisible(false);
    mUi->radioButton_Sout->setVisible(false);
    mUi->radioButton_Dout->setVisible(false);
    mUi->radioButton_Mout->setVisible(false);
    mUi->checkBoxCutThroat->setVisible(true);
    mUi->checkBoxCutThroat->move(mUi->radioButton_Sout->pos());
  }
  else
  {
    mUi->radioButton_Sin->setVisible(true);
    mUi->radioButton_Din->setVisible(true);
    mUi->radioButton_Min->setVisible(true);
    mUi->radioButton_Sout->setVisible(true);
    mUi->radioButton_Dout->setVisible(true);
    mUi->radioButton_Mout->setVisible(true);
    mUi->checkBoxCutThroat->setVisible(false);
  }
}
