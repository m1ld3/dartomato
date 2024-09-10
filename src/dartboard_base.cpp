#include "dartboard_base.h"
#include "dartboard_segment.h"
#include "cmath"

#ifndef TESTING
CDartBoard::CDartBoard(CDartBoardView * iGraphicsViewDartBoard, const CSettings iSettings)
  : mSettings(iSettings)
  , mSoundHandler(CSoundHandler::instance())
{
  mScene = new QGraphicsScene(0, 0, 800, 800, iGraphicsViewDartBoard);
  mScene->setSceneRect(0, 0, 800, 800);
  iGraphicsViewDartBoard->setRenderHint(QPainter::SmoothPixmapTransform, true);
  iGraphicsViewDartBoard->setRenderHint(QPainter::Antialiasing, true);
  iGraphicsViewDartBoard->setScene(mScene);
  iGraphicsViewDartBoard->show();

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

  p_sbull.addEllipse(QRect(400-40, 400-40, 80, 80));
  p_sbull.addEllipse(QRect(400-20, 400-20, 40, 40));

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

  p_noscore.addEllipse(QRect(0, 0, 800, 800));
  p_noscore.addEllipse(QRect(58, 58, 684, 684));
  mS20 = new CDartBoardSegment(this, p_s20, 20);
  mS19 = new CDartBoardSegment(this, p_s19, 19, "beige");
  mS18 = new CDartBoardSegment(this, p_s18, 18);
  mS17 = new CDartBoardSegment(this, p_s17, 17, "beige");
  mS14 = new CDartBoardSegment(this, p_s14, 14);
  mS13 = new CDartBoardSegment(this, p_s13, 13);
  mS12 = new CDartBoardSegment(this, p_s12, 12);
  mS11 = new CDartBoardSegment(this, p_s11, 11, "beige");
  mS16 = new CDartBoardSegment(this, p_s16, 16, "beige");
  mS15 = new CDartBoardSegment(this, p_s15, 15, "beige");
  mS10 = new CDartBoardSegment(this, p_s10, 10);
  mS9 = new CDartBoardSegment(this, p_s9, 9, "beige");
  mS8 = new CDartBoardSegment(this, p_s8, 8);
  mS7 = new CDartBoardSegment(this, p_s7, 7);
  mS6 = new CDartBoardSegment(this, p_s6, 6, "beige");
  mS5 = new CDartBoardSegment(this, p_s5, 5, "beige");
  mS4 = new CDartBoardSegment(this, p_s4, 4, "beige");
  mS3 = new CDartBoardSegment(this, p_s3, 3);
  mS2 = new CDartBoardSegment(this, p_s2, 2);
  mS1 = new CDartBoardSegment(this, p_s1, 1, "beige");
  mSBull = new CDartBoardSegment(this, p_sbull, 25, "green");
  mD20 = new CDartBoardSegment(this, p_d20, 40, "red", 'd');
  mD19 = new CDartBoardSegment(this, p_d19, 38, "green", 'd');
  mD18 = new CDartBoardSegment(this, p_d18, 36, "red", 'd');
  mD17 = new CDartBoardSegment(this, p_d17, 34, "green", 'd');
  mD16 = new CDartBoardSegment(this, p_d16, 32, "green", 'd');
  mD15 = new CDartBoardSegment(this, p_d15, 30, "green", 'd');
  mD14 = new CDartBoardSegment(this, p_d14, 28, "red", 'd');
  mD13 = new CDartBoardSegment(this, p_d13, 26, "red", 'd');
  mD12 = new CDartBoardSegment(this, p_d12, 24, "red", 'd');
  mD11 = new CDartBoardSegment(this, p_d11, 22, "green", 'd');
  mD10 = new CDartBoardSegment(this, p_d10, 20, "red", 'd');
  mD9 = new CDartBoardSegment(this, p_d9, 18, "green", 'd');
  mD8 = new CDartBoardSegment(this, p_d8, 16, "red", 'd');
  mD7 = new CDartBoardSegment(this, p_d7, 14, "red", 'd');
  mD6 = new CDartBoardSegment(this, p_d6, 12, "green", 'd');
  mD5 = new CDartBoardSegment(this, p_d5, 10, "green", 'd');
  mD4 = new CDartBoardSegment(this, p_d4, 8, "green", 'd');
  mD3 = new CDartBoardSegment(this, p_d3, 6, "red", 'd');
  mD2 = new CDartBoardSegment(this, p_d2, 4, "red", 'd');
  mD1 = new CDartBoardSegment(this, p_d1, 2, "green", 'd');
  mDBull = new CDartBoardSegment(this, p_dbull, 50, "red", 'd');
  mT20 = new CDartBoardSegment(this, p_t20, 60, "red", 't');
  mT19 = new CDartBoardSegment(this, p_t19, 57, "green", 't');
  mT18 = new CDartBoardSegment(this, p_t18, 54, "red", 't');
  mT17 = new CDartBoardSegment(this, p_t17, 51, "green", 't');
  mT16 = new CDartBoardSegment(this, p_t16, 48, "green", 't');
  mT15 = new CDartBoardSegment(this, p_t15, 45, "green", 't');
  mT14 = new CDartBoardSegment(this, p_t14, 42, "red", 't');
  mT13 = new CDartBoardSegment(this, p_t13, 39, "red", 't');
  mT12 = new CDartBoardSegment(this, p_t12, 36, "red", 't');
  mT11 = new CDartBoardSegment(this, p_t11, 33, "green", 't');
  mT10 = new CDartBoardSegment(this, p_t10, 30, "red", 't');
  mT9 = new CDartBoardSegment(this, p_t9, 27, "green", 't');
  mT8 = new CDartBoardSegment(this, p_t8, 24, "red", 't');
  mT7 = new CDartBoardSegment(this, p_t7, 21, "red", 't');
  mT6 = new CDartBoardSegment(this, p_t6, 18, "green", 't');
  mT5 = new CDartBoardSegment(this, p_t5, 15, "green", 't');
  mT4 = new CDartBoardSegment(this, p_t4, 12, "green", 't');
  mT3 = new CDartBoardSegment(this, p_t3, 9, "red", 't');
  mT2 = new CDartBoardSegment(this, p_t2, 6, "red", 't');
  mT1 = new CDartBoardSegment(this, p_t1, 3, "green", 't');
  mNoScore = new CDartBoardSegment(this, p_noscore);
  mLabels->setElementId("label");

  mScene->addItem(mNoScore);
  mScene->addItem(mS20);
  mScene->addItem(mS19);
  mScene->addItem(mS18);
  mScene->addItem(mS17);
  mScene->addItem(mS16);
  mScene->addItem(mS15);
  mScene->addItem(mS14);
  mScene->addItem(mS13);
  mScene->addItem(mS12);
  mScene->addItem(mS11);
  mScene->addItem(mS10);
  mScene->addItem(mS9);
  mScene->addItem(mS8);
  mScene->addItem(mS7);
  mScene->addItem(mS6);
  mScene->addItem(mS5);
  mScene->addItem(mS4);
  mScene->addItem(mS3);
  mScene->addItem(mS2);
  mScene->addItem(mS1);
  mScene->addItem(mSBull);
  mScene->addItem(mD20);
  mScene->addItem(mD19);
  mScene->addItem(mD18);
  mScene->addItem(mD17);
  mScene->addItem(mD16);
  mScene->addItem(mD15);
  mScene->addItem(mD14);
  mScene->addItem(mD13);
  mScene->addItem(mD12);
  mScene->addItem(mD11);
  mScene->addItem(mD10);
  mScene->addItem(mD9);
  mScene->addItem(mD8);
  mScene->addItem(mD7);
  mScene->addItem(mD6);
  mScene->addItem(mD5);
  mScene->addItem(mD4);
  mScene->addItem(mD3);
  mScene->addItem(mD2);
  mScene->addItem(mD1);
  mScene->addItem(mDBull);
  mScene->addItem(mT20);
  mScene->addItem(mT19);
  mScene->addItem(mT18);
  mScene->addItem(mT17);
  mScene->addItem(mT16);
  mScene->addItem(mT15);
  mScene->addItem(mT14);
  mScene->addItem(mT13);
  mScene->addItem(mT12);
  mScene->addItem(mT11);
  mScene->addItem(mT10);
  mScene->addItem(mT9);
  mScene->addItem(mT8);
  mScene->addItem(mT7);
  mScene->addItem(mT6);
  mScene->addItem(mT5);
  mScene->addItem(mT4);
  mScene->addItem(mT3);
  mScene->addItem(mT2);
  mScene->addItem(mT1);
  mScene->addItem(mLabels);
  mLabels->setZValue(1);
  mLabels->setScale(0.98);
  mLabels->setPos(10, 10);
}
#endif

