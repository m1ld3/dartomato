#include "dartboard.h"
#include <cmath>
#include <QMessageBox>

DartBoard::DartBoard(DartBoardView * iGraphicsViewDartBoard, int iStartVal, int iScore, bool iSingleIn, bool iSingleOut,
                     bool iDoubleIn, bool iDoubleOut, bool iMasterIn, bool iMasterOut)
    :
      StartVal(iStartVal)
    , Score(iScore)
    , Counter(3)
    , SingleIn(iSingleIn)
    , SingleOut(iSingleOut)
    , DoubleIn(iDoubleIn)
    , DoubleOut(iDoubleOut)
    , MasterIn(iMasterIn)
    , MasterOut(iMasterOut)
    , Finished(false)
    , Dart({})
    , busted(this)
    , gameshotsound(this)
{
    CheckoutAttempts = {0, 0, 0};
    mscene = new QGraphicsScene(0,0,800,800, iGraphicsViewDartBoard);
    mscene->setSceneRect(0,0,800,800);
    iGraphicsViewDartBoard->setRenderHint(QPainter::SmoothPixmapTransform, true);
    iGraphicsViewDartBoard->setRenderHint(QPainter::Antialiasing, true);
    iGraphicsViewDartBoard->setScene(mscene);
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

    p_noscore.addEllipse(QRect(0,0,800,800));
    p_noscore.addEllipse(QRect(58,58,684,684));

    DartboardSegment * s20 = new DartboardSegment(p_s20,20);
    DartboardSegment * s19 = new DartboardSegment(p_s19,19,"beige");
    DartboardSegment * s18 = new DartboardSegment(p_s18,18);
    DartboardSegment * s17 = new DartboardSegment(p_s17,17,"beige");
    DartboardSegment * s14 = new DartboardSegment(p_s14,14);
    DartboardSegment * s13 = new DartboardSegment(p_s13,13);
    DartboardSegment * s12 = new DartboardSegment(p_s12,12);
    DartboardSegment * s11 = new DartboardSegment(p_s11,11,"beige");
    DartboardSegment * s16 = new DartboardSegment(p_s16,16,"beige");
    DartboardSegment * s15 = new DartboardSegment(p_s15,15,"beige");
    DartboardSegment * s10 = new DartboardSegment(p_s10,10);
    DartboardSegment * s9 = new DartboardSegment(p_s9,9,"beige");
    DartboardSegment * s8 = new DartboardSegment(p_s8,8);
    DartboardSegment * s7 = new DartboardSegment(p_s7,7);
    DartboardSegment * s6 = new DartboardSegment(p_s6,6,"beige");
    DartboardSegment * s5 = new DartboardSegment(p_s5,5,"beige");
    DartboardSegment * s4 = new DartboardSegment(p_s4,4,"beige");
    DartboardSegment * s3 = new DartboardSegment(p_s3,3);
    DartboardSegment * s2 = new DartboardSegment(p_s2,2);
    DartboardSegment * s1 = new DartboardSegment(p_s1,1,"beige");
    DartboardSegment * sbull = new DartboardSegment(p_sbull,25,"green");
    DartboardSegment * d20 = new DartboardSegment(p_d20,40,"red",'d');
    DartboardSegment * d19 = new DartboardSegment(p_d19,38,"green",'d');
    DartboardSegment * d18 = new DartboardSegment(p_d18,36,"red",'d');
    DartboardSegment * d17 = new DartboardSegment(p_d17,34,"green",'d');
    DartboardSegment * d16 = new DartboardSegment(p_d16,32,"green",'d');
    DartboardSegment * d15 = new DartboardSegment(p_d15,30,"green",'d');
    DartboardSegment * d14 = new DartboardSegment(p_d14,28,"red",'d');
    DartboardSegment * d13 = new DartboardSegment(p_d13,26,"red",'d');
    DartboardSegment * d12 = new DartboardSegment(p_d12,24,"red",'d');
    DartboardSegment * d11 = new DartboardSegment(p_d11,22,"green",'d');
    DartboardSegment * d10 = new DartboardSegment(p_d10,20,"red",'d');
    DartboardSegment * d9 = new DartboardSegment(p_d9,18,"green",'d');
    DartboardSegment * d8 = new DartboardSegment(p_d8,16,"red",'d');
    DartboardSegment * d7 = new DartboardSegment(p_d7,14,"red",'d');
    DartboardSegment * d6 = new DartboardSegment(p_d6,12,"green",'d');
    DartboardSegment * d5 = new DartboardSegment(p_d5,10,"green",'d');
    DartboardSegment * d4 = new DartboardSegment(p_d4,8,"green",'d');
    DartboardSegment * d3 = new DartboardSegment(p_d3,6,"red",'d');
    DartboardSegment * d2 = new DartboardSegment(p_d2,4,"red",'d');
    DartboardSegment * d1 = new DartboardSegment(p_d1,2,"green",'d');
    DartboardSegment * dbull = new DartboardSegment(p_dbull,50,"red",'d');
    DartboardSegment * t20 = new DartboardSegment(p_t20,60,"red",'t');
    DartboardSegment * t19 = new DartboardSegment(p_t19,57,"green",'t');
    DartboardSegment * t18 = new DartboardSegment(p_t18,54,"red",'t');
    DartboardSegment * t17 = new DartboardSegment(p_t17,51,"green",'t');
    DartboardSegment * t16 = new DartboardSegment(p_t16,48,"green",'t');
    DartboardSegment * t15 = new DartboardSegment(p_t15,45,"green",'t');
    DartboardSegment * t14 = new DartboardSegment(p_t14,42,"red",'t');
    DartboardSegment * t13 = new DartboardSegment(p_t13,39,"red",'t');
    DartboardSegment * t12 = new DartboardSegment(p_t12,36,"red",'t');
    DartboardSegment * t11 = new DartboardSegment(p_t11,33,"green",'t');
    DartboardSegment * t10 = new DartboardSegment(p_t10,30,"red",'t');
    DartboardSegment * t9 = new DartboardSegment(p_t9,27,"green",'t');
    DartboardSegment * t8 = new DartboardSegment(p_t8,24,"red",'t');
    DartboardSegment * t7 = new DartboardSegment(p_t7,21,"red",'t');
    DartboardSegment * t6 = new DartboardSegment(p_t6,18,"green",'t');
    DartboardSegment * t5 = new DartboardSegment(p_t5,15,"green",'t');
    DartboardSegment * t4 = new DartboardSegment(p_t4,12,"green",'t');
    DartboardSegment * t3 = new DartboardSegment(p_t3,9,"red",'t');
    DartboardSegment * t2 = new DartboardSegment(p_t2,6,"red",'t');
    DartboardSegment * t1 = new DartboardSegment(p_t1,3,"green",'t');
    DartboardSegment * noscore = new DartboardSegment(p_noscore);
    QGraphicsSvgItem * labels = new QGraphicsSvgItem(":/resources/img/dartboard.svg");
    labels->setElementId("label");

    mscene->addItem(noscore);
    mscene->addItem(s20);
    mscene->addItem(s19);
    mscene->addItem(s18);
    mscene->addItem(s17);
    mscene->addItem(s16);
    mscene->addItem(s15);
    mscene->addItem(s14);
    mscene->addItem(s13);
    mscene->addItem(s12);
    mscene->addItem(s11);
    mscene->addItem(s10);
    mscene->addItem(s9);
    mscene->addItem(s8);
    mscene->addItem(s7);
    mscene->addItem(s6);
    mscene->addItem(s5);
    mscene->addItem(s4);
    mscene->addItem(s3);
    mscene->addItem(s2);
    mscene->addItem(s1);
    mscene->addItem(sbull);
    mscene->addItem(d20);
    mscene->addItem(d19);
    mscene->addItem(d18);
    mscene->addItem(d17);
    mscene->addItem(d16);
    mscene->addItem(d15);
    mscene->addItem(d14);
    mscene->addItem(d13);
    mscene->addItem(d12);
    mscene->addItem(d11);
    mscene->addItem(d10);
    mscene->addItem(d9);
    mscene->addItem(d8);
    mscene->addItem(d7);
    mscene->addItem(d6);
    mscene->addItem(d5);
    mscene->addItem(d4);
    mscene->addItem(d3);
    mscene->addItem(d2);
    mscene->addItem(d1);
    mscene->addItem(dbull);
    mscene->addItem(t20);
    mscene->addItem(t19);
    mscene->addItem(t18);
    mscene->addItem(t17);
    mscene->addItem(t16);
    mscene->addItem(t15);
    mscene->addItem(t14);
    mscene->addItem(t13);
    mscene->addItem(t12);
    mscene->addItem(t11);
    mscene->addItem(t10);
    mscene->addItem(t9);
    mscene->addItem(t8);
    mscene->addItem(t7);
    mscene->addItem(t6);
    mscene->addItem(t5);
    mscene->addItem(t4);
    mscene->addItem(t3);
    mscene->addItem(t2);
    mscene->addItem(t1);
    mscene->addItem(labels);
    labels->setZValue(1);
    labels->setScale(0.98);
    labels->setPos(10,10);
    eraseAllDarts();
    displayScore(Score);

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
    busted.setSource(QUrl("qrc:/resources/sounds/busted.wav"));
    gameshotsound.setSource(QUrl("qrc:/resources/sounds/gameshot.wav"));
}

