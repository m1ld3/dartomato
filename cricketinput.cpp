#include "cricketinput.h"
#include "ui_cricketinput.h"
#include <QGraphicsPathItem>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsLinearLayout>
#include <QtDebug>
#include <cmath>
#include <QMessageBox>
#include <QVector>
#include "cricketclass.h"

CCricketInput::CCricketInput(QWidget * iParent, uint32_t iSets, uint32_t iLegs, CCricketClass * iPlayer, CCricketMainWindow * iGameWindow, bool iCutThroat)
  : QDialog(iParent), mUi(new Ui::CCricketinput), mSets(iSets), mLegs(iLegs), mCounter(3), mPlayer(iPlayer), mGameWindow(iGameWindow)
  , mSlot15(mPlayer->get_slot15()), mSlot16(mPlayer->get_slot16()), mSlot17(mPlayer->get_slot17()), mSlot18(mPlayer->get_slot18())
  , mSlot19(mPlayer->get_slot19()), mSlot20(mPlayer->get_slot20()), mSlot25(mPlayer->get_slot25()), mExtra15(mPlayer->get_extra15())
  , mExtra16(mPlayer->get_extra16()), mExtra17(mPlayer->get_extra17()), mExtra18(mPlayer->get_extra18()), mExtra19(mPlayer->get_extra19())
  , mExtra20(mPlayer->get_extra20()), mExtra25(mPlayer->get_extra25()), mScore(mPlayer->get_score()), mCutThroat(iCutThroat)
  , mExtra15s(mGameWindow->compute_extra15s(0, mPlayer->get_player_number()))
  , mExtra16s(mGameWindow->compute_extra16s(0, mPlayer->get_player_number()))
  , mExtra17s(mGameWindow->compute_extra17s(0, mPlayer->get_player_number()))
  , mExtra18s(mGameWindow->compute_extra18s(0, mPlayer->get_player_number()))
  , mExtra19s(mGameWindow->compute_extra19s(0, mPlayer->get_player_number()))
  , mExtra20s(mGameWindow->compute_extra20s(0, mPlayer->get_player_number()))
  , mExtra25s(mGameWindow->compute_extra25s(0, mPlayer->get_player_number())), mSlot15Array({mSlot15}), mSlot16Array({mSlot16})
  , mSlot17Array({mSlot17}), mSlot18Array({mSlot18}), mSlot19Array({mSlot19}), mSlot20Array({mSlot20}), mSlot25Array({mSlot25})
  , mExtra15Array({mExtra15}), mExtra16Array({mExtra16}), mExtra17Array({mExtra17}), mExtra18Array({mExtra18})
  , mExtra19Array({mExtra19}), mExtra20Array({mExtra20}), mExtra25Array({mExtra25}), mExtra15sArray({mExtra15s})
  , mExtra16sArray({mExtra16s}), mExtra17sArray({mExtra17s}), mExtra18sArray({mExtra18s}), mExtra19sArray({mExtra19s})
  , mExtra20sArray({mExtra20s}), mExtra25sArray({mExtra25s})
  , mGameShotSound(this)
{
  mUi->setupUi(this);
  mStop = false;
  mUi->submitButton->setAutoDefault(true);
  mScene = new QGraphicsScene(0, 0, 800, 800, mUi->graphicsView_dartboard);
  mScene->setSceneRect(0, 0, 800, 800);
  mUi->graphicsView_dartboard->setRenderHint(QPainter::SmoothPixmapTransform, true);
  mUi->graphicsView_dartboard->setRenderHint(QPainter::Antialiasing, true);
  mUi->graphicsView_dartboard->setScene(mScene);
  mUi->graphicsView_dartboard->show();

  QPainterPath p_s20, p_s19, p_s18, p_s17, p_s16, p_s15, p_s14, p_s13, p_s12, p_s11,
          p_s10, p_s9, p_s8, p_s7, p_s6, p_s5, p_s4, p_s3, p_s2, p_s1, p_sbull,
          p_d20, p_d19, p_d18, p_d17, p_d16, p_d15, p_d14, p_d13, p_d12, p_d11,
          p_d10, p_d9, p_d8, p_d7, p_d6, p_d5, p_d4, p_d3, p_d2, p_d1, p_dbull,
          p_t20, p_t19, p_t18, p_t17, p_t16, p_t15, p_t14, p_t13, p_t12, p_t11,
          p_t10, p_t9, p_t8, p_t7, p_t6, p_t5, p_t4, p_t3, p_t2, p_t1, p_noscore;

  p_s20.moveTo(400+40*cos(M_PI*81/180), 400-40*sin(M_PI*81/180));
  p_s20.lineTo(400+180*cos(M_PI*81/180), 400-180*sin(M_PI*81/180));
  p_s20.arcTo(400-180, 400-180, 2*180, 2*180, 81, 18);
  p_s20.lineTo(400+40*cos(M_PI*99/180), 400-40*sin(M_PI*99/180));
  p_s20.arcTo(400-40, 400-40, 2*40, 2*40, 99, -18);
  p_s20.moveTo(400+200*cos(M_PI*81/180), 400-200*sin(M_PI*81/180));
  p_s20.lineTo(400+320*cos(M_PI*81/180), 400-320*sin(M_PI*81/180));
  p_s20.arcTo(400-320, 400-320, 2*320, 2*320, 81, 18);
  p_s20.lineTo(400+320*cos(M_PI*99/180), 400-320*sin(M_PI*99/180));
  p_s20.arcTo(400-200, 400-200, 2*200, 2*200, 99, -18);

  p_s19.moveTo(400+40*cos(M_PI*243/180), 400-40*sin(M_PI*243/180));
  p_s19.lineTo(400+180*cos(M_PI*243/180), 400-180*sin(M_PI*243/180));
  p_s19.arcTo(400-180, 400-180, 2*180, 2*180, 243, 18);
  p_s19.lineTo(400+40*cos(M_PI*261/180), 400-40*sin(M_PI*261/180));
  p_s19.arcTo(400-40, 400-40, 2*40, 2*40, 261, -18);
  p_s19.moveTo(400+200*cos(M_PI*243/180), 400-200*sin(M_PI*243/180));
  p_s19.lineTo(400+320*cos(M_PI*243/180), 400-320*sin(M_PI*243/180));
  p_s19.arcTo(400-320, 400-320, 2*320, 2*320, 243, 18);
  p_s19.lineTo(400+320*cos(M_PI*261/180), 400-320*sin(M_PI*261/180));
  p_s19.arcTo(400-200, 400-200, 2*200, 2*200, 261, -18);

  p_s18.moveTo(400+40*cos(M_PI*45/180), 400-40*sin(M_PI*45/180));
  p_s18.lineTo(400+180*cos(M_PI*45/180), 400-180*sin(M_PI*45/180));
  p_s18.arcTo(400-180, 400-180, 2*180, 2*180, 45, 18);
  p_s18.lineTo(400+40*cos(M_PI*63/180), 400-40*sin(M_PI*63/180));
  p_s18.arcTo(400-40, 400-40, 2*40, 2*40, 63, -18);
  p_s18.moveTo(400+200*cos(M_PI*45/180), 400-200*sin(M_PI*45/180));
  p_s18.lineTo(400+320*cos(M_PI*45/180), 400-320*sin(M_PI*45/180));
  p_s18.arcTo(400-320, 400-320, 2*320, 2*320, 45, 18);
  p_s18.lineTo(400+320*cos(M_PI*63/180), 400-320*sin(M_PI*63/180));
  p_s18.arcTo(400-200, 400-200, 2*200, 2*200, 63, -18);

  p_s17.moveTo(400+40*cos(M_PI*279/180), 400-40*sin(M_PI*279/180));
  p_s17.lineTo(400+180*cos(M_PI*279/180), 400-180*sin(M_PI*279/180));
  p_s17.arcTo(400-180, 400-180, 2*180, 2*180, 279, 18);
  p_s17.lineTo(400+40*cos(M_PI*297/180), 400-40*sin(M_PI*297/180));
  p_s17.arcTo(400-40, 400-40, 2*40, 2*40, 297, -18);
  p_s17.moveTo(400+200*cos(M_PI*279/180), 400-200*sin(M_PI*279/180));
  p_s17.lineTo(400+320*cos(M_PI*279/180), 400-320*sin(M_PI*279/180));
  p_s17.arcTo(400-320, 400-320, 2*320, 2*320, 279, 18);
  p_s17.lineTo(400+320*cos(M_PI*297/180), 400-320*sin(M_PI*297/180));
  p_s17.arcTo(400-200, 400-200, 2*200, 2*200, 297, -18);

  p_s16.moveTo(400+40*cos(M_PI*207/180), 400-40*sin(M_PI*207/180));
  p_s16.lineTo(400+180*cos(M_PI*207/180), 400-180*sin(M_PI*207/180));
  p_s16.arcTo(400-180, 400-180, 2*180, 2*180, 207, 18);
  p_s16.lineTo(400+40*cos(M_PI*225/180), 400-40*sin(M_PI*225/180));
  p_s16.arcTo(400-40, 400-40, 2*40, 2*40, 225, -18);
  p_s16.moveTo(400+200*cos(M_PI*207/180), 400-200*sin(M_PI*207/180));
  p_s16.lineTo(400+320*cos(M_PI*207/180), 400-320*sin(M_PI*207/180));
  p_s16.arcTo(400-320, 400-320, 2*320, 2*320, 207, 18);
  p_s16.lineTo(400+320*cos(M_PI*225/180), 400-320*sin(M_PI*225/180));
  p_s16.arcTo(400-200, 400-200, 2*200, 2*200, 225, -18);

  p_s15.moveTo(400+40*cos(M_PI*315/180), 400-40*sin(M_PI*315/180));
  p_s15.lineTo(400+180*cos(M_PI*315/180), 400-180*sin(M_PI*315/180));
  p_s15.arcTo(400-180, 400-180, 2*180, 2*180, 315, 18);
  p_s15.lineTo(400+40*cos(M_PI*333/180), 400-40*sin(M_PI*333/180));
  p_s15.arcTo(400-40, 400-40, 2*40, 2*40, 333, -18);
  p_s15.moveTo(400+200*cos(M_PI*315/180), 400-200*sin(M_PI*315/180));
  p_s15.lineTo(400+320*cos(M_PI*315/180), 400-320*sin(M_PI*315/180));
  p_s15.arcTo(400-320, 400-320, 2*320, 2*320, 315, 18);
  p_s15.lineTo(400+320*cos(M_PI*333/180), 400-320*sin(M_PI*333/180));
  p_s15.arcTo(400-200, 400-200, 2*200, 2*200, 333, -18);

  p_s14.moveTo(400+40*cos(M_PI*153/180), 400-40*sin(M_PI*153/180));
  p_s14.lineTo(400+180*cos(M_PI*153/180), 400-180*sin(M_PI*153/180));
  p_s14.arcTo(400-180, 400-180, 2*180, 2*180, 153, 18);
  p_s14.lineTo(400+40*cos(M_PI*171/180), 400-40*sin(M_PI*171/180));
  p_s14.arcTo(400-40, 400-40, 2*40, 2*40, 171, -18);
  p_s14.moveTo(400+200*cos(M_PI*153/180), 400-200*sin(M_PI*153/180));
  p_s14.lineTo(400+320*cos(M_PI*153/180), 400-320*sin(M_PI*153/180));
  p_s14.arcTo(400-320, 400-320, 2*320, 2*320, 153, 18);
  p_s14.lineTo(400+320*cos(M_PI*171/180), 400-320*sin(M_PI*171/180));
  p_s14.arcTo(400-200, 400-200, 2*200, 2*200, 171, -18);

  p_s13.moveTo(400+40*cos(M_PI*9/180), 400-40*sin(M_PI*9/180));
  p_s13.lineTo(400+180*cos(M_PI*9/180), 400-180*sin(M_PI*9/180));
  p_s13.arcTo(400-180, 400-180, 2*180, 2*180, 9, 18);
  p_s13.lineTo(400+40*cos(M_PI*27/180), 400-40*sin(M_PI*27/180));
  p_s13.arcTo(400-40, 400-40, 2*40, 2*40, 27, -18);
  p_s13.moveTo(400+200*cos(M_PI*9/180), 400-200*sin(M_PI*9/180));
  p_s13.lineTo(400+320*cos(M_PI*9/180), 400-320*sin(M_PI*9/180));
  p_s13.arcTo(400-320, 400-320, 2*320, 2*320, 9, 18);
  p_s13.lineTo(400+320*cos(M_PI*27/180), 400-320*sin(M_PI*27/180));
  p_s13.arcTo(400-200, 400-200, 2*200, 2*200, 27, -18);

  p_s12.moveTo(400+40*cos(M_PI*117/180), 400-40*sin(M_PI*117/180));
  p_s12.lineTo(400+180*cos(M_PI*117/180), 400-180*sin(M_PI*117/180));
  p_s12.arcTo(400-180, 400-180, 2*180, 2*180, 117, 18);
  p_s12.lineTo(400+40*cos(M_PI*135/180), 400-40*sin(M_PI*135/180));
  p_s12.arcTo(400-40, 400-40, 2*40, 2*40, 135, -18);
  p_s12.moveTo(400+200*cos(M_PI*117/180), 400-200*sin(M_PI*117/180));
  p_s12.lineTo(400+320*cos(M_PI*117/180), 400-320*sin(M_PI*117/180));
  p_s12.arcTo(400-320, 400-320, 2*320, 2*320, 117, 18);
  p_s12.lineTo(400+320*cos(M_PI*135/180), 400-320*sin(M_PI*135/180));
  p_s12.arcTo(400-200, 400-200, 2*200, 2*200, 135, -18);

  p_s11.moveTo(400+40*cos(M_PI*171/180), 400-40*sin(M_PI*171/180));
  p_s11.lineTo(400+180*cos(M_PI*171/180), 400-180*sin(M_PI*171/180));
  p_s11.arcTo(400-180, 400-180, 2*180, 2*180, 171, 18);
  p_s11.lineTo(400+40*cos(M_PI*189/180), 400-40*sin(M_PI*189/180));
  p_s11.arcTo(400-40, 400-40, 2*40, 2*40, 189, -18);
  p_s11.moveTo(400+200*cos(M_PI*171/180), 400-200*sin(M_PI*171/180));
  p_s11.lineTo(400+320*cos(M_PI*171/180), 400-320*sin(M_PI*171/180));
  p_s11.arcTo(400-320, 400-320, 2*320, 2*320, 171, 18);
  p_s11.lineTo(400+320*cos(M_PI*189/180), 400-320*sin(M_PI*189/180));
  p_s11.arcTo(400-200, 400-200, 2*200, 2*200, 189, -18);

  p_s10.moveTo(400+40*cos(M_PI*333/180), 400-40*sin(M_PI*333/180));
  p_s10.lineTo(400+180*cos(M_PI*333/180), 400-180*sin(M_PI*333/180));
  p_s10.arcTo(400-180, 400-180, 2*180, 2*180, 333, 18);
  p_s10.lineTo(400+40*cos(M_PI*351/180), 400-40*sin(M_PI*351/180));
  p_s10.arcTo(400-40, 400-40, 2*40, 2*40, 351, -18);
  p_s10.moveTo(400+200*cos(M_PI*333/180), 400-200*sin(M_PI*333/180));
  p_s10.lineTo(400+320*cos(M_PI*333/180), 400-320*sin(M_PI*333/180));
  p_s10.arcTo(400-320, 400-320, 2*320, 2*320, 333, 18);
  p_s10.lineTo(400+320*cos(M_PI*351/180), 400-320*sin(M_PI*351/180));
  p_s10.arcTo(400-200, 400-200, 2*200, 2*200, 351, -18);

  p_s9.moveTo(400+40*cos(M_PI*135/180), 400-40*sin(M_PI*135/180));
  p_s9.lineTo(400+180*cos(M_PI*135/180), 400-180*sin(M_PI*135/180));
  p_s9.arcTo(400-180, 400-180, 2*180, 2*180, 135, 18);
  p_s9.lineTo(400+40*cos(M_PI*153/180), 400-40*sin(M_PI*153/180));
  p_s9.arcTo(400-40, 400-40, 2*40, 2*40, 153, -18);
  p_s9.moveTo(400+200*cos(M_PI*135/180), 400-200*sin(M_PI*135/180));
  p_s9.lineTo(400+320*cos(M_PI*135/180), 400-320*sin(M_PI*135/180));
  p_s9.arcTo(400-320, 400-320, 2*320, 2*320, 135, 18);
  p_s9.lineTo(400+320*cos(M_PI*153/180), 400-320*sin(M_PI*153/180));
  p_s9.arcTo(400-200, 400-200, 2*200, 2*200, 153, -18);

  p_s8.moveTo(400+40*cos(M_PI*189/180), 400-40*sin(M_PI*189/180));
  p_s8.lineTo(400+180*cos(M_PI*189/180), 400-180*sin(M_PI*189/180));
  p_s8.arcTo(400-180, 400-180, 2*180, 2*180, 189, 18);
  p_s8.lineTo(400+40*cos(M_PI*207/180), 400-40*sin(M_PI*207/180));
  p_s8.arcTo(400-40, 400-40, 2*40, 2*40, 207, -18);
  p_s8.moveTo(400+200*cos(M_PI*189/180), 400-200*sin(M_PI*189/180));
  p_s8.lineTo(400+320*cos(M_PI*189/180), 400-320*sin(M_PI*189/180));
  p_s8.arcTo(400-320, 400-320, 2*320, 2*320, 189, 18);
  p_s8.lineTo(400+320*cos(M_PI*207/180), 400-320*sin(M_PI*207/180));
  p_s8.arcTo(400-200, 400-200, 2*200, 2*200, 207, -18);

  p_s7.moveTo(400+40*cos(M_PI*225/180), 400-40*sin(M_PI*225/180));
  p_s7.lineTo(400+180*cos(M_PI*225/180), 400-180*sin(M_PI*225/180));
  p_s7.arcTo(400-180, 400-180, 2*180, 2*180, 225, 18);
  p_s7.lineTo(400+40*cos(M_PI*243/180), 400-40*sin(M_PI*243/180));
  p_s7.arcTo(400-40, 400-40, 2*40, 2*40, 243, -18);
  p_s7.moveTo(400+200*cos(M_PI*225/180), 400-200*sin(M_PI*225/180));
  p_s7.lineTo(400+320*cos(M_PI*225/180), 400-320*sin(M_PI*225/180));
  p_s7.arcTo(400-320, 400-320, 2*320, 2*320, 225, 18);
  p_s7.lineTo(400+320*cos(M_PI*243/180), 400-320*sin(M_PI*243/180));
  p_s7.arcTo(400-200, 400-200, 2*200, 2*200, 243, -18);

  p_s6.moveTo(400+40*cos(M_PI*351/180), 400-40*sin(M_PI*351/180));
  p_s6.lineTo(400+180*cos(M_PI*351/180), 400-180*sin(M_PI*351/180));
  p_s6.arcTo(400-180, 400-180, 2*180, 2*180, 351, 18);
  p_s6.lineTo(400+40*cos(M_PI*9/180), 400-40*sin(M_PI*9/180));
  p_s6.arcTo(400-40, 400-40, 2*40, 2*40, 9, -18);
  p_s6.moveTo(400+200*cos(M_PI*351/180), 400-200*sin(M_PI*351/180));
  p_s6.lineTo(400+320*cos(M_PI*351/180), 400-320*sin(M_PI*351/180));
  p_s6.arcTo(400-320, 400-320, 2*320, 2*320, 351, 18);
  p_s6.lineTo(400+320*cos(M_PI*9/180), 400-320*sin(M_PI*9/180));
  p_s6.arcTo(400-200, 400-200, 2*200, 2*200, 9, -18);

  p_s5.moveTo(400+40*cos(M_PI*99/180), 400-40*sin(M_PI*99/180));
  p_s5.lineTo(400+180*cos(M_PI*99/180), 400-180*sin(M_PI*99/180));
  p_s5.arcTo(400-180, 400-180, 2*180, 2*180, 99, 18);
  p_s5.lineTo(400+40*cos(M_PI*117/180), 400-40*sin(M_PI*117/180));
  p_s5.arcTo(400-40, 400-40, 2*40, 2*40, 117, -18);
  p_s5.moveTo(400+200*cos(M_PI*99/180), 400-200*sin(M_PI*99/180));
  p_s5.lineTo(400+320*cos(M_PI*99/180), 400-320*sin(M_PI*99/180));
  p_s5.arcTo(400-320, 400-320, 2*320, 2*320, 99, 18);
  p_s5.lineTo(400+320*cos(M_PI*117/180), 400-320*sin(M_PI*117/180));
  p_s5.arcTo(400-200, 400-200, 2*200, 2*200, 117, -18);

  p_s4.moveTo(400+40*cos(M_PI*27/180), 400-40*sin(M_PI*27/180));
  p_s4.lineTo(400+180*cos(M_PI*27/180), 400-180*sin(M_PI*27/180));
  p_s4.arcTo(400-180, 400-180, 2*180, 2*180, 27, 18);
  p_s4.lineTo(400+40*cos(M_PI*45/180), 400-40*sin(M_PI*45/180));
  p_s4.arcTo(400-40, 400-40, 2*40, 2*40, 45, -18);
  p_s4.moveTo(400+200*cos(M_PI*27/180), 400-200*sin(M_PI*27/180));
  p_s4.lineTo(400+320*cos(M_PI*27/180), 400-320*sin(M_PI*27/180));
  p_s4.arcTo(400-320, 400-320, 2*320, 2*320, 27, 18);
  p_s4.lineTo(400+320*cos(M_PI*45/180), 400-320*sin(M_PI*45/180));
  p_s4.arcTo(400-200, 400-200, 2*200, 2*200, 45, -18);

  p_s3.moveTo(400+40*cos(M_PI*261/180), 400-40*sin(M_PI*261/180));
  p_s3.lineTo(400+180*cos(M_PI*261/180), 400-180*sin(M_PI*261/180));
  p_s3.arcTo(400-180, 400-180, 2*180, 2*180, 261, 18);
  p_s3.lineTo(400+40*cos(M_PI*279/180), 400-40*sin(M_PI*279/180));
  p_s3.arcTo(400-40, 400-40, 2*40, 2*40, 279, -18);
  p_s3.moveTo(400+200*cos(M_PI*261/180), 400-200*sin(M_PI*261/180));
  p_s3.lineTo(400+320*cos(M_PI*261/180), 400-320*sin(M_PI*261/180));
  p_s3.arcTo(400-320, 400-320, 2*320, 2*320, 261, 18);
  p_s3.lineTo(400+320*cos(M_PI*279/180), 400-320*sin(M_PI*279/180));
  p_s3.arcTo(400-200, 400-200, 2*200, 2*200, 279, -18);


  p_s2.moveTo(400+40*cos(M_PI*297/180), 400-40*sin(M_PI*297/180));
  p_s2.lineTo(400+180*cos(M_PI*297/180), 400-180*sin(M_PI*297/180));
  p_s2.arcTo(400-180, 400-180, 2*180, 2*180, 297, 18);
  p_s2.lineTo(400+40*cos(M_PI*315/180), 400-40*sin(M_PI*315/180));
  p_s2.arcTo(400-40, 400-40, 2*40, 2*40, 315, -18);
  p_s2.moveTo(400+200*cos(M_PI*297/180), 400-200*sin(M_PI*297/180));
  p_s2.lineTo(400+320*cos(M_PI*297/180), 400-320*sin(M_PI*297/180));
  p_s2.arcTo(400-320, 400-320, 2*320, 2*320, 297, 18);
  p_s2.lineTo(400+320*cos(M_PI*315/180), 400-320*sin(M_PI*315/180));
  p_s2.arcTo(400-200, 400-200, 2*200, 2*200, 315, -18);

  p_s1.moveTo(400+40*cos(M_PI*63/180), 400-40*sin(M_PI*63/180));
  p_s1.lineTo(400+180*cos(M_PI*63/180), 400-180*sin(M_PI*63/180));
  p_s1.arcTo(400-180, 400-180, 2*180, 2*180, 63, 18);
  p_s1.lineTo(400+40*cos(M_PI*81/180), 400-40*sin(M_PI*81/180));
  p_s1.arcTo(400-40, 400-40, 2*40, 2*40, 81, -18);
  p_s1.moveTo(400+200*cos(M_PI*63/180), 400-200*sin(M_PI*63/180));
  p_s1.lineTo(400+320*cos(M_PI*63/180), 400-320*sin(M_PI*63/180));
  p_s1.arcTo(400-320, 400-320, 2*320, 2*320, 63, 18);
  p_s1.lineTo(400+320*cos(M_PI*81/180), 400-320*sin(M_PI*81/180));
  p_s1.arcTo(400-200, 400-200, 2*200, 2*200, 81, -18);

  p_sbull.moveTo(400+40, 400);
  p_sbull.arcTo(400-40, 400-40, 2*40, 2*40, 0, 360);
  p_sbull.moveTo(400+20, 400);
  p_sbull.arcTo(400-20, 400-20, 2*20, 2*20, 0, 360);

  p_d20.moveTo(400+320*cos(M_PI*81/180), 400-320*sin(M_PI*81/180));
  p_d20.lineTo(400+340*cos(M_PI*81/180), 400-340*sin(M_PI*81/180));
  p_d20.arcTo(400-340, 400-340, 2*340, 2*340, 81, 18);
  p_d20.lineTo(400+320*cos(M_PI*99/180), 400-320*sin(M_PI*99/180));
  p_d20.arcTo(400-320, 400-320, 2*320, 2*320, 99, -18);

  p_d19.moveTo(400+320*cos(M_PI*243/180), 400-320*sin(M_PI*243/180));
  p_d19.lineTo(400+340*cos(M_PI*243/180), 400-340*sin(M_PI*243/180));
  p_d19.arcTo(400-340, 400-340, 2*340, 2*340, 243, 18);
  p_d19.lineTo(400+320*cos(M_PI*261/180), 400-320*sin(M_PI*261/180));
  p_d19.arcTo(400-320, 400-320, 2*320, 2*320, 261, -18);

  p_d18.moveTo(400+320*cos(M_PI*45/180), 400-320*sin(M_PI*45/180));
  p_d18.lineTo(400+340*cos(M_PI*45/180), 400-340*sin(M_PI*45/180));
  p_d18.arcTo(400-340, 400-340, 2*340, 2*340, 45, 18);
  p_d18.lineTo(400+320*cos(M_PI*63/180), 400-320*sin(M_PI*63/180));
  p_d18.arcTo(400-320, 400-320, 2*320, 2*320, 63, -18);

  p_d17.moveTo(400+320*cos(M_PI*279/180), 400-320*sin(M_PI*279/180));
  p_d17.lineTo(400+340*cos(M_PI*279/180), 400-340*sin(M_PI*279/180));
  p_d17.arcTo(400-340, 400-340, 2*340, 2*340, 279, 18);
  p_d17.lineTo(400+320*cos(M_PI*297/180), 400-320*sin(M_PI*297/180));
  p_d17.arcTo(400-320, 400-320, 2*320, 2*320, 297, -18);

  p_d16.moveTo(400+320*cos(M_PI*207/180), 400-320*sin(M_PI*207/180));
  p_d16.lineTo(400+340*cos(M_PI*207/180), 400-340*sin(M_PI*207/180));
  p_d16.arcTo(400-340, 400-340, 2*340, 2*340, 207, 18);
  p_d16.lineTo(400+320*cos(M_PI*225/180), 400-320*sin(M_PI*225/180));
  p_d16.arcTo(400-320, 400-320, 2*320, 2*320, 225, -18);

  p_d15.moveTo(400+320*cos(M_PI*315/180), 400-320*sin(M_PI*315/180));
  p_d15.lineTo(400+340*cos(M_PI*315/180), 400-340*sin(M_PI*315/180));
  p_d15.arcTo(400-340, 400-340, 2*340, 2*340, 315, 18);
  p_d15.lineTo(400+320*cos(M_PI*333/180), 400-320*sin(M_PI*333/180));
  p_d15.arcTo(400-320, 400-320, 2*320, 2*320, 333, -18);

  p_d14.moveTo(400+320*cos(M_PI*153/180), 400-320*sin(M_PI*153/180));
  p_d14.lineTo(400+340*cos(M_PI*153/180), 400-340*sin(M_PI*153/180));
  p_d14.arcTo(400-340, 400-340, 2*340, 2*340, 153, 18);
  p_d14.lineTo(400+320*cos(M_PI*171/180), 400-320*sin(M_PI*171/180));
  p_d14.arcTo(400-320, 400-320, 2*320, 2*320, 171, -18);

  p_d13.moveTo(400+320*cos(M_PI*9/180), 400-320*sin(M_PI*9/180));
  p_d13.lineTo(400+340*cos(M_PI*9/180), 400-340*sin(M_PI*9/180));
  p_d13.arcTo(400-340, 400-340, 2*340, 2*340, 9, 18);
  p_d13.lineTo(400+320*cos(M_PI*27/180), 400-320*sin(M_PI*27/180));
  p_d13.arcTo(400-320, 400-320, 2*320, 2*320, 27, -18);

  p_d12.moveTo(400+320*cos(M_PI*117/180), 400-320*sin(M_PI*117/180));
  p_d12.lineTo(400+340*cos(M_PI*117/180), 400-340*sin(M_PI*117/180));
  p_d12.arcTo(400-340, 400-340, 2*340, 2*340, 117, 18);
  p_d12.lineTo(400+320*cos(M_PI*135/180), 400-320*sin(M_PI*135/180));
  p_d12.arcTo(400-320, 400-320, 2*320, 2*320, 135, -18);

  p_d11.moveTo(400+320*cos(M_PI*171/180), 400-320*sin(M_PI*171/180));
  p_d11.lineTo(400+340*cos(M_PI*171/180), 400-340*sin(M_PI*171/180));
  p_d11.arcTo(400-340, 400-340, 2*340, 2*340, 171, 18);
  p_d11.lineTo(400+320*cos(M_PI*189/180), 400-320*sin(M_PI*189/180));
  p_d11.arcTo(400-320, 400-320, 2*320, 2*320, 189, -18);

  p_d10.moveTo(400+320*cos(M_PI*333/180), 400-320*sin(M_PI*333/180));
  p_d10.lineTo(400+340*cos(M_PI*333/180), 400-340*sin(M_PI*333/180));
  p_d10.arcTo(400-340, 400-340, 2*340, 2*340, 333, 18);
  p_d10.lineTo(400+320*cos(M_PI*351/180), 400-320*sin(M_PI*351/180));
  p_d10.arcTo(400-320, 400-320, 2*320, 2*320, 351, -18);

  p_d9.moveTo(400+320*cos(M_PI*135/180), 400-320*sin(M_PI*135/180));
  p_d9.lineTo(400+340*cos(M_PI*135/180), 400-340*sin(M_PI*135/180));
  p_d9.arcTo(400-340, 400-340, 2*340, 2*340, 135, 18);
  p_d9.lineTo(400+320*cos(M_PI*153/180), 400-320*sin(M_PI*153/180));
  p_d9.arcTo(400-320, 400-320, 2*320, 2*320, 153, -18);

  p_d8.moveTo(400+320*cos(M_PI*189/180), 400-320*sin(M_PI*189/180));
  p_d8.lineTo(400+340*cos(M_PI*189/180), 400-340*sin(M_PI*189/180));
  p_d8.arcTo(400-340, 400-340, 2*340, 2*340, 189, 18);
  p_d8.lineTo(400+320*cos(M_PI*207/180), 400-320*sin(M_PI*207/180));
  p_d8.arcTo(400-320, 400-320, 2*320, 2*320, 207, -18);

  p_d7.moveTo(400+320*cos(M_PI*225/180), 400-320*sin(M_PI*225/180));
  p_d7.lineTo(400+340*cos(M_PI*225/180), 400-340*sin(M_PI*225/180));
  p_d7.arcTo(400-340, 400-340, 2*340, 2*340, 225, 18);
  p_d7.lineTo(400+320*cos(M_PI*243/180), 400-320*sin(M_PI*243/180));
  p_d7.arcTo(400-320, 400-320, 2*320, 2*320, 243, -18);

  p_d6.moveTo(400+320*cos(M_PI*351/180), 400-320*sin(M_PI*351/180));
  p_d6.lineTo(400+340*cos(M_PI*351/180), 400-340*sin(M_PI*351/180));
  p_d6.arcTo(400-340, 400-340, 2*340, 2*340, 351, 18);
  p_d6.lineTo(400+320*cos(M_PI*9/180), 400-320*sin(M_PI*9/180));
  p_d6.arcTo(400-320, 400-320, 2*320, 2*320, 9, -18);

  p_d5.moveTo(400+320*cos(M_PI*99/180), 400-320*sin(M_PI*99/180));
  p_d5.lineTo(400+340*cos(M_PI*99/180), 400-340*sin(M_PI*99/180));
  p_d5.arcTo(400-340, 400-340, 2*340, 2*340, 99, 18);
  p_d5.lineTo(400+320*cos(M_PI*117/180), 400-320*sin(M_PI*117/180));
  p_d5.arcTo(400-320, 400-320, 2*320, 2*320, 117, -18);

  p_d4.moveTo(400+320*cos(M_PI*27/180), 400-320*sin(M_PI*27/180));
  p_d4.lineTo(400+340*cos(M_PI*27/180), 400-340*sin(M_PI*27/180));
  p_d4.arcTo(400-340, 400-340, 2*340, 2*340, 27, 18);
  p_d4.lineTo(400+320*cos(M_PI*45/180), 400-320*sin(M_PI*45/180));
  p_d4.arcTo(400-320, 400-320, 2*320, 2*320, 45, -18);

  p_d3.moveTo(400+320*cos(M_PI*261/180), 400-320*sin(M_PI*261/180));
  p_d3.lineTo(400+340*cos(M_PI*261/180), 400-340*sin(M_PI*261/180));
  p_d3.arcTo(400-340, 400-340, 2*340, 2*340, 261, 18);
  p_d3.lineTo(400+320*cos(M_PI*279/180), 400-320*sin(M_PI*279/180));
  p_d3.arcTo(400-320, 400-320, 2*320, 2*320, 279, -18);

  p_d2.moveTo(400+320*cos(M_PI*297/180), 400-320*sin(M_PI*297/180));
  p_d2.lineTo(400+340*cos(M_PI*297/180), 400-340*sin(M_PI*297/180));
  p_d2.arcTo(400-340, 400-340, 2*340, 2*340, 297, 18);
  p_d2.lineTo(400+320*cos(M_PI*315/180), 400-320*sin(M_PI*315/180));
  p_d2.arcTo(400-320, 400-320, 2*320, 2*320, 315, -18);

  p_d1.moveTo(400+320*cos(M_PI*63/180), 400-320*sin(M_PI*63/180));
  p_d1.lineTo(400+340*cos(M_PI*63/180), 400-340*sin(M_PI*63/180));
  p_d1.arcTo(400-340, 400-340, 2*340, 2*340, 63, 18);
  p_d1.lineTo(400+320*cos(M_PI*81/180), 400-320*sin(M_PI*81/180));
  p_d1.arcTo(400-320, 400-320, 2*320, 2*320, 81, -18);

  p_dbull.moveTo(400+20, 400);
  p_dbull.arcTo(400-20, 400-20, 2*20, 2*20, 0, 360);

  p_t20.moveTo(400+180*cos(M_PI*81/180), 400-180*sin(M_PI*81/180));
  p_t20.lineTo(400+200*cos(M_PI*81/180), 400-200*sin(M_PI*81/180));
  p_t20.arcTo(400-200, 400-200, 2*200, 2*200, 81, 18);
  p_t20.lineTo(400+180*cos(M_PI*99/180), 400-180*sin(M_PI*99/180));
  p_t20.arcTo(400-180, 400-180, 2*180, 2*180, 99, -18);

  p_t19.moveTo(400+180*cos(M_PI*243/180), 400-180*sin(M_PI*243/180));
  p_t19.lineTo(400+200*cos(M_PI*243/180), 400-200*sin(M_PI*243/180));
  p_t19.arcTo(400-200, 400-200, 2*200, 2*200, 243, 18);
  p_t19.lineTo(400+180*cos(M_PI*261/180), 400-180*sin(M_PI*261/180));
  p_t19.arcTo(400-180, 400-180, 2*180, 2*180, 261, -18);

  p_t18.moveTo(400+180*cos(M_PI*45/180), 400-180*sin(M_PI*45/180));
  p_t18.lineTo(400+200*cos(M_PI*45/180), 400-200*sin(M_PI*45/180));
  p_t18.arcTo(400-200, 400-200, 2*200, 2*200, 45, 18);
  p_t18.lineTo(400+180*cos(M_PI*63/180), 400-180*sin(M_PI*63/180));
  p_t18.arcTo(400-180, 400-180, 2*180, 2*180, 63, -18);

  p_t17.moveTo(400+180*cos(M_PI*279/180), 400-180*sin(M_PI*279/180));
  p_t17.lineTo(400+200*cos(M_PI*279/180), 400-200*sin(M_PI*279/180));
  p_t17.arcTo(400-200, 400-200, 2*200, 2*200, 279, 18);
  p_t17.lineTo(400+180*cos(M_PI*297/180), 400-180*sin(M_PI*297/180));
  p_t17.arcTo(400-180, 400-180, 2*180, 2*180, 297, -18);

  p_t16.moveTo(400+180*cos(M_PI*207/180), 400-180*sin(M_PI*207/180));
  p_t16.lineTo(400+200*cos(M_PI*207/180), 400-200*sin(M_PI*207/180));
  p_t16.arcTo(400-200, 400-200, 2*200, 2*200, 207, 18);
  p_t16.lineTo(400+180*cos(M_PI*225/180), 400-180*sin(M_PI*225/180));
  p_t16.arcTo(400-180, 400-180, 2*180, 2*180, 225, -18);

  p_t15.moveTo(400+180*cos(M_PI*315/180), 400-180*sin(M_PI*315/180));
  p_t15.lineTo(400+200*cos(M_PI*315/180), 400-200*sin(M_PI*315/180));
  p_t15.arcTo(400-200, 400-200, 2*200, 2*200, 315, 18);
  p_t15.lineTo(400+180*cos(M_PI*333/180), 400-180*sin(M_PI*333/180));
  p_t15.arcTo(400-180, 400-180, 2*180, 2*180, 333, -18);

  p_t14.moveTo(400+180*cos(M_PI*153/180), 400-180*sin(M_PI*153/180));
  p_t14.lineTo(400+200*cos(M_PI*153/180), 400-200*sin(M_PI*153/180));
  p_t14.arcTo(400-200, 400-200, 2*200, 2*200, 153, 18);
  p_t14.lineTo(400+180*cos(M_PI*171/180), 400-180*sin(M_PI*171/180));
  p_t14.arcTo(400-180, 400-180, 2*180, 2*180, 171, -18);

  p_t13.moveTo(400+180*cos(M_PI*9/180), 400-180*sin(M_PI*9/180));
  p_t13.lineTo(400+200*cos(M_PI*9/180), 400-200*sin(M_PI*9/180));
  p_t13.arcTo(400-200, 400-200, 2*200, 2*200, 9, 18);
  p_t13.lineTo(400+180*cos(M_PI*27/180), 400-180*sin(M_PI*27/180));
  p_t13.arcTo(400-180, 400-180, 2*180, 2*180, 27, -18);

  p_t12.moveTo(400+180*cos(M_PI*117/180), 400-180*sin(M_PI*117/180));
  p_t12.lineTo(400+200*cos(M_PI*117/180), 400-200*sin(M_PI*117/180));
  p_t12.arcTo(400-200, 400-200, 2*200, 2*200, 117, 18);
  p_t12.lineTo(400+180*cos(M_PI*135/180), 400-180*sin(M_PI*135/180));
  p_t12.arcTo(400-180, 400-180, 2*180, 2*180, 135, -18);

  p_t11.moveTo(400+180*cos(M_PI*171/180), 400-180*sin(M_PI*171/180));
  p_t11.lineTo(400+200*cos(M_PI*171/180), 400-200*sin(M_PI*171/180));
  p_t11.arcTo(400-200, 400-200, 2*200, 2*200, 171, 18);
  p_t11.lineTo(400+180*cos(M_PI*189/180), 400-180*sin(M_PI*189/180));
  p_t11.arcTo(400-180, 400-180, 2*180, 2*180, 189, -18);

  p_t10.moveTo(400+180*cos(M_PI*333/180), 400-180*sin(M_PI*333/180));
  p_t10.lineTo(400+200*cos(M_PI*333/180), 400-200*sin(M_PI*333/180));
  p_t10.arcTo(400-200, 400-200, 2*200, 2*200, 333, 18);
  p_t10.lineTo(400+180*cos(M_PI*351/180), 400-180*sin(M_PI*351/180));
  p_t10.arcTo(400-180, 400-180, 2*180, 2*180, 351, -18);

  p_t9.moveTo(400+180*cos(M_PI*135/180), 400-180*sin(M_PI*135/180));
  p_t9.lineTo(400+200*cos(M_PI*135/180), 400-200*sin(M_PI*135/180));
  p_t9.arcTo(400-200, 400-200, 2*200, 2*200, 135, 18);
  p_t9.lineTo(400+180*cos(M_PI*153/180), 400-180*sin(M_PI*153/180));
  p_t9.arcTo(400-180, 400-180, 2*180, 2*180, 153, -18);

  p_t8.moveTo(400+180*cos(M_PI*189/180), 400-180*sin(M_PI*189/180));
  p_t8.lineTo(400+200*cos(M_PI*189/180), 400-200*sin(M_PI*189/180));
  p_t8.arcTo(400-200, 400-200, 2*200, 2*200, 189, 18);
  p_t8.lineTo(400+180*cos(M_PI*207/180), 400-180*sin(M_PI*207/180));
  p_t8.arcTo(400-180, 400-180, 2*180, 2*180, 207, -18);

  p_t7.moveTo(400+180*cos(M_PI*225/180), 400-180*sin(M_PI*225/180));
  p_t7.lineTo(400+200*cos(M_PI*225/180), 400-200*sin(M_PI*225/180));
  p_t7.arcTo(400-200, 400-200, 2*200, 2*200, 225, 18);
  p_t7.lineTo(400+180*cos(M_PI*243/180), 400-180*sin(M_PI*243/180));
  p_t7.arcTo(400-180, 400-180, 2*180, 2*180, 243, -18);

  p_t6.moveTo(400+180*cos(M_PI*351/180), 400-180*sin(M_PI*351/180));
  p_t6.lineTo(400+200*cos(M_PI*351/180), 400-200*sin(M_PI*351/180));
  p_t6.arcTo(400-200, 400-200, 2*200, 2*200, 351, 18);
  p_t6.lineTo(400+180*cos(M_PI*9/180), 400-180*sin(M_PI*9/180));
  p_t6.arcTo(400-180, 400-180, 2*180, 2*180, 9, -18);

  p_t5.moveTo(400+180*cos(M_PI*99/180), 400-180*sin(M_PI*99/180));
  p_t5.lineTo(400+200*cos(M_PI*99/180), 400-200*sin(M_PI*99/180));
  p_t5.arcTo(400-200, 400-200, 2*200, 2*200, 99, 18);
  p_t5.lineTo(400+180*cos(M_PI*117/180), 400-180*sin(M_PI*117/180));
  p_t5.arcTo(400-180, 400-180, 2*180, 2*180, 117, -18);

  p_t4.moveTo(400+180*cos(M_PI*27/180), 400-180*sin(M_PI*27/180));
  p_t4.lineTo(400+200*cos(M_PI*27/180), 400-200*sin(M_PI*27/180));
  p_t4.arcTo(400-200, 400-200, 2*200, 2*200, 27, 18);
  p_t4.lineTo(400+180*cos(M_PI*45/180), 400-180*sin(M_PI*45/180));
  p_t4.arcTo(400-180, 400-180, 2*180, 2*180, 45, -18);

  p_t3.moveTo(400+180*cos(M_PI*261/180), 400-180*sin(M_PI*261/180));
  p_t3.lineTo(400+200*cos(M_PI*261/180), 400-200*sin(M_PI*261/180));
  p_t3.arcTo(400-200, 400-200, 2*200, 2*200, 261, 18);
  p_t3.lineTo(400+180*cos(M_PI*279/180), 400-180*sin(M_PI*279/180));
  p_t3.arcTo(400-180, 400-180, 2*180, 2*180, 279, -18);

  p_t2.moveTo(400+180*cos(M_PI*297/180), 400-180*sin(M_PI*297/180));
  p_t2.lineTo(400+200*cos(M_PI*297/180), 400-200*sin(M_PI*297/180));
  p_t2.arcTo(400-200, 400-200, 2*200, 2*200, 297, 18);
  p_t2.lineTo(400+180*cos(M_PI*315/180), 400-180*sin(M_PI*315/180));
  p_t2.arcTo(400-180, 400-180, 2*180, 2*180, 315, -18);

  p_t1.moveTo(400+180*cos(M_PI*63/180), 400-180*sin(M_PI*63/180));
  p_t1.lineTo(400+200*cos(M_PI*63/180), 400-200*sin(M_PI*63/180));
  p_t1.arcTo(400-200, 400-200, 2*200, 2*200, 63, 18);
  p_t1.lineTo(400+180*cos(M_PI*81/180), 400-180*sin(M_PI*81/180));
  p_t1.arcTo(400-180, 400-180, 2*180, 2*180, 81, -18);

  p_noscore.moveTo(800, 400);
  p_noscore.arcTo(0, 0, 800, 800, 0, 360);
  p_noscore.moveTo(400+342, 400);
  p_noscore.arcTo(400-342, 400-342, 342*2, 342*2, 0, 360);

  CDartBoardSegment * s20 = new CDartBoardSegment(p_s20, 20);
  CDartBoardSegment * s19 = new CDartBoardSegment(p_s19, 19, "beige");
  CDartBoardSegment * s18 = new CDartBoardSegment(p_s18, 18);
  CDartBoardSegment * s17 = new CDartBoardSegment(p_s17, 17, "beige");
  CDartBoardSegment * s14 = new CDartBoardSegment(p_s14, 0);
  CDartBoardSegment * s13 = new CDartBoardSegment(p_s13, 0);
  CDartBoardSegment * s12 = new CDartBoardSegment(p_s12, 0);
  CDartBoardSegment * s11 = new CDartBoardSegment(p_s11, 0, "beige");
  CDartBoardSegment * s16 = new CDartBoardSegment(p_s16, 16, "beige");
  CDartBoardSegment * s15 = new CDartBoardSegment(p_s15, 15, "beige");
  CDartBoardSegment * s10 = new CDartBoardSegment(p_s10, 0);
  CDartBoardSegment * s9 = new CDartBoardSegment(p_s9, 0, "beige");
  CDartBoardSegment * s8 = new CDartBoardSegment(p_s8, 0);
  CDartBoardSegment * s7 = new CDartBoardSegment(p_s7, 0);
  CDartBoardSegment * s6 = new CDartBoardSegment(p_s6, 0, "beige");
  CDartBoardSegment * s5 = new CDartBoardSegment(p_s5, 0, "beige");
  CDartBoardSegment * s4 = new CDartBoardSegment(p_s4, 0, "beige");
  CDartBoardSegment * s3 = new CDartBoardSegment(p_s3, 0);
  CDartBoardSegment * s2 = new CDartBoardSegment(p_s2, 0);
  CDartBoardSegment * s1 = new CDartBoardSegment(p_s1, 0, "beige");
  CDartBoardSegment * sbull = new CDartBoardSegment(p_sbull, 25, "green");
  CDartBoardSegment * d20 = new CDartBoardSegment(p_d20, 40, "red", 'd');
  CDartBoardSegment * d19 = new CDartBoardSegment(p_d19, 38, "green", 'd');
  CDartBoardSegment * d18 = new CDartBoardSegment(p_d18, 36, "red", 'd');
  CDartBoardSegment * d17 = new CDartBoardSegment(p_d17, 34, "green", 'd');
  CDartBoardSegment * d16 = new CDartBoardSegment(p_d16, 32, "green", 'd');
  CDartBoardSegment * d15 = new CDartBoardSegment(p_d15, 30, "green", 'd');
  CDartBoardSegment * d14 = new CDartBoardSegment(p_d14, 0, "red", 'd');
  CDartBoardSegment * d13 = new CDartBoardSegment(p_d13, 0, "red", 'd');
  CDartBoardSegment * d12 = new CDartBoardSegment(p_d12, 0, "red", 'd');
  CDartBoardSegment * d11 = new CDartBoardSegment(p_d11, 0, "green", 'd');
  CDartBoardSegment * d10 = new CDartBoardSegment(p_d10, 0, "red", 'd');
  CDartBoardSegment * d9 = new CDartBoardSegment(p_d9, 0, "green", 'd');
  CDartBoardSegment * d8 = new CDartBoardSegment(p_d8, 0, "red", 'd');
  CDartBoardSegment * d7 = new CDartBoardSegment(p_d7, 0, "red", 'd');
  CDartBoardSegment * d6 = new CDartBoardSegment(p_d6, 0, "green", 'd');
  CDartBoardSegment * d5 = new CDartBoardSegment(p_d5, 0, "green", 'd');
  CDartBoardSegment * d4 = new CDartBoardSegment(p_d4, 0, "green", 'd');
  CDartBoardSegment * d3 = new CDartBoardSegment(p_d3, 0, "red", 'd');
  CDartBoardSegment * d2 = new CDartBoardSegment(p_d2, 0, "red", 'd');
  CDartBoardSegment * d1 = new CDartBoardSegment(p_d1, 0, "green", 'd');
  CDartBoardSegment * dbull = new CDartBoardSegment(p_dbull, 50, "red", 'd');
  CDartBoardSegment * t20 = new CDartBoardSegment(p_t20, 60, "red", 't');
  CDartBoardSegment * t19 = new CDartBoardSegment(p_t19, 57, "green", 't');
  CDartBoardSegment * t18 = new CDartBoardSegment(p_t18, 54, "red", 't');
  CDartBoardSegment * t17 = new CDartBoardSegment(p_t17, 51, "green", 't');
  CDartBoardSegment * t16 = new CDartBoardSegment(p_t16, 48, "green", 't');
  CDartBoardSegment * t15 = new CDartBoardSegment(p_t15, 45, "green", 't');
  CDartBoardSegment * t14 = new CDartBoardSegment(p_t14, 0, "red", 't');
  CDartBoardSegment * t13 = new CDartBoardSegment(p_t13, 0, "red", 't');
  CDartBoardSegment * t12 = new CDartBoardSegment(p_t12, 0, "red", 't');
  CDartBoardSegment * t11 = new CDartBoardSegment(p_t11, 0, "green", 't');
  CDartBoardSegment * t10 = new CDartBoardSegment(p_t10, 0, "red", 't');
  CDartBoardSegment * t9 = new CDartBoardSegment(p_t9, 0, "green", 't');
  CDartBoardSegment * t8 = new CDartBoardSegment(p_t8, 0, "red", 't');
  CDartBoardSegment * t7 = new CDartBoardSegment(p_t7, 0, "red", 't');
  CDartBoardSegment * t6 = new CDartBoardSegment(p_t6, 0, "green", 't');
  CDartBoardSegment * t5 = new CDartBoardSegment(p_t5, 0, "green", 't');
  CDartBoardSegment * t4 = new CDartBoardSegment(p_t4, 0, "green", 't');
  CDartBoardSegment * t3 = new CDartBoardSegment(p_t3, 0, "red", 't');
  CDartBoardSegment * t2 = new CDartBoardSegment(p_t2, 0, "red", 't');
  CDartBoardSegment * t1 = new CDartBoardSegment(p_t1, 0, "green", 't');
  CDartBoardSegment * noscore = new CDartBoardSegment(p_noscore);
  QGraphicsSvgItem * labels = new QGraphicsSvgItem(":/resources/img/dartboard.svg");
  labels->setElementId("label");

  mScene->addItem(noscore);
  mScene->addItem(s20);
  mScene->addItem(s19);
  mScene->addItem(s18);
  mScene->addItem(s17);
  mScene->addItem(s16);
  mScene->addItem(s15);
  mScene->addItem(s14);
  mScene->addItem(s13);
  mScene->addItem(s12);
  mScene->addItem(s11);
  mScene->addItem(s10);
  mScene->addItem(s9);
  mScene->addItem(s8);
  mScene->addItem(s7);
  mScene->addItem(s6);
  mScene->addItem(s5);
  mScene->addItem(s4);
  mScene->addItem(s3);
  mScene->addItem(s2);
  mScene->addItem(s1);
  mScene->addItem(sbull);
  mScene->addItem(d20);
  mScene->addItem(d19);
  mScene->addItem(d18);
  mScene->addItem(d17);
  mScene->addItem(d16);
  mScene->addItem(d15);
  mScene->addItem(d14);
  mScene->addItem(d13);
  mScene->addItem(d12);
  mScene->addItem(d11);
  mScene->addItem(d10);
  mScene->addItem(d9);
  mScene->addItem(d8);
  mScene->addItem(d7);
  mScene->addItem(d6);
  mScene->addItem(d5);
  mScene->addItem(d4);
  mScene->addItem(d3);
  mScene->addItem(d2);
  mScene->addItem(d1);
  mScene->addItem(dbull);
  mScene->addItem(t20);
  mScene->addItem(t19);
  mScene->addItem(t18);
  mScene->addItem(t17);
  mScene->addItem(t16);
  mScene->addItem(t15);
  mScene->addItem(t14);
  mScene->addItem(t13);
  mScene->addItem(t12);
  mScene->addItem(t11);
  mScene->addItem(t10);
  mScene->addItem(t9);
  mScene->addItem(t8);
  mScene->addItem(t7);
  mScene->addItem(t6);
  mScene->addItem(t5);
  mScene->addItem(t4);
  mScene->addItem(t3);
  mScene->addItem(t2);
  mScene->addItem(t1);
  mScene->addItem(labels);
  labels->setZValue(1);
  labels->setScale(0.98);
  labels->setPos(10, 10);

  connect(s20, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s19, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s18, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s17, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s16, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s15, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s14, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s13, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s12, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s11, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s10, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s9, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s8, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s7, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s6, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s5, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s4, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s3, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s2, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(s1, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(sbull, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d20, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d19, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d18, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d17, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d16, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d15, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d14, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d13, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d12, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d11, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d10, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d9, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d8, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d7, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d6, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d5, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d4, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d3, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d2, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(d1, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(dbull, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t20, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t19, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t18, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t17, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t16, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t15, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t14, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t13, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t12, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t11, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t10, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t9, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t8, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t7, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t6, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t5, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t4, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t3, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t2, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(t1, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  connect(noscore, SIGNAL (signal_segment_pressed(int&, QChar&)), this, SLOT (signal_segment_pressed(int&, QChar&)));
  mGameShotSound.setSource(QUrl("qrc:/resources/sounds/gameshot.wav"));
}

CCricketInput::~CCricketInput()
{
  delete mUi;
  delete mScene;
}

void CCricketInput::set_score(uint32_t iVal, QChar iType)
{
  if (iType == 't')
  {
    iVal = iVal / 3;
  }
  else if (iType == 'd')
  {
    iVal = iVal / 2;
  }
  if (iVal > 0)
  {
    if (mCounter == 3)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart1->setText(temp);
      mDarts[0] = iType + QString::number(iVal);
    }
    else if (mCounter == 2)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart2->setText(temp);
      mDarts[1] = iType + QString::number(iVal);
    }
    else if (mCounter == 1)
    {
      QString temp = iType.toUpper()+QString::number(iVal);
      mUi->labelScoreDart3->setText(temp);
      mDarts[2] = iType + QString::number(iVal);
    }
  }
  else
  {
    if (mCounter == 3)
    {
      mUi->labelScoreDart1->setText("X");
      mDarts[0] = iType + QString::number(iVal);
    }
    else if (mCounter == 2)
    {
      mUi->labelScoreDart2->setText("X");
      mDarts[1] = iType + QString::number(iVal);
    }
    else if (mCounter == 1)
    {
      mUi->labelScoreDart3->setText("X");
      mDarts[2] = iType + QString::number(iVal);
    }
  }
}

void CCricketInput::compute_score()
{
  mScore = mExtra15 + mExtra16 + mExtra17 + mExtra18 + mExtra19 + mExtra20 + mExtra25;
}

bool CCricketInput::are_slots_full() const
{
  bool full15 = mSlot15 == 3;
  bool full16 = mSlot16 == 3;
  bool full17 = mSlot17 == 3;
  bool full18 = mSlot18 == 3;
  bool full19 = mSlot19 == 3;
  bool full20 = mSlot20 == 3;
  bool full25 = mSlot25 == 3;

  return full15 && full16 && full17 && full18 && full19 && full20 && full25;
}

void CCricketInput::signal_segment_pressed(uint32_t iVal, QChar & iType)
{
  bool result = true;
  QString dart = iType.toUpper()+QString::number(iVal);
  if (!mCutThroat)
  {
    if (!mStop)
    {
      if (mCounter > 0)
      {
        set_score(iVal, iType);
        uint32_t hits;
        if (iType == 't')
        {
            hits = 3;
        }
        else if (iType == 'd')
        {
            hits = 2;
        }
        else
        {
            hits = 1;
        }
        uint32_t val = iVal;
        val /= hits;
        switch (val)
        {
        case 15:
          if (mSlot15 < 3)
          {
            if (mSlot15 + hits <= 3)
            {
              mSlot15 += hits;
            }
            else
            {
              hits -= 3 - mSlot15;
              mSlot15 = 3;
              if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
              {
                mExtra15 += hits * 15;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
            {
                mExtra15 += hits * 15;
            }
          }
          break;
        case 16:
          if (mSlot16 < 3)
          {
            if (mSlot16 + hits <= 3)
            {
              mSlot16 += hits;
            }
            else
            {
              hits -= 3 - mSlot16;
              mSlot16 = 3;
              if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
              {
                mExtra16 += hits * 16;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
            {
              mExtra16 += hits * 16;
            }
          }
          break;
        case 17:
          if (mSlot17 < 3)
          {
            if (mSlot17 + hits <= 3)
            {
              mSlot17 += hits;
            }
            else
            {
              hits -= 3 - mSlot17;
              mSlot17 = 3;
              if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
              {
                mExtra17 += hits * 17;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
            {
              mExtra17 += hits * 17;
            }
          }
          break;
        case 18:
          if (mSlot18 < 3)
          {
            if (mSlot18 + hits <= 3)
            {
              mSlot18 += hits;
            }
            else
            {
              hits -= 3 - mSlot18;
              mSlot18 = 3;
              if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
              {
                mExtra18 += hits * 18;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
            {
              mExtra18 += hits * 18;
            }
          }
          break;
        case 19:
          if (mSlot19 < 3)
          {
            if (mSlot19 + hits <= 3)
            {
              mSlot19 += hits;
            }
            else
            {
              hits -= 3 - mSlot19;
              mSlot19 = 3;
              if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
              {
                mExtra19 += hits * 19;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
            {
              mExtra19 += hits * 19;
            }
          }
          break;
        case 20:
          if (mSlot20 < 3)
          {
            if (mSlot20 + hits <= 3)
            {
              mSlot20 += hits;
            }
            else
            {
              hits -= 3 - mSlot20;
              mSlot20 = 3;
              if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
              {
                mExtra20 += hits * 20;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
            {
              mExtra20 += hits * 20;
            }
          }
          break;
        case 25:
          if (mSlot25 < 3)
          {
            if (mSlot25 + hits <= 3)
            {
              mSlot25 += hits;
            }
            else
            {
              hits -= 3 - mSlot25;
              mSlot25 = 3;
              if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
              {
                mExtra25 += hits * 25;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
            {
              mExtra25 += hits * 25;
            }
          }
          break;
          default:;
        }

        compute_score();
        if (are_slots_full() && mGameWindow->is_score_bigger(mScore))
        {
          mStop = true;  // Game shot
          mGameShotSound.play();
          mUi->submitButton->setFocus();
        }

        mCounter--;
        if (mCounter == 0)
        {
          mStop = true;
          mUi->submitButton->setFocus();
        }
      }
    }
    else if (are_slots_full() && mGameWindow->is_score_bigger(mScore))
    {
      QMessageBox::warning(this, "Warning", "You have already won this leg!");
    }
    else
    {
      QMessageBox::warning(this, "Warning", "You only have three darts!");
    }
  }
  else
  {
    if (!mStop)
    {
      if (mCounter > 0)
      {
        set_score(iVal, iType);
        uint32_t hits;
        if (iType == 't')
        {
          hits = 3;
        }
        else if (iType == 'd')
        {
          hits = 2;
        }
        else
        {
          hits = 1;
        }
        uint32_t val = iVal;
        val /= hits;
        switch (val)
        {
        case 15:
          if (mSlot15 < 3)
          {
            if (mSlot15 + hits <= 3)
            {
              mSlot15 += hits;
            }
            else
            {
              hits -= 3 - mSlot15;
              mSlot15 = 3;
              if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
              {
                mExtra15 += hits * 15;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot15_free(mPlayer->get_player_number()))
            {
              mExtra15 += hits * 15;
            }
          }
          break;
        case 16:
          if (mSlot16 < 3)
          {
            if (mSlot16 + hits <= 3)
            {
              mSlot16 += hits;
            }
            else
            {
              hits -= 3 - mSlot16;
              mSlot16 = 3;
              if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
              {
                mExtra16 += hits * 16;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot16_free(mPlayer->get_player_number()))
            {
              mExtra16 += hits * 16;
            }
          }
          break;
        case 17:
          if (mSlot17 < 3)
          {
            if (mSlot17 + hits <= 3)
            {
              mSlot17 += hits;
            }
            else
            {
              hits -= 3 - mSlot17;
              mSlot17 = 3;
              if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
              {
                mExtra17 += hits * 17;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot17_free(mPlayer->get_player_number()))
            {
              mExtra17 += hits * 17;
            }
          }
          break;
        case 18:
          if (mSlot18 < 3)
          {
            if (mSlot18 + hits <= 3)
            {
              mSlot18 += hits;
            }
            else
            {
              hits -= 3 - mSlot18;
              mSlot18 = 3;
              if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
              {
                mExtra18 += hits * 18;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot18_free(mPlayer->get_player_number()))
            {
              mExtra18 += hits * 18;
            }
          }
          break;
        case 19:
          if (mSlot19 < 3)
          {
            if (mSlot19 + hits <= 3)
            {
              mSlot19 += hits;
            }
            else
            {
              hits -= 3 - mSlot19;
              mSlot19 = 3;
              if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
              {
                mExtra19 += hits * 19;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot19_free(mPlayer->get_player_number()))
            {
              mExtra19 += hits * 19;
            }
          }
          break;
        case 20:
          if (mSlot20 < 3)
          {
            if (mSlot20 + hits <= 3)
            {
              mSlot20 += hits;
            }
            else
            {
              hits -= 3 - mSlot20;
              mSlot20 = 3;
              if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
              {
                mExtra20 += hits * 20;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot20_free(mPlayer->get_player_number()))
            {
              mExtra20 += hits * 20;
            }
          }
          break;
        case 25:
          if (mSlot25 < 3)
          {
            if (mSlot25 + hits <= 3)
            {
              mSlot25 += hits;
            }
            else
            {
              hits -= 3 - mSlot25;
              mSlot25 = 3;
              if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
              {
                mExtra25 += hits * 25;
              }
            }
          }
          else
          {
            if (mGameWindow->is_slot25_free(mPlayer->get_player_number()))
            {
              mExtra25 += hits * 25;
            }
          }
          break;
          default:;
        }

        QVector<uint32_t> temp15s = mGameWindow->compute_extra15s(mExtra15, mPlayer->get_player_number());
        QVector<uint32_t> temp16s = mGameWindow->compute_extra16s(mExtra16, mPlayer->get_player_number());
        QVector<uint32_t> temp17s = mGameWindow->compute_extra17s(mExtra17, mPlayer->get_player_number());
        QVector<uint32_t> temp18s = mGameWindow->compute_extra18s(mExtra18, mPlayer->get_player_number());
        QVector<uint32_t> temp19s = mGameWindow->compute_extra19s(mExtra19, mPlayer->get_player_number());
        QVector<uint32_t> temp20s = mGameWindow->compute_extra20s(mExtra20, mPlayer->get_player_number());
        QVector<uint32_t> temp25s = mGameWindow->compute_extra25s(mExtra25, mPlayer->get_player_number());
        QVector<uint32_t> scores(temp15s.size());

        for (uint32_t i = 0; i < mExtra15s.size(); i++)
        {
          mExtra15s[i] += temp15s[i];
          mExtra16s[i] += temp16s[i];
          mExtra17s[i] += temp17s[i];
          mExtra18s[i] += temp18s[i];
          mExtra19s[i] += temp19s[i];
          mExtra20s[i] += temp20s[i];
          mExtra25s[i] += temp25s[i];
          scores[i] = temp15s[i] + temp16s[i] + temp17s[i] + temp18s[i] + temp19s[i] + temp20s[i] + temp25s[i];
        }

        mExtra15sArray.push_back(mExtra15s);
        mExtra16sArray.push_back(mExtra16s);
        mExtra17sArray.push_back(mExtra17s);
        mExtra18sArray.push_back(mExtra18s);
        mExtra19sArray.push_back(mExtra19s);
        mExtra20sArray.push_back(mExtra20s);
        mExtra25sArray.push_back(mExtra25s);

        for (uint32_t i = 0; i < scores.size(); i++)
        {
          result = result && scores[i] >= mScore;
        }

        if (are_slots_full() && result)
        {
          mStop = true;  // Game shot
          mGameShotSound.play();
          mUi->submitButton->setFocus();
        }

        mCounter--;
        if (mCounter == 0)
        {
          mStop = true;
          mUi->submitButton->setFocus();
        }
      }
    }
    else if (are_slots_full() && result)
    {
      QMessageBox::warning(this, "Warning", "You have already won this leg!");
    }
    else
    {
      QMessageBox::warning(this, "Warning", "You only have three darts!");
    }
  }

  mSlot15Array.push_back(mSlot15);
  mSlot16Array.push_back(mSlot16);
  mSlot17Array.push_back(mSlot17);
  mSlot18Array.push_back(mSlot18);
  mSlot19Array.push_back(mSlot19);
  mSlot20Array.push_back(mSlot20);
  mSlot25Array.push_back(mSlot25);
  mExtra15Array.push_back(mExtra15);
  mExtra16Array.push_back(mExtra16);
  mExtra17Array.push_back(mExtra17);
  mExtra18Array.push_back(mExtra18);
  mExtra19Array.push_back(mExtra19);
  mExtra20Array.push_back(mExtra20);
  mExtra25Array.push_back(mExtra25);
}

void CCricketInput::on_submitButton_clicked()
{
  if (mStop)
  {
    uint32_t numberofdarts = 3 - mCounter;
    emit signal_cricket_submit_button_pressed(numberofdarts, mDarts);
  }
  else
  {
    QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
  }
}

void CCricketInput::on_undoButton_clicked()
{
  if (mCounter < 3)
  {
    mDarts[2-mCounter] = "";
    mSlot15Array.pop_back();
    mSlot16Array.pop_back();
    mSlot17Array.pop_back();
    mSlot18Array.pop_back();
    mSlot19Array.pop_back();
    mSlot20Array.pop_back();
    mSlot25Array.pop_back();
    mExtra15Array.pop_back();
    mExtra16Array.pop_back();
    mExtra17Array.pop_back();
    mExtra18Array.pop_back();
    mExtra19Array.pop_back();
    mExtra20Array.pop_back();
    mExtra25Array.pop_back();
    mSlot15 = mSlot15Array.back();
    mSlot16 = mSlot16Array.back();
    mSlot17 = mSlot17Array.back();
    mSlot18 = mSlot18Array.back();
    mSlot19 = mSlot19Array.back();
    mSlot20 = mSlot20Array.back();
    mSlot25 = mSlot25Array.back();
    mExtra15 = mExtra15Array.back();
    mExtra16 = mExtra16Array.back();
    mExtra17 = mExtra17Array.back();
    mExtra18 = mExtra18Array.back();
    mExtra19 = mExtra19Array.back();
    mExtra20 = mExtra20Array.back();
    mExtra25 = mExtra25Array.back();

    if (mCutThroat)
    {
      mExtra15sArray.pop_back();
      mExtra16sArray.pop_back();
      mExtra17sArray.pop_back();
      mExtra18sArray.pop_back();
      mExtra19sArray.pop_back();
      mExtra20sArray.pop_back();
      mExtra25sArray.pop_back();
      mExtra15s = mExtra15sArray.back();
      mExtra16s = mExtra16sArray.back();
      mExtra17s = mExtra17sArray.back();
      mExtra18s = mExtra18sArray.back();
      mExtra19s = mExtra19sArray.back();
      mExtra20s = mExtra20sArray.back();
      mExtra25s = mExtra25sArray.back();
    }

    if (mCounter == 2)
    {
      mUi->labelScoreDart1->setText("---");
    }
    else if (mCounter == 1)
    {
      mUi->labelScoreDart2->setText("---");
    }
    else if (mCounter == 0)
    {
      mUi->labelScoreDart3->setText("---");
    }
    mCounter++;
    mStop = false;
  }
}
