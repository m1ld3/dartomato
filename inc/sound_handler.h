#ifndef SOUND_HANDLER_H
#define SOUND_HANDLER_H

#include <QMediaPlayer>
#include <QAudioOutput>


class CSoundHandler : public QObject
{
  Q_OBJECT

public:

  static CSoundHandler & instance()
  {
    static CSoundHandler instance;
    return instance;
  }

  void set_muted(const bool iMuted) const
  {
    mBustedSound.audioOutput()->setMuted(iMuted);
    mGameShotSound.audioOutput()->setMuted(iMuted);
    mGameOnSound.audioOutput()->setMuted(iMuted);
    mScoreSound.audioOutput()->setMuted(iMuted);
  }

  void set_score_sound_source(const QString & iSoundPath)
  {
    if (mScoreSound.source() != QUrl(iSoundPath)) mScoreSound.setSource(QUrl(iSoundPath));
  }
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
    // each QMediaPlayer needs its own QAudioOutput
    mBustedSound.setAudioOutput(new QAudioOutput);
    mGameShotSound.setAudioOutput(new QAudioOutput);
    mGameOnSound.setAudioOutput(new QAudioOutput);
    mScoreSound.setAudioOutput(new QAudioOutput);

    mBustedSound.setSource(QUrl("qrc:/resources/sounds/busted.mp3"));
    mGameShotSound.setSource(QUrl("qrc:/resources/sounds/gameshot.mp3"));
    mGameOnSound.setSource(QUrl("qrc:/resources/sounds/gameon.mp3"));
  }

  QMediaPlayer mBustedSound;
  QMediaPlayer mGameShotSound;
  QMediaPlayer mGameOnSound;
  QMediaPlayer mScoreSound;
};

#endif  // SOUND_HANDLER_H