void DartBoard::setScore(int value, QChar type, int checkout) {
    Score -= value;
    displayScore(Score);
    if (Counter == 3) {
        displayDart1(value);
        emit signalUpdateFinishes(Score, 2);
        Undo[0] = value;
        if (!Busted)
        {
            Dart.append(type+QString::number(value));
        }
        else if (Busted)
        {
            Dart.append("S0");
        }
        CheckoutAttempts[0] = checkout;
    } else if (Counter == 2) {
        displayDart2(value);
        emit signalUpdateFinishes(Score, 1);
        Undo[1] = value;
        if (!Busted)
        {
            Dart.append(type+QString::number(value));
        }
        else if (Busted)
        {
            Dart.append("S0");
        }
        CheckoutAttempts[1] = checkout;
    } else if (Counter == 1) {
        displayDart3(value);
        Undo[2] = value;
        if (!Busted)
        {
            Dart.append(type+QString::number(value));
        }
        else if (Busted)
        {
            Dart.append("S0");
        }
        CheckoutAttempts[2] = checkout;
    }
}

void DartBoard::initDartBoard(int score)
{
    Stop = false;
    Busted = false;
    Score = score;
    OldScore = Score;
    CheckoutAttempts = {0, 0, 0};
    eraseAllDarts();
    displayScore(Score);
    Counter = 3;
    Dart = {};
}

