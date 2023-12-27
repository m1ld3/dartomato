#include "dartboard.h"
#include <cmath>
#include <QMessageBox>

CDartBoard::CDartBoard(CDartBoardView * iGraphicsViewDartBoard, uint32_t iStartVal, uint32_t iScore, bool iSingleIn, bool iSingleOut,
                       bool iDoubleIn, bool iDoubleOut, bool iMasterIn, bool iMasterOut)
  : mStartVal(iStartVal)
  , mScore(iScore)
  , mCounter(3)
  , mSingleIn(iSingleIn)
  , mSingleOut(iSingleOut)
  , mDoubleIn(iDoubleIn)
  , mDoubleOut(iDoubleOut)
  , mMasterIn(iMasterIn)
  , mMasterOut(iMasterOut)
  , mFinished(false)
  , mDarts({})
  , mBustedSound(this)
  , mGameShotSound(this)
{
  mCheckoutAttempts = {0, 0, 0};
  mScene = new QGraphicsScene(0, 0, 800, 800, iGraphicsViewDartBoard);
  mScene->setSceneRect(0, 0, 800, 800);
  iGraphicsViewDartBoard->setRenderHint(QPainter::SmoothPixmapTransform, true);
  iGraphicsViewDartBoard->setRenderHint(QPainter::Antialiasing, true);
  iGraphicsViewDartBoard->setScene(mScene);
  iGraphicsViewDartBoard->show();

  QPainterPath p_s20, p_s19, p_s18, p_s17, p_s16, p_s15, p_s14, p_s13, p_s12, p_s11,
          p_s10, p_s9, p_s8, p_s7, p_s6, p_s5, p_s4,p_s3, p_s2, p_s1, p_sbull,
          p_d20, p_d19, p_d18, p_d17, p_d16, p_d15, p_d14, p_d13, p_d12, p_d11,
          p_d10, p_d9, p_d8, p_d7, p_d6, p_d5, p_d4,p_d3, p_d2, p_d1, p_dbull,
          p_t20, p_t19, p_t18, p_t17, p_t16, p_t15, p_t14, p_t13, p_t12, p_t11,
          p_t10, p_t9, p_t8, p_t7, p_t6, p_t5, p_t4,p_t3, p_t2, p_t1, p_noscore;

  p_s20.moveTo(400+40*cos(M_PI*81/180),400-40*sin(M_PI*81/180));
  p_s20.lineTo(400+180*cos(M_PI*81/180),400-180*sin(M_PI*81/180));
  p_s20.arcTo(400-180,400-180,2*180,2*180,81,18);
  p_s20.lineTo(400+40*cos(M_PI*99/180),400-40*sin(M_PI*99/180));
  p_s20.arcTo(400-40,400-40,2*40,2*40,99,-18);
  p_s20.moveTo(400+200*cos(M_PI*81/180),400-200*sin(M_PI*81/180));
  p_s20.lineTo(400+320*cos(M_PI*81/180),400-320*sin(M_PI*81/180));
  p_s20.arcTo(400-320,400-320,2*320,2*320,81,18);
  p_s20.lineTo(400+320*cos(M_PI*99/180),400-320*sin(M_PI*99/180));
  p_s20.arcTo(400-200,400-200,2*200,2*200,99,-18);

  p_s19.moveTo(400+40*cos(M_PI*243/180),400-40*sin(M_PI*243/180));
  p_s19.lineTo(400+180*cos(M_PI*243/180),400-180*sin(M_PI*243/180));
  p_s19.arcTo(400-180,400-180,2*180,2*180,243,18);
  p_s19.lineTo(400+40*cos(M_PI*261/180),400-40*sin(M_PI*261/180));
  p_s19.arcTo(400-40,400-40,2*40,2*40,261,-18);
  p_s19.moveTo(400+200*cos(M_PI*243/180),400-200*sin(M_PI*243/180));
  p_s19.lineTo(400+320*cos(M_PI*243/180),400-320*sin(M_PI*243/180));
  p_s19.arcTo(400-320,400-320,2*320,2*320,243,18);
  p_s19.lineTo(400+320*cos(M_PI*261/180),400-320*sin(M_PI*261/180));
  p_s19.arcTo(400-200,400-200,2*200,2*200,261,-18);

  p_s18.moveTo(400+40*cos(M_PI*45/180),400-40*sin(M_PI*45/180));
  p_s18.lineTo(400+180*cos(M_PI*45/180),400-180*sin(M_PI*45/180));
  p_s18.arcTo(400-180,400-180,2*180,2*180,45,18);
  p_s18.lineTo(400+40*cos(M_PI*63/180),400-40*sin(M_PI*63/180));
  p_s18.arcTo(400-40,400-40,2*40,2*40,63,-18);
  p_s18.moveTo(400+200*cos(M_PI*45/180),400-200*sin(M_PI*45/180));
  p_s18.lineTo(400+320*cos(M_PI*45/180),400-320*sin(M_PI*45/180));
  p_s18.arcTo(400-320,400-320,2*320,2*320,45,18);
  p_s18.lineTo(400+320*cos(M_PI*63/180),400-320*sin(M_PI*63/180));
  p_s18.arcTo(400-200,400-200,2*200,2*200,63,-18);

  p_s17.moveTo(400+40*cos(M_PI*279/180),400-40*sin(M_PI*279/180));
  p_s17.lineTo(400+180*cos(M_PI*279/180),400-180*sin(M_PI*279/180));
  p_s17.arcTo(400-180,400-180,2*180,2*180,279,18);
  p_s17.lineTo(400+40*cos(M_PI*297/180),400-40*sin(M_PI*297/180));
  p_s17.arcTo(400-40,400-40,2*40,2*40,297,-18);
  p_s17.moveTo(400+200*cos(M_PI*279/180),400-200*sin(M_PI*279/180));
  p_s17.lineTo(400+320*cos(M_PI*279/180),400-320*sin(M_PI*279/180));
  p_s17.arcTo(400-320,400-320,2*320,2*320,279,18);
  p_s17.lineTo(400+320*cos(M_PI*297/180),400-320*sin(M_PI*297/180));
  p_s17.arcTo(400-200,400-200,2*200,2*200,297,-18);

  p_s16.moveTo(400+40*cos(M_PI*207/180),400-40*sin(M_PI*207/180));
  p_s16.lineTo(400+180*cos(M_PI*207/180),400-180*sin(M_PI*207/180));
  p_s16.arcTo(400-180,400-180,2*180,2*180,207,18);
  p_s16.lineTo(400+40*cos(M_PI*225/180),400-40*sin(M_PI*225/180));
  p_s16.arcTo(400-40,400-40,2*40,2*40,225,-18);
  p_s16.moveTo(400+200*cos(M_PI*207/180),400-200*sin(M_PI*207/180));
  p_s16.lineTo(400+320*cos(M_PI*207/180),400-320*sin(M_PI*207/180));
  p_s16.arcTo(400-320,400-320,2*320,2*320,207,18);
  p_s16.lineTo(400+320*cos(M_PI*225/180),400-320*sin(M_PI*225/180));
  p_s16.arcTo(400-200,400-200,2*200,2*200,225,-18);

  p_s15.moveTo(400+40*cos(M_PI*315/180),400-40*sin(M_PI*315/180));
  p_s15.lineTo(400+180*cos(M_PI*315/180),400-180*sin(M_PI*315/180));
  p_s15.arcTo(400-180,400-180,2*180,2*180,315,18);
  p_s15.lineTo(400+40*cos(M_PI*333/180),400-40*sin(M_PI*333/180));
  p_s15.arcTo(400-40,400-40,2*40,2*40,333,-18);
  p_s15.moveTo(400+200*cos(M_PI*315/180),400-200*sin(M_PI*315/180));
  p_s15.lineTo(400+320*cos(M_PI*315/180),400-320*sin(M_PI*315/180));
  p_s15.arcTo(400-320,400-320,2*320,2*320,315,18);
  p_s15.lineTo(400+320*cos(M_PI*333/180),400-320*sin(M_PI*333/180));
  p_s15.arcTo(400-200,400-200,2*200,2*200,333,-18);

  p_s14.moveTo(400+40*cos(M_PI*153/180),400-40*sin(M_PI*153/180));
  p_s14.lineTo(400+180*cos(M_PI*153/180),400-180*sin(M_PI*153/180));
  p_s14.arcTo(400-180,400-180,2*180,2*180,153,18);
  p_s14.lineTo(400+40*cos(M_PI*171/180),400-40*sin(M_PI*171/180));
  p_s14.arcTo(400-40,400-40,2*40,2*40,171,-18);
  p_s14.moveTo(400+200*cos(M_PI*153/180),400-200*sin(M_PI*153/180));
  p_s14.lineTo(400+320*cos(M_PI*153/180),400-320*sin(M_PI*153/180));
  p_s14.arcTo(400-320,400-320,2*320,2*320,153,18);
  p_s14.lineTo(400+320*cos(M_PI*171/180),400-320*sin(M_PI*171/180));
  p_s14.arcTo(400-200,400-200,2*200,2*200,171,-18);

  p_s13.moveTo(400+40*cos(M_PI*9/180),400-40*sin(M_PI*9/180));
  p_s13.lineTo(400+180*cos(M_PI*9/180),400-180*sin(M_PI*9/180));
  p_s13.arcTo(400-180,400-180,2*180,2*180,9,18);
  p_s13.lineTo(400+40*cos(M_PI*27/180),400-40*sin(M_PI*27/180));
  p_s13.arcTo(400-40,400-40,2*40,2*40,27,-18);
  p_s13.moveTo(400+200*cos(M_PI*9/180),400-200*sin(M_PI*9/180));
  p_s13.lineTo(400+320*cos(M_PI*9/180),400-320*sin(M_PI*9/180));
  p_s13.arcTo(400-320,400-320,2*320,2*320,9,18);
  p_s13.lineTo(400+320*cos(M_PI*27/180),400-320*sin(M_PI*27/180));
  p_s13.arcTo(400-200,400-200,2*200,2*200,27,-18);

  p_s12.moveTo(400+40*cos(M_PI*117/180),400-40*sin(M_PI*117/180));
  p_s12.lineTo(400+180*cos(M_PI*117/180),400-180*sin(M_PI*117/180));
  p_s12.arcTo(400-180,400-180,2*180,2*180,117,18);
  p_s12.lineTo(400+40*cos(M_PI*135/180),400-40*sin(M_PI*135/180));
  p_s12.arcTo(400-40,400-40,2*40,2*40,135,-18);
  p_s12.moveTo(400+200*cos(M_PI*117/180),400-200*sin(M_PI*117/180));
  p_s12.lineTo(400+320*cos(M_PI*117/180),400-320*sin(M_PI*117/180));
  p_s12.arcTo(400-320,400-320,2*320,2*320,117,18);
  p_s12.lineTo(400+320*cos(M_PI*135/180),400-320*sin(M_PI*135/180));
  p_s12.arcTo(400-200,400-200,2*200,2*200,135,-18);

  p_s11.moveTo(400+40*cos(M_PI*171/180),400-40*sin(M_PI*171/180));
  p_s11.lineTo(400+180*cos(M_PI*171/180),400-180*sin(M_PI*171/180));
  p_s11.arcTo(400-180,400-180,2*180,2*180,171,18);
  p_s11.lineTo(400+40*cos(M_PI*189/180),400-40*sin(M_PI*189/180));
  p_s11.arcTo(400-40,400-40,2*40,2*40,189,-18);
  p_s11.moveTo(400+200*cos(M_PI*171/180),400-200*sin(M_PI*171/180));
  p_s11.lineTo(400+320*cos(M_PI*171/180),400-320*sin(M_PI*171/180));
  p_s11.arcTo(400-320,400-320,2*320,2*320,171,18);
  p_s11.lineTo(400+320*cos(M_PI*189/180),400-320*sin(M_PI*189/180));
  p_s11.arcTo(400-200,400-200,2*200,2*200,189,-18);

  p_s10.moveTo(400+40*cos(M_PI*333/180),400-40*sin(M_PI*333/180));
  p_s10.lineTo(400+180*cos(M_PI*333/180),400-180*sin(M_PI*333/180));
  p_s10.arcTo(400-180,400-180,2*180,2*180,333,18);
  p_s10.lineTo(400+40*cos(M_PI*351/180),400-40*sin(M_PI*351/180));
  p_s10.arcTo(400-40,400-40,2*40,2*40,351,-18);
  p_s10.moveTo(400+200*cos(M_PI*333/180),400-200*sin(M_PI*333/180));
  p_s10.lineTo(400+320*cos(M_PI*333/180),400-320*sin(M_PI*333/180));
  p_s10.arcTo(400-320,400-320,2*320,2*320,333,18);
  p_s10.lineTo(400+320*cos(M_PI*351/180),400-320*sin(M_PI*351/180));
  p_s10.arcTo(400-200,400-200,2*200,2*200,351,-18);

  p_s9.moveTo(400+40*cos(M_PI*135/180),400-40*sin(M_PI*135/180));
  p_s9.lineTo(400+180*cos(M_PI*135/180),400-180*sin(M_PI*135/180));
  p_s9.arcTo(400-180,400-180,2*180,2*180,135,18);
  p_s9.lineTo(400+40*cos(M_PI*153/180),400-40*sin(M_PI*153/180));
  p_s9.arcTo(400-40,400-40,2*40,2*40,153,-18);
  p_s9.moveTo(400+200*cos(M_PI*135/180),400-200*sin(M_PI*135/180));
  p_s9.lineTo(400+320*cos(M_PI*135/180),400-320*sin(M_PI*135/180));
  p_s9.arcTo(400-320,400-320,2*320,2*320,135,18);
  p_s9.lineTo(400+320*cos(M_PI*153/180),400-320*sin(M_PI*153/180));
  p_s9.arcTo(400-200,400-200,2*200,2*200,153,-18);

  p_s8.moveTo(400+40*cos(M_PI*189/180),400-40*sin(M_PI*189/180));
  p_s8.lineTo(400+180*cos(M_PI*189/180),400-180*sin(M_PI*189/180));
  p_s8.arcTo(400-180,400-180,2*180,2*180,189,18);
  p_s8.lineTo(400+40*cos(M_PI*207/180),400-40*sin(M_PI*207/180));
  p_s8.arcTo(400-40,400-40,2*40,2*40,207,-18);
  p_s8.moveTo(400+200*cos(M_PI*189/180),400-200*sin(M_PI*189/180));
  p_s8.lineTo(400+320*cos(M_PI*189/180),400-320*sin(M_PI*189/180));
  p_s8.arcTo(400-320,400-320,2*320,2*320,189,18);
  p_s8.lineTo(400+320*cos(M_PI*207/180),400-320*sin(M_PI*207/180));
  p_s8.arcTo(400-200,400-200,2*200,2*200,207,-18);

  p_s7.moveTo(400+40*cos(M_PI*225/180),400-40*sin(M_PI*225/180));
  p_s7.lineTo(400+180*cos(M_PI*225/180),400-180*sin(M_PI*225/180));
  p_s7.arcTo(400-180,400-180,2*180,2*180,225,18);
  p_s7.lineTo(400+40*cos(M_PI*243/180),400-40*sin(M_PI*243/180));
  p_s7.arcTo(400-40,400-40,2*40,2*40,243,-18);
  p_s7.moveTo(400+200*cos(M_PI*225/180),400-200*sin(M_PI*225/180));
  p_s7.lineTo(400+320*cos(M_PI*225/180),400-320*sin(M_PI*225/180));
  p_s7.arcTo(400-320,400-320,2*320,2*320,225,18);
  p_s7.lineTo(400+320*cos(M_PI*243/180),400-320*sin(M_PI*243/180));
  p_s7.arcTo(400-200,400-200,2*200,2*200,243,-18);

  p_s6.moveTo(400+40*cos(M_PI*351/180),400-40*sin(M_PI*351/180));
  p_s6.lineTo(400+180*cos(M_PI*351/180),400-180*sin(M_PI*351/180));
  p_s6.arcTo(400-180,400-180,2*180,2*180,351,18);
  p_s6.lineTo(400+40*cos(M_PI*9/180),400-40*sin(M_PI*9/180));
  p_s6.arcTo(400-40,400-40,2*40,2*40,9,-18);
  p_s6.moveTo(400+200*cos(M_PI*351/180),400-200*sin(M_PI*351/180));
  p_s6.lineTo(400+320*cos(M_PI*351/180),400-320*sin(M_PI*351/180));
  p_s6.arcTo(400-320,400-320,2*320,2*320,351,18);
  p_s6.lineTo(400+320*cos(M_PI*9/180),400-320*sin(M_PI*9/180));
  p_s6.arcTo(400-200,400-200,2*200,2*200,9,-18);

  p_s5.moveTo(400+40*cos(M_PI*99/180),400-40*sin(M_PI*99/180));
  p_s5.lineTo(400+180*cos(M_PI*99/180),400-180*sin(M_PI*99/180));
  p_s5.arcTo(400-180,400-180,2*180,2*180,99,18);
  p_s5.lineTo(400+40*cos(M_PI*117/180),400-40*sin(M_PI*117/180));
  p_s5.arcTo(400-40,400-40,2*40,2*40,117,-18);
  p_s5.moveTo(400+200*cos(M_PI*99/180),400-200*sin(M_PI*99/180));
  p_s5.lineTo(400+320*cos(M_PI*99/180),400-320*sin(M_PI*99/180));
  p_s5.arcTo(400-320,400-320,2*320,2*320,99,18);
  p_s5.lineTo(400+320*cos(M_PI*117/180),400-320*sin(M_PI*117/180));
  p_s5.arcTo(400-200,400-200,2*200,2*200,117,-18);

  p_s4.moveTo(400+40*cos(M_PI*27/180),400-40*sin(M_PI*27/180));
  p_s4.lineTo(400+180*cos(M_PI*27/180),400-180*sin(M_PI*27/180));
  p_s4.arcTo(400-180,400-180,2*180,2*180,27,18);
  p_s4.lineTo(400+40*cos(M_PI*45/180),400-40*sin(M_PI*45/180));
  p_s4.arcTo(400-40,400-40,2*40,2*40,45,-18);
  p_s4.moveTo(400+200*cos(M_PI*27/180),400-200*sin(M_PI*27/180));
  p_s4.lineTo(400+320*cos(M_PI*27/180),400-320*sin(M_PI*27/180));
  p_s4.arcTo(400-320,400-320,2*320,2*320,27,18);
  p_s4.lineTo(400+320*cos(M_PI*45/180),400-320*sin(M_PI*45/180));
  p_s4.arcTo(400-200,400-200,2*200,2*200,45,-18);

  p_s3.moveTo(400+40*cos(M_PI*261/180),400-40*sin(M_PI*261/180));
  p_s3.lineTo(400+180*cos(M_PI*261/180),400-180*sin(M_PI*261/180));
  p_s3.arcTo(400-180,400-180,2*180,2*180,261,18);
  p_s3.lineTo(400+40*cos(M_PI*279/180),400-40*sin(M_PI*279/180));
  p_s3.arcTo(400-40,400-40,2*40,2*40,279,-18);
  p_s3.moveTo(400+200*cos(M_PI*261/180),400-200*sin(M_PI*261/180));
  p_s3.lineTo(400+320*cos(M_PI*261/180),400-320*sin(M_PI*261/180));
  p_s3.arcTo(400-320,400-320,2*320,2*320,261,18);
  p_s3.lineTo(400+320*cos(M_PI*279/180),400-320*sin(M_PI*279/180));
  p_s3.arcTo(400-200,400-200,2*200,2*200,279,-18);


  p_s2.moveTo(400+40*cos(M_PI*297/180),400-40*sin(M_PI*297/180));
  p_s2.lineTo(400+180*cos(M_PI*297/180),400-180*sin(M_PI*297/180));
  p_s2.arcTo(400-180,400-180,2*180,2*180,297,18);
  p_s2.lineTo(400+40*cos(M_PI*315/180),400-40*sin(M_PI*315/180));
  p_s2.arcTo(400-40,400-40,2*40,2*40,315,-18);
  p_s2.moveTo(400+200*cos(M_PI*297/180),400-200*sin(M_PI*297/180));
  p_s2.lineTo(400+320*cos(M_PI*297/180),400-320*sin(M_PI*297/180));
  p_s2.arcTo(400-320,400-320,2*320,2*320,297,18);
  p_s2.lineTo(400+320*cos(M_PI*315/180),400-320*sin(M_PI*315/180));
  p_s2.arcTo(400-200,400-200,2*200,2*200,315,-18);

  p_s1.moveTo(400+40*cos(M_PI*63/180),400-40*sin(M_PI*63/180));
  p_s1.lineTo(400+180*cos(M_PI*63/180),400-180*sin(M_PI*63/180));
  p_s1.arcTo(400-180,400-180,2*180,2*180,63,18);
  p_s1.lineTo(400+40*cos(M_PI*81/180),400-40*sin(M_PI*81/180));
  p_s1.arcTo(400-40,400-40,2*40,2*40,81,-18);
  p_s1.moveTo(400+200*cos(M_PI*63/180),400-200*sin(M_PI*63/180));
  p_s1.lineTo(400+320*cos(M_PI*63/180),400-320*sin(M_PI*63/180));
  p_s1.arcTo(400-320,400-320,2*320,2*320,63,18);
  p_s1.lineTo(400+320*cos(M_PI*81/180),400-320*sin(M_PI*81/180));
  p_s1.arcTo(400-200,400-200,2*200,2*200,81,-18);

  p_sbull.addEllipse(QRect(400-40,400-40,80,80));
  p_sbull.addEllipse(QRect(400-20,400-20,40,40));

  p_d20.moveTo(400+320*cos(M_PI*81/180),400-320*sin(M_PI*81/180));
  p_d20.lineTo(400+340*cos(M_PI*81/180),400-340*sin(M_PI*81/180));
  p_d20.arcTo(400-340,400-340,2*340,2*340,81,18);
  p_d20.lineTo(400+320*cos(M_PI*99/180),400-320*sin(M_PI*99/180));
  p_d20.arcTo(400-320,400-320,2*320,2*320,99,-18);

  p_d19.moveTo(400+320*cos(M_PI*243/180),400-320*sin(M_PI*243/180));
  p_d19.lineTo(400+340*cos(M_PI*243/180),400-340*sin(M_PI*243/180));
  p_d19.arcTo(400-340,400-340,2*340,2*340,243,18);
  p_d19.lineTo(400+320*cos(M_PI*261/180),400-320*sin(M_PI*261/180));
  p_d19.arcTo(400-320,400-320,2*320,2*320,261,-18);

  p_d18.moveTo(400+320*cos(M_PI*45/180),400-320*sin(M_PI*45/180));
  p_d18.lineTo(400+340*cos(M_PI*45/180),400-340*sin(M_PI*45/180));
  p_d18.arcTo(400-340,400-340,2*340,2*340,45,18);
  p_d18.lineTo(400+320*cos(M_PI*63/180),400-320*sin(M_PI*63/180));
  p_d18.arcTo(400-320,400-320,2*320,2*320,63,-18);

  p_d17.moveTo(400+320*cos(M_PI*279/180),400-320*sin(M_PI*279/180));
  p_d17.lineTo(400+340*cos(M_PI*279/180),400-340*sin(M_PI*279/180));
  p_d17.arcTo(400-340,400-340,2*340,2*340,279,18);
  p_d17.lineTo(400+320*cos(M_PI*297/180),400-320*sin(M_PI*297/180));
  p_d17.arcTo(400-320,400-320,2*320,2*320,297,-18);

  p_d16.moveTo(400+320*cos(M_PI*207/180),400-320*sin(M_PI*207/180));
  p_d16.lineTo(400+340*cos(M_PI*207/180),400-340*sin(M_PI*207/180));
  p_d16.arcTo(400-340,400-340,2*340,2*340,207,18);
  p_d16.lineTo(400+320*cos(M_PI*225/180),400-320*sin(M_PI*225/180));
  p_d16.arcTo(400-320,400-320,2*320,2*320,225,-18);

  p_d15.moveTo(400+320*cos(M_PI*315/180),400-320*sin(M_PI*315/180));
  p_d15.lineTo(400+340*cos(M_PI*315/180),400-340*sin(M_PI*315/180));
  p_d15.arcTo(400-340,400-340,2*340,2*340,315,18);
  p_d15.lineTo(400+320*cos(M_PI*333/180),400-320*sin(M_PI*333/180));
  p_d15.arcTo(400-320,400-320,2*320,2*320,333,-18);

  p_d14.moveTo(400+320*cos(M_PI*153/180),400-320*sin(M_PI*153/180));
  p_d14.lineTo(400+340*cos(M_PI*153/180),400-340*sin(M_PI*153/180));
  p_d14.arcTo(400-340,400-340,2*340,2*340,153,18);
  p_d14.lineTo(400+320*cos(M_PI*171/180),400-320*sin(M_PI*171/180));
  p_d14.arcTo(400-320,400-320,2*320,2*320,171,-18);

  p_d13.moveTo(400+320*cos(M_PI*9/180),400-320*sin(M_PI*9/180));
  p_d13.lineTo(400+340*cos(M_PI*9/180),400-340*sin(M_PI*9/180));
  p_d13.arcTo(400-340,400-340,2*340,2*340,9,18);
  p_d13.lineTo(400+320*cos(M_PI*27/180),400-320*sin(M_PI*27/180));
  p_d13.arcTo(400-320,400-320,2*320,2*320,27,-18);

  p_d12.moveTo(400+320*cos(M_PI*117/180),400-320*sin(M_PI*117/180));
  p_d12.lineTo(400+340*cos(M_PI*117/180),400-340*sin(M_PI*117/180));
  p_d12.arcTo(400-340,400-340,2*340,2*340,117,18);
  p_d12.lineTo(400+320*cos(M_PI*135/180),400-320*sin(M_PI*135/180));
  p_d12.arcTo(400-320,400-320,2*320,2*320,135,-18);

  p_d11.moveTo(400+320*cos(M_PI*171/180),400-320*sin(M_PI*171/180));
  p_d11.lineTo(400+340*cos(M_PI*171/180),400-340*sin(M_PI*171/180));
  p_d11.arcTo(400-340,400-340,2*340,2*340,171,18);
  p_d11.lineTo(400+320*cos(M_PI*189/180),400-320*sin(M_PI*189/180));
  p_d11.arcTo(400-320,400-320,2*320,2*320,189,-18);

  p_d10.moveTo(400+320*cos(M_PI*333/180),400-320*sin(M_PI*333/180));
  p_d10.lineTo(400+340*cos(M_PI*333/180),400-340*sin(M_PI*333/180));
  p_d10.arcTo(400-340,400-340,2*340,2*340,333,18);
  p_d10.lineTo(400+320*cos(M_PI*351/180),400-320*sin(M_PI*351/180));
  p_d10.arcTo(400-320,400-320,2*320,2*320,351,-18);

  p_d9.moveTo(400+320*cos(M_PI*135/180),400-320*sin(M_PI*135/180));
  p_d9.lineTo(400+340*cos(M_PI*135/180),400-340*sin(M_PI*135/180));
  p_d9.arcTo(400-340,400-340,2*340,2*340,135,18);
  p_d9.lineTo(400+320*cos(M_PI*153/180),400-320*sin(M_PI*153/180));
  p_d9.arcTo(400-320,400-320,2*320,2*320,153,-18);

  p_d8.moveTo(400+320*cos(M_PI*189/180),400-320*sin(M_PI*189/180));
  p_d8.lineTo(400+340*cos(M_PI*189/180),400-340*sin(M_PI*189/180));
  p_d8.arcTo(400-340,400-340,2*340,2*340,189,18);
  p_d8.lineTo(400+320*cos(M_PI*207/180),400-320*sin(M_PI*207/180));
  p_d8.arcTo(400-320,400-320,2*320,2*320,207,-18);

  p_d7.moveTo(400+320*cos(M_PI*225/180),400-320*sin(M_PI*225/180));
  p_d7.lineTo(400+340*cos(M_PI*225/180),400-340*sin(M_PI*225/180));
  p_d7.arcTo(400-340,400-340,2*340,2*340,225,18);
  p_d7.lineTo(400+320*cos(M_PI*243/180),400-320*sin(M_PI*243/180));
  p_d7.arcTo(400-320,400-320,2*320,2*320,243,-18);

  p_d6.moveTo(400+320*cos(M_PI*351/180),400-320*sin(M_PI*351/180));
  p_d6.lineTo(400+340*cos(M_PI*351/180),400-340*sin(M_PI*351/180));
  p_d6.arcTo(400-340,400-340,2*340,2*340,351,18);
  p_d6.lineTo(400+320*cos(M_PI*9/180),400-320*sin(M_PI*9/180));
  p_d6.arcTo(400-320,400-320,2*320,2*320,9,-18);

  p_d5.moveTo(400+320*cos(M_PI*99/180),400-320*sin(M_PI*99/180));
  p_d5.lineTo(400+340*cos(M_PI*99/180),400-340*sin(M_PI*99/180));
  p_d5.arcTo(400-340,400-340,2*340,2*340,99,18);
  p_d5.lineTo(400+320*cos(M_PI*117/180),400-320*sin(M_PI*117/180));
  p_d5.arcTo(400-320,400-320,2*320,2*320,117,-18);

  p_d4.moveTo(400+320*cos(M_PI*27/180),400-320*sin(M_PI*27/180));
  p_d4.lineTo(400+340*cos(M_PI*27/180),400-340*sin(M_PI*27/180));
  p_d4.arcTo(400-340,400-340,2*340,2*340,27,18);
  p_d4.lineTo(400+320*cos(M_PI*45/180),400-320*sin(M_PI*45/180));
  p_d4.arcTo(400-320,400-320,2*320,2*320,45,-18);

  p_d3.moveTo(400+320*cos(M_PI*261/180),400-320*sin(M_PI*261/180));
  p_d3.lineTo(400+340*cos(M_PI*261/180),400-340*sin(M_PI*261/180));
  p_d3.arcTo(400-340,400-340,2*340,2*340,261,18);
  p_d3.lineTo(400+320*cos(M_PI*279/180),400-320*sin(M_PI*279/180));
  p_d3.arcTo(400-320,400-320,2*320,2*320,279,-18);

  p_d2.moveTo(400+320*cos(M_PI*297/180),400-320*sin(M_PI*297/180));
  p_d2.lineTo(400+340*cos(M_PI*297/180),400-340*sin(M_PI*297/180));
  p_d2.arcTo(400-340,400-340,2*340,2*340,297,18);
  p_d2.lineTo(400+320*cos(M_PI*315/180),400-320*sin(M_PI*315/180));
  p_d2.arcTo(400-320,400-320,2*320,2*320,315,-18);

  p_d1.moveTo(400+320*cos(M_PI*63/180),400-320*sin(M_PI*63/180));
  p_d1.lineTo(400+340*cos(M_PI*63/180),400-340*sin(M_PI*63/180));
  p_d1.arcTo(400-340,400-340,2*340,2*340,63,18);
  p_d1.lineTo(400+320*cos(M_PI*81/180),400-320*sin(M_PI*81/180));
  p_d1.arcTo(400-320,400-320,2*320,2*320,81,-18);

  p_dbull.moveTo(400+20,400);
  p_dbull.arcTo(400-20,400-20,2*20,2*20,0,360);

  p_t20.moveTo(400+180*cos(M_PI*81/180),400-180*sin(M_PI*81/180));
  p_t20.lineTo(400+200*cos(M_PI*81/180),400-200*sin(M_PI*81/180));
  p_t20.arcTo(400-200,400-200,2*200,2*200,81,18);
  p_t20.lineTo(400+180*cos(M_PI*99/180),400-180*sin(M_PI*99/180));
  p_t20.arcTo(400-180,400-180,2*180,2*180,99,-18);

  p_t19.moveTo(400+180*cos(M_PI*243/180),400-180*sin(M_PI*243/180));
  p_t19.lineTo(400+200*cos(M_PI*243/180),400-200*sin(M_PI*243/180));
  p_t19.arcTo(400-200,400-200,2*200,2*200,243,18);
  p_t19.lineTo(400+180*cos(M_PI*261/180),400-180*sin(M_PI*261/180));
  p_t19.arcTo(400-180,400-180,2*180,2*180,261,-18);

  p_t18.moveTo(400+180*cos(M_PI*45/180),400-180*sin(M_PI*45/180));
  p_t18.lineTo(400+200*cos(M_PI*45/180),400-200*sin(M_PI*45/180));
  p_t18.arcTo(400-200,400-200,2*200,2*200,45,18);
  p_t18.lineTo(400+180*cos(M_PI*63/180),400-180*sin(M_PI*63/180));
  p_t18.arcTo(400-180,400-180,2*180,2*180,63,-18);

  p_t17.moveTo(400+180*cos(M_PI*279/180),400-180*sin(M_PI*279/180));
  p_t17.lineTo(400+200*cos(M_PI*279/180),400-200*sin(M_PI*279/180));
  p_t17.arcTo(400-200,400-200,2*200,2*200,279,18);
  p_t17.lineTo(400+180*cos(M_PI*297/180),400-180*sin(M_PI*297/180));
  p_t17.arcTo(400-180,400-180,2*180,2*180,297,-18);

  p_t16.moveTo(400+180*cos(M_PI*207/180),400-180*sin(M_PI*207/180));
  p_t16.lineTo(400+200*cos(M_PI*207/180),400-200*sin(M_PI*207/180));
  p_t16.arcTo(400-200,400-200,2*200,2*200,207,18);
  p_t16.lineTo(400+180*cos(M_PI*225/180),400-180*sin(M_PI*225/180));
  p_t16.arcTo(400-180,400-180,2*180,2*180,225,-18);

  p_t15.moveTo(400+180*cos(M_PI*315/180),400-180*sin(M_PI*315/180));
  p_t15.lineTo(400+200*cos(M_PI*315/180),400-200*sin(M_PI*315/180));
  p_t15.arcTo(400-200,400-200,2*200,2*200,315,18);
  p_t15.lineTo(400+180*cos(M_PI*333/180),400-180*sin(M_PI*333/180));
  p_t15.arcTo(400-180,400-180,2*180,2*180,333,-18);

  p_t14.moveTo(400+180*cos(M_PI*153/180),400-180*sin(M_PI*153/180));
  p_t14.lineTo(400+200*cos(M_PI*153/180),400-200*sin(M_PI*153/180));
  p_t14.arcTo(400-200,400-200,2*200,2*200,153,18);
  p_t14.lineTo(400+180*cos(M_PI*171/180),400-180*sin(M_PI*171/180));
  p_t14.arcTo(400-180,400-180,2*180,2*180,171,-18);

  p_t13.moveTo(400+180*cos(M_PI*9/180),400-180*sin(M_PI*9/180));
  p_t13.lineTo(400+200*cos(M_PI*9/180),400-200*sin(M_PI*9/180));
  p_t13.arcTo(400-200,400-200,2*200,2*200,9,18);
  p_t13.lineTo(400+180*cos(M_PI*27/180),400-180*sin(M_PI*27/180));
  p_t13.arcTo(400-180,400-180,2*180,2*180,27,-18);

  p_t12.moveTo(400+180*cos(M_PI*117/180),400-180*sin(M_PI*117/180));
  p_t12.lineTo(400+200*cos(M_PI*117/180),400-200*sin(M_PI*117/180));
  p_t12.arcTo(400-200,400-200,2*200,2*200,117,18);
  p_t12.lineTo(400+180*cos(M_PI*135/180),400-180*sin(M_PI*135/180));
  p_t12.arcTo(400-180,400-180,2*180,2*180,135,-18);

  p_t11.moveTo(400+180*cos(M_PI*171/180),400-180*sin(M_PI*171/180));
  p_t11.lineTo(400+200*cos(M_PI*171/180),400-200*sin(M_PI*171/180));
  p_t11.arcTo(400-200,400-200,2*200,2*200,171,18);
  p_t11.lineTo(400+180*cos(M_PI*189/180),400-180*sin(M_PI*189/180));
  p_t11.arcTo(400-180,400-180,2*180,2*180,189,-18);

  p_t10.moveTo(400+180*cos(M_PI*333/180),400-180*sin(M_PI*333/180));
  p_t10.lineTo(400+200*cos(M_PI*333/180),400-200*sin(M_PI*333/180));
  p_t10.arcTo(400-200,400-200,2*200,2*200,333,18);
  p_t10.lineTo(400+180*cos(M_PI*351/180),400-180*sin(M_PI*351/180));
  p_t10.arcTo(400-180,400-180,2*180,2*180,351,-18);

  p_t9.moveTo(400+180*cos(M_PI*135/180),400-180*sin(M_PI*135/180));
  p_t9.lineTo(400+200*cos(M_PI*135/180),400-200*sin(M_PI*135/180));
  p_t9.arcTo(400-200,400-200,2*200,2*200,135,18);
  p_t9.lineTo(400+180*cos(M_PI*153/180),400-180*sin(M_PI*153/180));
  p_t9.arcTo(400-180,400-180,2*180,2*180,153,-18);

  p_t8.moveTo(400+180*cos(M_PI*189/180),400-180*sin(M_PI*189/180));
  p_t8.lineTo(400+200*cos(M_PI*189/180),400-200*sin(M_PI*189/180));
  p_t8.arcTo(400-200,400-200,2*200,2*200,189,18);
  p_t8.lineTo(400+180*cos(M_PI*207/180),400-180*sin(M_PI*207/180));
  p_t8.arcTo(400-180,400-180,2*180,2*180,207,-18);

  p_t7.moveTo(400+180*cos(M_PI*225/180),400-180*sin(M_PI*225/180));
  p_t7.lineTo(400+200*cos(M_PI*225/180),400-200*sin(M_PI*225/180));
  p_t7.arcTo(400-200,400-200,2*200,2*200,225,18);
  p_t7.lineTo(400+180*cos(M_PI*243/180),400-180*sin(M_PI*243/180));
  p_t7.arcTo(400-180,400-180,2*180,2*180,243,-18);

  p_t6.moveTo(400+180*cos(M_PI*351/180),400-180*sin(M_PI*351/180));
  p_t6.lineTo(400+200*cos(M_PI*351/180),400-200*sin(M_PI*351/180));
  p_t6.arcTo(400-200,400-200,2*200,2*200,351,18);
  p_t6.lineTo(400+180*cos(M_PI*9/180),400-180*sin(M_PI*9/180));
  p_t6.arcTo(400-180,400-180,2*180,2*180,9,-18);

  p_t5.moveTo(400+180*cos(M_PI*99/180),400-180*sin(M_PI*99/180));
  p_t5.lineTo(400+200*cos(M_PI*99/180),400-200*sin(M_PI*99/180));
  p_t5.arcTo(400-200,400-200,2*200,2*200,99,18);
  p_t5.lineTo(400+180*cos(M_PI*117/180),400-180*sin(M_PI*117/180));
  p_t5.arcTo(400-180,400-180,2*180,2*180,117,-18);

  p_t4.moveTo(400+180*cos(M_PI*27/180),400-180*sin(M_PI*27/180));
  p_t4.lineTo(400+200*cos(M_PI*27/180),400-200*sin(M_PI*27/180));
  p_t4.arcTo(400-200,400-200,2*200,2*200,27,18);
  p_t4.lineTo(400+180*cos(M_PI*45/180),400-180*sin(M_PI*45/180));
  p_t4.arcTo(400-180,400-180,2*180,2*180,45,-18);

  p_t3.moveTo(400+180*cos(M_PI*261/180),400-180*sin(M_PI*261/180));
  p_t3.lineTo(400+200*cos(M_PI*261/180),400-200*sin(M_PI*261/180));
  p_t3.arcTo(400-200,400-200,2*200,2*200,261,18);
  p_t3.lineTo(400+180*cos(M_PI*279/180),400-180*sin(M_PI*279/180));
  p_t3.arcTo(400-180,400-180,2*180,2*180,279,-18);

  p_t2.moveTo(400+180*cos(M_PI*297/180),400-180*sin(M_PI*297/180));
  p_t2.lineTo(400+200*cos(M_PI*297/180),400-200*sin(M_PI*297/180));
  p_t2.arcTo(400-200,400-200,2*200,2*200,297,18);
  p_t2.lineTo(400+180*cos(M_PI*315/180),400-180*sin(M_PI*315/180));
  p_t2.arcTo(400-180,400-180,2*180,2*180,315,-18);

  p_t1.moveTo(400+180*cos(M_PI*63/180),400-180*sin(M_PI*63/180));
  p_t1.lineTo(400+200*cos(M_PI*63/180),400-200*sin(M_PI*63/180));
  p_t1.arcTo(400-200,400-200,2*200,2*200,63,18);
  p_t1.lineTo(400+180*cos(M_PI*81/180),400-180*sin(M_PI*81/180));
  p_t1.arcTo(400-180,400-180,2*180,2*180,81,-18);

  p_noscore.addEllipse(QRect(0, 0, 800, 800));
  p_noscore.addEllipse(QRect(58, 58, 684, 684));

  CDartBoardSegment * s20 = new CDartBoardSegment(p_s20, 20);
  CDartBoardSegment * s19 = new CDartBoardSegment(p_s19, 19, "beige");
  CDartBoardSegment * s18 = new CDartBoardSegment(p_s18, 18);
  CDartBoardSegment * s17 = new CDartBoardSegment(p_s17, 17, "beige");
  CDartBoardSegment * s14 = new CDartBoardSegment(p_s14, 14);
  CDartBoardSegment * s13 = new CDartBoardSegment(p_s13, 13);
  CDartBoardSegment * s12 = new CDartBoardSegment(p_s12, 12);
  CDartBoardSegment * s11 = new CDartBoardSegment(p_s11, 11, "beige");
  CDartBoardSegment * s16 = new CDartBoardSegment(p_s16, 16, "beige");
  CDartBoardSegment * s15 = new CDartBoardSegment(p_s15, 15, "beige");
  CDartBoardSegment * s10 = new CDartBoardSegment(p_s10, 10);
  CDartBoardSegment * s9 = new CDartBoardSegment(p_s9, 9, "beige");
  CDartBoardSegment * s8 = new CDartBoardSegment(p_s8, 8);
  CDartBoardSegment * s7 = new CDartBoardSegment(p_s7, 7);
  CDartBoardSegment * s6 = new CDartBoardSegment(p_s6, 6, "beige");
  CDartBoardSegment * s5 = new CDartBoardSegment(p_s5, 5, "beige");
  CDartBoardSegment * s4 = new CDartBoardSegment(p_s4, 4, "beige");
  CDartBoardSegment * s3 = new CDartBoardSegment(p_s3, 3);
  CDartBoardSegment * s2 = new CDartBoardSegment(p_s2, 2);
  CDartBoardSegment * s1 = new CDartBoardSegment(p_s1, 1, "beige");
  CDartBoardSegment * sbull = new CDartBoardSegment(p_sbull, 25, "green");
  CDartBoardSegment * d20 = new CDartBoardSegment(p_d20, 40, "red", 'd');
  CDartBoardSegment * d19 = new CDartBoardSegment(p_d19, 38, "green", 'd');
  CDartBoardSegment * d18 = new CDartBoardSegment(p_d18, 36, "red", 'd');
  CDartBoardSegment * d17 = new CDartBoardSegment(p_d17, 34, "green", 'd');
  CDartBoardSegment * d16 = new CDartBoardSegment(p_d16, 32, "green", 'd');
  CDartBoardSegment * d15 = new CDartBoardSegment(p_d15, 30, "green", 'd');
  CDartBoardSegment * d14 = new CDartBoardSegment(p_d14, 28, "red", 'd');
  CDartBoardSegment * d13 = new CDartBoardSegment(p_d13, 26, "red", 'd');
  CDartBoardSegment * d12 = new CDartBoardSegment(p_d12, 24, "red", 'd');
  CDartBoardSegment * d11 = new CDartBoardSegment(p_d11, 22, "green", 'd');
  CDartBoardSegment * d10 = new CDartBoardSegment(p_d10, 20, "red", 'd');
  CDartBoardSegment * d9 = new CDartBoardSegment(p_d9, 18, "green", 'd');
  CDartBoardSegment * d8 = new CDartBoardSegment(p_d8, 16, "red", 'd');
  CDartBoardSegment * d7 = new CDartBoardSegment(p_d7, 14, "red", 'd');
  CDartBoardSegment * d6 = new CDartBoardSegment(p_d6, 12, "green", 'd');
  CDartBoardSegment * d5 = new CDartBoardSegment(p_d5, 10, "green", 'd');
  CDartBoardSegment * d4 = new CDartBoardSegment(p_d4, 8, "green", 'd');
  CDartBoardSegment * d3 = new CDartBoardSegment(p_d3, 6, "red", 'd');
  CDartBoardSegment * d2 = new CDartBoardSegment(p_d2, 4, "red", 'd');
  CDartBoardSegment * d1 = new CDartBoardSegment(p_d1, 2, "green", 'd');
  CDartBoardSegment * dbull = new CDartBoardSegment(p_dbull, 50, "red", 'd');
  CDartBoardSegment * t20 = new CDartBoardSegment(p_t20, 60, "red", 't');
  CDartBoardSegment * t19 = new CDartBoardSegment(p_t19, 57, "green", 't');
  CDartBoardSegment * t18 = new CDartBoardSegment(p_t18, 54, "red", 't');
  CDartBoardSegment * t17 = new CDartBoardSegment(p_t17, 51, "green", 't');
  CDartBoardSegment * t16 = new CDartBoardSegment(p_t16, 48, "green", 't');
  CDartBoardSegment * t15 = new CDartBoardSegment(p_t15, 45, "green", 't');
  CDartBoardSegment * t14 = new CDartBoardSegment(p_t14, 42, "red", 't');
  CDartBoardSegment * t13 = new CDartBoardSegment(p_t13, 39, "red", 't');
  CDartBoardSegment * t12 = new CDartBoardSegment(p_t12, 36, "red", 't');
  CDartBoardSegment * t11 = new CDartBoardSegment(p_t11, 33, "green", 't');
  CDartBoardSegment * t10 = new CDartBoardSegment(p_t10, 30, "red", 't');
  CDartBoardSegment * t9 = new CDartBoardSegment(p_t9, 27, "green", 't');
  CDartBoardSegment * t8 = new CDartBoardSegment(p_t8, 24, "red", 't');
  CDartBoardSegment * t7 = new CDartBoardSegment(p_t7, 21, "red", 't');
  CDartBoardSegment * t6 = new CDartBoardSegment(p_t6, 18, "green", 't');
  CDartBoardSegment * t5 = new CDartBoardSegment(p_t5, 15, "green", 't');
  CDartBoardSegment * t4 = new CDartBoardSegment(p_t4, 12, "green", 't');
  CDartBoardSegment * t3 = new CDartBoardSegment(p_t3, 9, "red", 't');
  CDartBoardSegment * t2 = new CDartBoardSegment(p_t2, 6, "red", 't');
  CDartBoardSegment * t1 = new CDartBoardSegment(p_t1, 3, "green", 't');
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
  erase_all_darts();
  display_score(mScore);

  connect(s20, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s19, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s18, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s17, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s16, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s15, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s14, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s13, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s12, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s11, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s10, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s9, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s8, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s7, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s6, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s5, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s4, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s3, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s2, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(s1, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(sbull, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d20, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d19, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d18, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d17, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d16, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d15, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d14, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d13, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d12, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d11, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d10, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d9, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d8, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d7, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d6, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d5, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d4, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d3, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d2, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(d1, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(dbull, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t20, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t19, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t18, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t17, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t16, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t15, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t14, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t13, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t12, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t11, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t10, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t9, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t8, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t7, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t6, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t5, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t4, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t3, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t2, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(t1, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  connect(noscore, SIGNAL (signalSegmentPressed(int&, QChar&)), this, SLOT (signalSegmentPressed(int&, QChar&)));
  mBustedSound.setSource(QUrl("qrc:/resources/sounds/busted.wav"));
  mGameShotSound.setSource(QUrl("qrc:/resources/sounds/gameshot.wav"));
}

void CDartBoard::set_score(uint32_t iVal, QChar iType, uint32_t iCheckout)
{
  mScore -= iVal;
  display_score(mScore);
  if (mCounter == 3)
  {
    display_dart1(iVal);
    emit signal_update_finishes(mScore, 2);
    mUndo[0] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType + QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[0] = iCheckout;
  }
  else if (mCounter == 2)
  {
    display_dart2(iVal);
    emit signal_update_finishes(mScore, 1);
    mUndo[1] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType+QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[1] = iCheckout;
  }
  else if (mCounter == 1)
  {
    display_dart3(iVal);
    mUndo[2] = iVal;
    if (!mBusted)
    {
      mDarts.append(iType+QString::number(iVal));
    }
    else if (mBusted)
    {
      mDarts.append("S0");
    }
    mCheckoutAttempts[2] = iCheckout;
  }
}

void CDartBoard::init_dartboard(uint32_t iScore)
{
  mStop = false;
  mBusted = false;
  mScore = iScore;
  mOldScore = mScore;
  mCheckoutAttempts = {0, 0, 0};
  erase_all_darts();
  display_score(mScore);
  mCounter = 3;
  mDarts = {};
}

void CDartBoard::signalSegmentPressed(uint32_t &value, QChar &type)
{
  if (mFinished)
  {
    QMessageBox::warning(this, "","Game already finished!");
    return;
  }
  QVector<uint32_t> array {23, 29, 31, 35, 37, 41, 43, 44, 46, 47, 49, 52, 53, 55, 56, 58, 59};
  uint32_t checkout = 0;
  if (!mStop)
  {
    if (mCounter > 0)
    {
      if (mScore == mStartVal)
      {
        if (mSingleIn)
        {
          set_score(value, type, checkout);
        }
        else if (mDoubleIn)
        {
          if (type == 'd')
          {
            set_score(value, type, checkout);
          } else
          {
            set_score(0, type, checkout);
          }
        }
        else if (mMasterIn)
        {
          if (type == 't')
          {
            set_score(value, type, checkout);
          }
          else
          {
            set_score(0, type, checkout);
          }
        }
      }
      else if (mScore > value && mScore < mStartVal && mSingleOut)
      {
        if (mScore <= 60 && !(std::find(array.begin(), array.end(), mScore)))
        {
          checkout = 1;
        }
        set_score(value, type, checkout);
      }
      else if (mScore - value > 1 && mScore < mStartVal && mDoubleOut)
      {
        if ((mScore <= 40 && mScore % 2 == 0 && mScore > 1) || mScore == 50)
        {
          checkout = 1;
        }
        set_score(value, type, checkout);
      }
      else if (mScore - value > 2 && mScore < mStartVal && mMasterOut)
      {
        if (mScore <= 60 && mScore % 3 == 0 && mScore > 2)
        {
          checkout = 1;
        }
        set_score(value, type, checkout);
      }
      else if (mScore == value)
      {
        if (mSingleOut)
        {
          mStop = true;  // Game shot
          emit signal_set_focus_to_submit_button();
          mGameShotSound.play();
          checkout = 1;
          set_score(value, type, checkout);
        } else if (mDoubleOut)
        {
          if (type == 'd')
          {
            mStop = true;  // Game shot
            emit signal_set_focus_to_submit_button();
            mGameShotSound.play();
            checkout = 1;
            set_score(value, type, checkout);
          }
          else
          {
            if ((mScore > 1 && mScore % 2 == 0 && mScore <= 40) || mScore == 50)
            {
              checkout = 1;
            }
            set_score(value, type, checkout);
            display_score(mOldScore);
            mStop = true;  // Überwofen
            mBusted = true;
            emit signal_set_focus_to_submit_button();
            mBustedSound.play();
          }
        }
        else if (mMasterOut)
        {
          if (type == 't')
          {
            checkout = 1;
            mStop = true;  // Game shot
            mGameShotSound.play();
            set_score(value, type, checkout);
          }
          else
          {
            if (mScore <= 60 && mScore % 3 == 0 && mScore > 2)
            {
              checkout = 1;
            }
            set_score(value, type, checkout);
            display_score(mOldScore);
            mStop = true;  // Überwofen
            mBusted = true;
            emit signal_set_focus_to_submit_button();
            mBustedSound.play();
          }
        }
      }
      else
      {
        if (mSingleOut && (mScore <= 60 && !(std::find(array.begin(), array.end(), mScore))))
        {
          checkout = 1;
        }
        else if (mDoubleOut && ((mScore <= 40 && mScore % 2 == 0 && mScore > 1) || mScore == 50))
        {
          checkout = 1;
        }
        else if (mMasterOut && (mScore <= 60 && mScore % 3 == 0 && mScore > 2))
        {
          checkout = 1;
        }
        mStop = true;
        mBusted = true;  // Überworfen
        emit signal_set_focus_to_submit_button();
        set_score(value, type, checkout);
        display_score(mOldScore);
        mBustedSound.play();
      }

      mCounter--;
      if (mCounter == 0)
      {
        mStop = true;
        emit signal_set_focus_to_submit_button();
      }
    }
  }
  else if (mBusted)
  {
    QMessageBox::warning(this, "Warning", "You are already busted!");
  }
  else if (mScore == 0)
  {
    QMessageBox::warning(this, "Warning", "You have already won this leg!");
  }
  else if (mScore > 0 && mCounter == 0)
  {
    QMessageBox::warning(this, "Warning", "You only have three darts!");
  }
}

void CDartBoard::perform_undo()
{
  if (mCounter < 3)
  {
    mScore += mUndo[2 - mCounter];
    mUndo[2 - mCounter] = 0;
    mDarts.pop_back();
    mCheckoutAttempts[2 - mCounter] = 0;
    display_score(mScore);

    if (mCounter == 2)
    {
      erase_dart1();
      emit signal_update_finishes(mScore, 3);
    }
    else if (mCounter == 1)
    {
      erase_dart2();
      emit signal_update_finishes(mScore, 2);
    }
    else if (mCounter == 0)
    {
      erase_dart3();
      emit signal_update_finishes(mScore, 1);
    }

    mCounter++;
    mStop = false;
    mBusted = false;
  }
}

void CDartBoard::submit_score()
{
  if (mStop)
  {
    QVector<QString> darts = mDarts;
    uint32_t score;
    if (mBusted)
    {
      darts = {"S0", "S0", "S0"};
      score = 0;
    }
    else
    {
      score = mOldScore - mScore;
    }
    uint32_t numberofdarts = 3 - mCounter;
    uint32_t checkoutattempts = std::accumulate(mCheckoutAttempts.begin(), mCheckoutAttempts.end(),0);
    emit signal_submit_button_pressed_to_game_window(score, numberofdarts, checkoutattempts, darts);
  }
  else if (!mFinished)
  {
    QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
  }
  else
  {
    QMessageBox::warning(this, "","Game already finished!");
  }
}

void CDartBoard::display_score(uint32_t iScore)
{
  emit signal_display_score(iScore);
}

void CDartBoard::display_dart1(uint32_t iVal)
{
  emit signal_display_dart1(iVal);
}

void CDartBoard::display_dart2(uint32_t iVal)
{
  emit signal_display_dart2(iVal);
}

void CDartBoard::display_dart3(uint32_t iVal)
{
  emit signal_display_dart3(iVal);
}

void CDartBoard::erase_all_darts()
{
  erase_dart1();
  erase_dart2();
  erase_dart3();
}

void CDartBoard::erase_dart1()
{
  emit signal_erase_dart1();
}

void CDartBoard::erase_dart2()
{
  emit signal_erase_dart2();
}

void CDartBoard::erase_dart3()
{
  emit signal_erase_dart3();
}

void CDartBoard::set_finished()
{
  mFinished = true;
}

void CDartBoard::unset_finished()
{
  mFinished = false;
}



