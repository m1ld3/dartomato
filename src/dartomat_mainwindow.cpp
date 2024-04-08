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
#include "alltime_stats_dialog.h"
#include <QFile>
#include <QTimer>

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
  connect(mUi->pushButtonStats, &QPushButton::clicked, this, &CDartomatMain::push_button_stats_clicked_slot);
  QTimer::singleShot(500, this, &CDartomatMain::check_for_unfinished_game);
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

void CDartomatMain::check_for_unfinished_game()
{
  QFile file(OpenGamePath);
  if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream in(&file);
    auto timeStamp = in.readLine().toStdString();
    show_unfinished_game_popup(QString::fromStdString(timeStamp));
    file.remove();
  }
}

bool CDartomatMain::delete_player_data(const QStringList & iSelectedPlayers)
{
  bool result = true;
  for (const auto & player : iSelectedPlayers) result = result && mGameDataHandler.delete_player_from_db(player);
  return result;
}

void CDartomatMain::handle_selected_players(const QStringList & iSelectedPlayers)
{
  mSelectedPlayers = iSelectedPlayers;
}

void CDartomatMain::start_game(CSettings iSettings)
{
  if (iSettings.Game == EGame::GAME_CRICKET)
  {
    mCricketMainWindow = new CCricketMainWindow(this, iSettings, mGameDataHandler);
    mCricketMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mCricketMainWindow->show();
  }
  else
  {
    mX01MainWindow = new CX01MainWindow(this, iSettings, mGameDataHandler);
    mX01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mX01MainWindow->show();
  }

  play_game_on_sound();
}

void CDartomatMain::resume_game(const CGameDataHandler::SGameData iGameData)
{
  if (iGameData.Settings.Game == EGame::GAME_CRICKET)
  {
    mCricketMainWindow = new CCricketMainWindow(this, iGameData.Settings, mGameDataHandler, iGameData.GameDataCricket);
    mCricketMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mCricketMainWindow->show();
  }
  else
  {
    mX01MainWindow = new CX01MainWindow(this, iGameData.Settings, mGameDataHandler, iGameData.GameDataX01);
    mX01MainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mX01MainWindow->show();
  }
  mGameDataHandler.delete_game_from_db(iGameData.TimeStamp);
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

  auto settings = CSettings(static_cast<EGame>(game), mSelectedPlayers,
                        sets, legs, inMode, outMode, cutThroat);

  start_game(settings);
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
  QPointer<CGameHistoryDialog> dialog = new CGameHistoryDialog(mGameDataHandler, this);
  dialog->show();
}

void CDartomatMain::show_unfinished_game_popup(const QString & iTimeStamp)
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Resume Game.",
                                                             "Do you want to resume the last unfinished game?",
                                                             QMessageBox::Yes | QMessageBox::No);
  if (resBtn == QMessageBox::Yes)
  {
    const QVector<CGameDataHandler::SGameData> gameData = mGameDataHandler.get_game_data();
    for (const auto & data : gameData)
    {
      if (data.TimeStamp == iTimeStamp) resume_game(data);
      break;
    }
  }
}

void CDartomatMain::push_button_stats_clicked_slot()
{
  QPointer<CAllTimeStatsDialog> dialog = new CAllTimeStatsDialog(mGameDataHandler);
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