void DartBoard::signalSegmentPressed(int &value, QChar &type)
{
    if (Finished)
    {
        QMessageBox::warning(this, "","Game already finished!");
        return;
    }
    QVector<int> array {23, 29, 31, 35, 37, 41, 43, 44, 46, 47, 49, 52, 53, 55, 56, 58, 59};
    int checkout = 0;
    if (!Stop) {
        if (Counter > 0) {
            if (Score == StartVal) {
                if (SingleIn) {
                    setScore(value, type, checkout);
                } else if (DoubleIn) {
                    if (type == 'd') {
                        setScore(value, type, checkout);
                    } else {
                        setScore(0, type, checkout);
                    }
                } else if (MasterIn) {
                    if (type == 't') {
                        setScore(value, type, checkout);
                    } else {
                        setScore(0, type, checkout);
                    }
                }
            } else if (Score > value && Score < StartVal && SingleOut) {
                if (Score <= 60 && !(std::find(array.begin(), array.end(), Score))) {
                    checkout = 1;
                }
                setScore(value, type, checkout);
            } else if (Score - value > 1 && Score < StartVal && DoubleOut) {
                if ((Score <= 40 && Score % 2 == 0 && Score > 1) || Score == 50) {
                    checkout = 1;
                }
                setScore(value, type, checkout);
            } else if (Score - value > 2 && Score < StartVal && MasterOut) {
                if (Score <= 60 && Score % 3 == 0 && Score > 2) {
                    checkout = 1;
                }
                setScore(value, type, checkout);
            } else if (Score == value) {
                if (SingleOut) {
                    Stop = true;  // Game shot
                    emit signalSetFocusToSubmitButton();
                    gameshotsound.play();
                    checkout = 1;
                    setScore(value, type, checkout);
                } else if (DoubleOut) {
                    if (type == 'd') {
                        Stop = true; // Game shot
                        emit signalSetFocusToSubmitButton();
                        gameshotsound.play();
                        checkout = 1;
                        setScore(value, type, checkout);
                    } else {
                        if ((Score > 1 && Score % 2 == 0 && Score <= 40) || Score == 50) {
                            checkout = 1;
                        }
                        setScore(value, type, checkout);
                        displayScore(OldScore);
                        Stop = true; // Überwofen
                        Busted = true;
                        emit signalSetFocusToSubmitButton();
                        busted.play();
                    }
                } else if (MasterOut) {
                    if (type == 't') {
                        checkout = 1;
                        Stop = true; // Game shot
                        gameshotsound.play();
                        setScore(value, type, checkout);
                    } else {
                        if (Score <= 60 && Score % 3 == 0 && Score > 2) {
                            checkout = 1;
                        }
                        setScore(value, type, checkout);
                        displayScore(OldScore);
                        Stop = true; // Überwofen
                        Busted = true;
                        emit signalSetFocusToSubmitButton();
                        busted.play();
                    }
                }
            } else {
                if (SingleOut && (Score <= 60 && !(std::find(array.begin(), array.end(), Score)))) {
                    checkout = 1;
                } else if (DoubleOut && ((Score <= 40 && Score % 2 == 0 && Score > 1) || Score == 50)) {
                    checkout = 1;
                } else if (MasterOut && (Score <= 60 && Score % 3 == 0 && Score > 2)) {
                    checkout = 1;
                }
                Stop = true;
                Busted = true; // Überworfen
                emit signalSetFocusToSubmitButton();
                setScore(value, type, checkout);
                displayScore(OldScore);
                busted.play();
            }
            Counter--;
            if (Counter == 0) {
                Stop = true;
                emit signalSetFocusToSubmitButton();
            }
        }
    } else if (Busted) {
        QMessageBox::warning(this, "Warning", "You are already busted!");
    } else if (Score == 0) {
        QMessageBox::warning(this, "Warning", "You have already won this leg!");
    } else if (Score > 0 && Counter == 0) {
        QMessageBox::warning(this, "Warning", "You only have three darts!");
    }
}

