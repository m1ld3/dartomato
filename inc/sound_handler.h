#ifndef SOUND_HANDLER_H
#define SOUND_HANDLER_H

#include <QSoundEffect>


class CSoundHandler : public QObject
{
  Q_OBJECT

public:

  static CSoundHandler & instance()
  {
    static CSoundHandler instance;
    return instance;
  }

  void set_muted(bool iMuted)
  {
    mBustedSound.setMuted(iMuted);
    mGameShotSound.setMuted(iMuted);
    mGameOnSound.setMuted(iMuted);
    mScoreSound.setMuted(iMuted);
  }

  void set_score_sound_source(const QString & iSoundPath) { mScoreSound.setSource(iSoundPath); }
  void play_score_sound() { mScoreSound.play(); }
  void play_game_on_sound() { mGameOnSound.play(); }
  void play_busted_sound() { mBustedSound.play(); }
  void play_game_shot_sound() { mGameShotSound.play(); }

private:

  CSoundHandler()
    : mBustedSound(this)
    , mGameShotSound(this)
    , mGameOnSound(this)
    , mScoreSound(this)
  {
    mBustedSound.setSource(QUrl("qrc:/resources/sounds/busted.wav"));
    mGameShotSound.setSource(QUrl("qrc:/resources/sounds/gameshot.wav"));
    mGameOnSound.setSource(QUrl("qrc:/resources/sounds/gameon.wav"));
  }

  QSoundEffect mBustedSound;
  QSoundEffect mGameShotSound;
  QSoundEffect mGameOnSound;
  QSoundEffect mScoreSound;
};

#endif  // SOUND_HANDLER_H
