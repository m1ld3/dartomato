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
#include "version.h"
#include "game_history_dialog.h"

CDartomatMain::CDartomatMain(QWidget * iParent)
  : QMainWindow(iParent)
  , mUi(new Ui::CDartomatMain)
#ifndef USE_TTS
  , mGameOnSound(this)
  , mGameDataHandler(CGameDataHandler())
  , mPlayerListModel(CPlayerListModel(mGameDataHandler, this))
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

  create_menu();

  connect(mUi->pushButtonStartGame, &QPushButton::clicked, this, &CDartomatMain::push_button_startgame_clicked_slot);
  connect(mUi->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
  {
    const QString game = mUi->comboBoxGame->itemText(index);
    combo_box_game_current_index_changed_slot(game);
  });
  connect(mUi->pushButtonSelectPlayers, &QPushButton::clicked, this, &CDartomatMain::push_button_select_players_clicked_slot);
  connect(mUi->pushButtonGameHistory, &QPushButton::clicked, this, &CDartomatMain::push_button_game_history_clicked_slot);
}

CDartomatMain::~CDartomatMain()
{
  delete mUi;
}

void CDartomatMain::create_menu()
{
  QMenuBar * menuBar = new QMenuBar(this);
  setMenuBar(menuBar);

  QMenu * fileMenu = menuBar->addMenu("?");
  QAction * aboutAction = fileMenu->addAction("About");
  connect(aboutAction, &QAction::triggered, this, &CDartomatMain::show_about_dialog);
}

void CDartomatMain::handle_selected_players(const QStringList & iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
}

void CDartomatMain::push_button_startgame_clicked_slot()
{
  if (mSelectedPlayers.empty())
  {
    QMessageBox::warning(this, "No players selected!", "Please select at least one player.");
    return;
  }
  uint32_t game = (mUi->comboBoxGame->itemText(mUi->comboBoxGame->currentIndex())).toInt();
  uint32_t sets = mUi->spinBoxSets->value();
  uint32_t legs = mUi->spinBoxLegs->value();
  bool cutThroat = mUi->checkBoxCutThroat->isChecked();
  EX01InMode inMode;
  EX01OutMode outMode;
  if (mUi->radioButtonSin->isChecked()) inMode = EX01InMode::SINGLE_IN;
  else if (mUi->radioButtonDin->isChecked()) inMode = EX01InMode::DOUBLE_IN;
  else inMode = EX01InMode::MASTER_IN;
  if (mUi->radioButtonSout->isChecked()) outMode = EX01OutMode::SINGLE_OUT;
  else if (mUi->radioButtonDout->isChecked()) outMode = EX01OutMode::DOUBLE_OUT;
  else outMode = EX01OutMode::MASTER_OUT;

  mSettings = CSettings(static_cast<EGame>(game), mSelectedPlayers,
                        sets, legs, inMode, outMode, cutThroat);

  if (game > 0)
  {
    mX01MainWindow = new CX01MainWindow(this, mSettings, mGameDataHandler);
    mX01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mX01MainWindow->show();
  }
  else
  {
    mCricketMainWindow = new CCricketMainWindow(this, mSettings, mGameDataHandler);
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

void CDartomatMain::push_button_select_players_clicked_slot()
{
  QPointer<CAddPlayersDialog> dialog = new CAddPlayersDialog(mPlayerListModel, this);
  dialog->show();
}

void CDartomatMain::show_about_dialog()
{
  QMessageBox::about(this, "About", QString("Version: ") + VER_FILEVERSION_STR);
}

void CDartomatMain::push_button_game_history_clicked_slot()
{
  QPointer<CGameHistoryDialog> dialog = new CGameHistoryDialog(mGameDataHandler);
  dialog->show();
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