void DartBoard::performUndo()
{
    if (Counter < 3) {
        Score += Undo[2-Counter];
        Undo[2-Counter] = 0;
        Dart.pop_back();
        CheckoutAttempts[2-Counter] = 0;
        displayScore(Score);
        if (Counter == 2) {
            eraseDart1();
            emit signalUpdateFinishes(Score, 3);
        } else if (Counter == 1) {
            eraseDart2();
            emit signalUpdateFinishes(Score, 2);
        } else if (Counter == 0) {
            eraseDart3();
            emit signalUpdateFinishes(Score, 1);
        }
        Counter++;
        Stop = false;
        Busted = false;
    }
}

void DartBoard::submitScore()
{
    if (Stop) {
        QVector<QString> darts = Dart;
        int score;
        if (Busted)
        {
            darts = {"S0", "S0", "S0"};
            score = 0;
        }
        else
        {
            score = OldScore - Score;
        }
        int numberofdarts = 3 - Counter;
        int checkoutattempts = std::accumulate(CheckoutAttempts.begin(), CheckoutAttempts.end(),0);
        emit signalSubmitButtonPressed2GameWindow(score, numberofdarts, checkoutattempts, darts);
    } else if (!Finished)
    {
        QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
    }
    else
    {
        QMessageBox::warning(this, "","Game already finished!");
    }
}

void DartBoard::displayScore(int score)
{
    emit signalDisplayScore(score);
}

void DartBoard::displayDart1(int dartVal)
{
    emit signalDisplayDart1(dartVal);
}

void DartBoard::displayDart2(int dartVal)
{
    emit signalDisplayDart2(dartVal);
}

void DartBoard::displayDart3(int dartVal)
{
    emit signalDisplayDart3(dartVal);
}

void DartBoard::eraseAllDarts()
{
    eraseDart1();
    eraseDart2();
    eraseDart3();
}

void DartBoard::eraseDart1()
{
    emit signalEraseDart1();
}

void DartBoard::eraseDart2()
{
    emit signalEraseDart2();
}

void DartBoard::eraseDart3()
{
    emit signalEraseDart3();
}

void DartBoard::setFinished()
{
    Finished = true;
}

void DartBoard::unsetFinished()
{
    Finished = false;
}