CDartBoard::~CDartBoard()
{
#ifndef TESTING
  delete mS20;
  delete mS19;
  delete mS18;
  delete mS17;
  delete mS16;
  delete mS15;
  delete mS14;
  delete mS13;
  delete mS12;
  delete mS11;
  delete mS10;
  delete mS9;
  delete mS8;
  delete mS7;
  delete mS6;
  delete mS5;
  delete mS4;
  delete mS3;
  delete mS2;
  delete mS1;
  delete mSBull;
  delete mD20;
  delete mD19;
  delete mD18;
  delete mD17;
  delete mD16;
  delete mD15;
  delete mD14;
  delete mD13;
  delete mD12;
  delete mD11;
  delete mD10;
  delete mD9;
  delete mD8;
  delete mD7;
  delete mD6;
  delete mD5;
  delete mD4;
  delete mD3;
  delete mD2;
  delete mD1;
  delete mDBull;
  delete mT20;
  delete mT19;
  delete mT18;
  delete mT17;
  delete mT16;
  delete mT15;
  delete mT14;
  delete mT13;
  delete mT12;
  delete mT11;
  delete mT10;
  delete mT9;
  delete mT8;
  delete mT7;
  delete mT6;
  delete mT5;
  delete mT4;
  delete mT3;
  delete mT2;
  delete mT1;
  delete mNoScore;
  delete mLabels;
  delete mScene;
#endif
}

void CDartBoard::play_game_shot_sound()
{
  mSoundHandler.play_game_shot_sound();
}
