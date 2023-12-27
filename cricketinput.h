#ifndef CRICKETINPUT_H
#define CRICKETINPUT_H

#include <QDialog>
#include <QGraphicsView>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include "undobutton.h"
#include "submitbutton.h"
#include <QLabel>
#include <QGraphicsSvgItem>
#include <QSoundEffect>

class CCricketMainWindow;
class CCricketClass;

namespace Ui
{
  class CCricketinput;
}


class CCricketInput : public QDialog
{
  Q_OBJECT
public:

  void set_score(uint32_t iValue, QChar iType);
  void compute_score();
  bool are_slots_full() const;

public slots:

  void signal_segment_pressed(uint32_t iVal, QChar & iType);

signals:

  void signal_cricket_submit_button_pressed(uint32_t & iNumberOfDarts, QVector<QString> iDarts);
  void signal_set_label_slot(uint32_t iHits, uint32_t iSlot);

public:

  CCricketInput(QWidget * iParent = nullptr, uint32_t iSets = 1, uint32_t iLegs = 1, CCricketClass * iPlayer = nullptr, CCricketMainWindow * iGameWindow = nullptr, bool iCutThroat = false);
  ~CCricketInput();

private slots:

  void on_submitButton_clicked();
  void on_undoButton_clicked();

private:

  Ui::CCricketinput * mUi;
  QGraphicsView * mView;
  QGraphicsScene * mScene;
  QGraphicsSvgItem * mLabels;
  uint32_t mSets, mLegs, mCounter;
  bool mStop;
  QLCDNumber * mScoreDart1;
  QLCDNumber * mScoreDart2;
  QLCDNumber * mScoreDart3;
  CUndoButton * mUndoButton;
  CSubmitButton * mSubmitButton;
  QVector<QString> mDarts{"","",""};
  CCricketClass * mPlayer;
  CCricketMainWindow * mGameWindow;
  uint32_t mSlot15, mSlot16, mSlot17, mSlot18, mSlot19, mSlot20, mSlot25;
  uint32_t mExtra15, mExtra16, mExtra17, mExtra18, mExtra19, mExtra20, mExtra25;
  uint32_t mScore;
  bool mCutThroat;
  QVector<uint32_t> mExtra15s, mExtra16s, mExtra17s, mExtra18s, mExtra19s, mExtra20s, mExtra25s;
  QVector<uint32_t> mSlot15Array, mSlot16Array, mSlot17Array, mSlot18Array, mSlot19Array, mSlot20Array, mSlot25Array;
  QVector<uint32_t> mExtra15Array, mExtra16Array, mExtra17Array, mExtra18Array, mExtra19Array, mExtra20Array, mExtra25Array;
  QVector<QVector<uint32_t>> mExtra15sArray, mExtra16sArray, mExtra17sArray, mExtra18sArray, mExtra19sArray, mExtra20sArray, mExtra25sArray;
  QSoundEffect mGameShotSound;
};

#endif  // CRICKETINPUT_H
