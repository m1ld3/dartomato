#ifndef CDARTBOARD_H
#define CDARTBOARD_H

#include <QDialog>
#include <QLCDNumber>
#include <QGraphicsSvgItem>
#include <QLabel>
#include "dartboard_view.h"
#include <QSoundEffect>
#include "settings.h"

class CDartBoardSegment;

#ifdef TESTING
class CDartBoard
{
#else
class CDartBoard : public QWidget
{
  Q_OBJECT
#endif
public:

  virtual void handle_segment_pressed_event(uint32_t iVal, QChar iType) = 0;
  virtual void set_finished() = 0;
  virtual void unset_finished() = 0;
  virtual void perform_undo() = 0;
  virtual void submit_score() = 0;
  virtual void init_dartboard(uint32_t iScore) = 0;

public:

  CDartBoard(CDartBoardView * iGraphicsViewDartBoard, const CSettings iSettings);
#ifdef TESTING
  CDartBoard(const CSettings iSettings)
    : mSettings(iSettings)
  {}
#endif
  virtual ~CDartBoard();
  void play_game_shot_sound();

protected:

  QGraphicsView * mView;
  QGraphicsScene * mScene;
  QSoundEffect mBustedSound;
  QSoundEffect mGameShotSound;
#ifdef TESTING
  CSettings mSettings;
#else
  const CSettings mSettings;
#endif

  CDartBoardSegment * mS20;
  CDartBoardSegment * mS19;
  CDartBoardSegment * mS18;
  CDartBoardSegment * mS17;
  CDartBoardSegment * mS14;
  CDartBoardSegment * mS13;
  CDartBoardSegment * mS12;
  CDartBoardSegment * mS11;
  CDartBoardSegment * mS16;
  CDartBoardSegment * mS15;
  CDartBoardSegment * mS10;
  CDartBoardSegment * mS9;
  CDartBoardSegment * mS8;
  CDartBoardSegment * mS7;
  CDartBoardSegment * mS6;
  CDartBoardSegment * mS5;
  CDartBoardSegment * mS4;
  CDartBoardSegment * mS3;
  CDartBoardSegment * mS2;
  CDartBoardSegment * mS1;
  CDartBoardSegment * mSBull;
  CDartBoardSegment * mD20;
  CDartBoardSegment * mD19;
  CDartBoardSegment * mD18;
  CDartBoardSegment * mD17;
  CDartBoardSegment * mD16;
  CDartBoardSegment * mD15;
  CDartBoardSegment * mD14;
  CDartBoardSegment * mD13;
  CDartBoardSegment * mD12;
  CDartBoardSegment * mD11;
  CDartBoardSegment * mD10;
  CDartBoardSegment * mD9;
  CDartBoardSegment * mD8;
  CDartBoardSegment * mD7;
  CDartBoardSegment * mD6;
  CDartBoardSegment * mD5;
  CDartBoardSegment * mD4;
  CDartBoardSegment * mD3;
  CDartBoardSegment * mD2;
  CDartBoardSegment * mD1;
  CDartBoardSegment * mDBull;
  CDartBoardSegment * mT20;
  CDartBoardSegment * mT19;
  CDartBoardSegment * mT18;
  CDartBoardSegment * mT17;
  CDartBoardSegment * mT16;
  CDartBoardSegment * mT15;
  CDartBoardSegment * mT14;
  CDartBoardSegment * mT13;
  CDartBoardSegment * mT12;
  CDartBoardSegment * mT11;
  CDartBoardSegment * mT10;
  CDartBoardSegment * mT9;
  CDartBoardSegment * mT8;
  CDartBoardSegment * mT7;
  CDartBoardSegment * mT6;
  CDartBoardSegment * mT5;
  CDartBoardSegment * mT4;
  CDartBoardSegment * mT3;
  CDartBoardSegment * mT2;
  CDartBoardSegment * mT1;
  CDartBoardSegment * mNoScore;
  QGraphicsSvgItem * const mLabels = new QGraphicsSvgItem(":/resources/img/dartboard.svg");
};

#endif  // CDARTBOARD_H
