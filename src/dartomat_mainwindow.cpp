#include "dartomat_mainwindow.h"
#include "ui_dartomat_mainwindow.h"
#include "add_players_dialog.h"
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#ifndef USE_TTS
#include <QSoundEffect>
#endif
#include <QJsonArray>
#include <QMessageBox>

CDartomatMain::CDartomatMain(QWidget * iParent)
  : QMainWindow(iParent)
  , mUi(new Ui::CDartomatMain)
#ifndef USE_TTS
  , mGameOnSound(this)
  , mGameDataHandler(CGameDataHandler())
  , mGameDataModel(CGameDataModel(mGameDataHandler, this))
#endif
{
  mUi->setupUi(this);
  mUi->radioButtonSin->setChecked(true);
  mUi->radioButtonDout->setChecked(true);
  mUi->comboBoxGame->setCurrentIndex(1);
  mUi->checkBoxCutThroat->setVisible(false);
#ifndef USE_TTS
  mGameOnSound.setSource(QUrl("qrc:/resources/sounds/gameon.wav"));
#endif

  connect(mUi->pushButtonStartGame, &QPushButton::clicked, this, &CDartomatMain::push_button_startgame_clicked_slot);
  connect(mUi->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
  {
    const QString game = mUi->comboBoxGame->itemText(index);
    combo_box_game_current_index_changed_slot(game);
  });
  connect(mUi->pushButtonSelectPlayers, &QPushButton::clicked, this, &CDartomatMain::push_button_select_players_clicked_slot);
  connect(mUi->pushButtonCreatePlayers, &QPushButton::clicked, this, &CDartomatMain::push_button_create_players_clicked_slot);
}

CDartomatMain::~CDartomatMain()
{
  delete mUi;
}

void CDartomatMain::push_button_startgame_clicked_slot()
{
  uint32_t game = (mUi->comboBoxGame->itemText(mUi->comboBoxGame->currentIndex())).toInt();
//  uint32_t numberOfPlayers = mUi->comboBox_players->currentIndex() + 1;
  uint32_t sets = mUi->spinBoxSets->value();
  uint32_t legs = mUi->spinBoxLegs->value();
  bool singleIn = mUi->radioButtonSin->isChecked();
  bool singleOut = mUi->radioButtonSout->isChecked();
  bool doubleIn = mUi->radioButtonDin->isChecked();
  bool doubleOut = mUi->radioButtonDout->isChecked();
  bool masterIn = mUi->radioButtonMin->isChecked();
  bool masterOut = mUi->radioButtonMout->isChecked();
  bool cutThroat = mUi->checkBoxCutThroat->isChecked();

  mSettings = CSettings(static_cast<EGame>(game),1 /*numberOfPlayers*/, sets, legs, singleIn,
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

  play_game_on_sound();
}

void CDartomatMain::combo_box_game_current_index_changed_slot(const QString & iGame)
{
  if (iGame == "Cricket")
  {
    mUi->radioButtonSin->setVisible(false);
    mUi->radioButtonDin->setVisible(false);
    mUi->radioButtonMin->setVisible(false);
    mUi->radioButtonSout->setVisible(false);
    mUi->radioButtonDout->setVisible(false);
    mUi->radioButtonMout->setVisible(false);
    mUi->checkBoxCutThroat->setVisible(true);
  }
  else
  {
    mUi->radioButtonSin->setVisible(true);
    mUi->radioButtonDin->setVisible(true);
    mUi->radioButtonMin->setVisible(true);
    mUi->radioButtonSout->setVisible(true);
    mUi->radioButtonDout->setVisible(true);
    mUi->radioButtonMout->setVisible(true);
    mUi->checkBoxCutThroat->setVisible(false);
  }
}

void CDartomatMain::open_add_players_dialog()
{
  QPointer<CAddPlayersDialog> dialog = new CAddPlayersDialog(mGameDataModel, this);
  dialog->show();
}

void CDartomatMain::push_button_select_players_clicked_slot()
{
  QStringList playerList = mGameDataHandler.get_player_names();
  if (playerList.isEmpty())
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Select Players", "There are no players available yet. Do you want to add new player?",
                          QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      open_add_players_dialog();
    }
  }
  else
  {
    QPointer<QDialog> dialog = new QDialog();
    dialog->show();
    }
}

void CDartomatMain::push_button_create_players_clicked_slot()
{
  open_add_players_dialog();
}

void CDartomatMain::play_game_on_sound()
{
#ifdef USE_TTS
  auto * tts = new QTextToSpeech;
  QLocale locale(QLocale::English, QLocale::UnitedKingdom);
  tts->setLocale(locale);
  tts->setVolume(1.0);
  tts->setRate(0.1);
  tts->say("Game on!");
#else
  mGameOnSound.play();
#endif
}
