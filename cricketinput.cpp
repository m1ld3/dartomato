#include "cricketinput.h"
#include "ui_cricketinput.h"
#include <QGraphicsPathItem>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QGraphicsLinearLayout>
#include <QtDebug>
#include <cmath>
#include <QMessageBox>
#include <QSound>
#include <QVector>
#include "cricketclass.h"

cricketinput::cricketinput(QWidget *parent, int sets, int legs, cricketclass* player, GameWindow* gamewindow, bool cutthroat) :
QDialog(parent),
ui(new Ui::cricketinput), Sets(sets), Legs(legs), Counter(3), Player(player), mGameWindow(gamewindow),
  mSlot15(Player->getSlot15()), mSlot16(Player->getSlot16()), mSlot17(Player->getSlot17()), mSlot18(Player->getSlot18()),
  mSlot19(Player->getSlot19()), mSlot20(Player->getSlot20()), mSlot25(Player->getSlot25()),
  mExtra15(Player->getExtra15()), mExtra16(Player->getExtra16()), mExtra17(Player->getExtra17()), mExtra18(Player->getExtra18()),
  mExtra19(Player->getExtra19()), mExtra20(Player->getExtra20()), mExtra25(Player->getExtra25()), mScore(Player->getScore()), mCutThroat(cutthroat),
  mExtra15s(mGameWindow->computeExtra15s(0, Player->get_player_name())),
  mExtra16s(mGameWindow->computeExtra16s(0, Player->get_player_name())), mExtra17s(mGameWindow->computeExtra17s(0, Player->get_player_name())),
  mExtra18s(mGameWindow->computeExtra18s(0, Player->get_player_name())), mExtra19s(mGameWindow->computeExtra19s(0, Player->get_player_name())),
  mExtra20s(mGameWindow->computeExtra20s(0, Player->get_player_name())), mExtra25s(mGameWindow->computeExtra25s(0, Player->get_player_name())),
  mSlot15Array({mSlot15}), mSlot16Array({mSlot16}), mSlot17Array({mSlot17}), mSlot18Array({mSlot18}), mSlot19Array({mSlot19}),
  mSlot20Array({mSlot20}), mSlot25Array({mSlot25}), mExtra15Array({mExtra15}), mExtra16Array({mExtra16}), mExtra17Array({mExtra17}),
  mExtra18Array({mExtra18}), mExtra19Array({mExtra19}), mExtra20Array({mExtra20}), mExtra25Array({mExtra25}),
  mExtra15sArray({mExtra15s}), mExtra16sArray({mExtra16s}), mExtra17sArray({mExtra17s}), mExtra18sArray({mExtra18s}),
  mExtra19sArray({mExtra19s}), mExtra20sArray({mExtra20s}), mExtra25sArray({mExtra25s})
{
ui->setupUi(this);
Stop = false;
labelScoreDart1 = nullptr;
labelScoreDart2 = nullptr;
labelScoreDart3 = nullptr;
mscene = new QGraphicsScene(0,0,800,800, ui->graphicsView_dartboard);
mscene->setSceneRect(0,0,800,800);
ui->graphicsView_dartboard->setRenderHint(QPainter::SmoothPixmapTransform, true);
ui->graphicsView_dartboard->setRenderHint(QPainter::Antialiasing, true);
ui->graphicsView_dartboard->setScene(mscene);
ui->graphicsView_dartboard->show();

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

p_sbull.moveTo(400+40,400);
p_sbull.arcTo(400-40,400-40,2*40,2*40,0,360);
p_sbull.moveTo(400+20,400);
p_sbull.arcTo(400-20,400-20,2*20,2*20,0,360);

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

p_noscore.moveTo(800,400);
p_noscore.arcTo(0,0,800,800,0,360);
p_noscore.moveTo(400+342,400);
p_noscore.arcTo(400-342,400-342,342*2,342*2,0,360);

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

submitButton = new SubmitButton("Submit");
undoButton = new UndoButton("Undo");
mlabelDart1 = new QLabel("1st");
mlabelDart2 = new QLabel("2nd");
mlabelDart3 = new QLabel("3rd");
labelScoreDart1 = new QLabel("---");
labelScoreDart2 = new QLabel("---");
labelScoreDart3 = new QLabel("---");
mlabelDart1->setGeometry(720,0,30,50);
mlabelDart2->setGeometry(720,50,30,50);
mlabelDart3->setGeometry(720,100,30,50);
labelScoreDart1->setGeometry(750,0,50,50);
labelScoreDart2->setGeometry(750,50,50,50);
labelScoreDart3->setGeometry(750,100,50,50);
labelScoreDart1->setStyleSheet("QLabel { color: darkgreen; font: 18pt}");
labelScoreDart2->setStyleSheet("QLabel { color: darkgreen; font: 18pt}");
labelScoreDart3->setStyleSheet("QLabel { color: darkgreen; font: 18pt}");
submitButton->setGeometry(0,750,100,50);
undoButton->setGeometry(700,750,100,50);
QFont font("System", 25);
submitButton->setFont(font);
mlabelDart1->setFont(font);
mlabelDart2->setFont(font);
mlabelDart3->setFont(font);
undoButton->setFont(font);
mscene->addWidget(submitButton);
mscene->addWidget(undoButton);
mscene->addWidget(mlabelDart1);
mscene->addWidget(mlabelDart2);
mscene->addWidget(mlabelDart3);
mscene->addWidget(labelScoreDart1);
mscene->addWidget(labelScoreDart2);
mscene->addWidget(labelScoreDart3);

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
connect(undoButton, SIGNAL (signalUndoButtonPressed()), this, SLOT (signalUndoButtonPressed()));
connect(submitButton, SIGNAL (signalSubmitButtonPressed()), this, SLOT (signalSubmitButtonPressed()));
}

cricketinput::~cricketinput()
{
    delete ui;
    delete mscene;
}

void cricketinput::setScore(int value, QChar type) {
    if (type == 't') {
        value = value/3;
    } else if (type == 'd') {
        value = value/2;
    }
    if (value > 0) {
        if (Counter == 3) {
            QString temp = type.toUpper()+QString::number(value);
            labelScoreDart1->setText(temp);
            Dart[0] = type + QString::number(value);
        } else if (Counter == 2) {
            QString temp = type.toUpper()+QString::number(value);
            labelScoreDart2->setText(temp);
            Dart[1] = type + QString::number(value);
        } else if (Counter == 1) {
            QString temp = type.toUpper()+QString::number(value);
            labelScoreDart3->setText(temp);
            Dart[2] = type + QString::number(value);
        }
    } else {
        if (Counter == 3) {
            labelScoreDart1->setNum(0);
            Dart[0] = type + QString::number(value);
        } else if (Counter == 2) {
            labelScoreDart2->setNum(0);
            Dart[1] = type + QString::number(value);
        } else if (Counter == 1) {
            labelScoreDart3->setNum(0);
            Dart[2] = type + QString::number(value);
        }
    }
}

void cricketinput::computeScore()
{
    mScore = mExtra15 + mExtra16 + mExtra17 + mExtra18 + mExtra19 + mExtra20 + mExtra25;
}

bool cricketinput::areSlotsFull()
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

void cricketinput::signalSegmentPressed(int &value, QChar &type)
{
    bool result = true;
    QString dart = type.toUpper()+QString::number(value);
    if (!mCutThroat) {
        if (!Stop) {
            if (Counter > 0) {
                setScore(value, type);
                int hits;
                if (type == 't') {
                    hits = 3;
                } else if (type == 'd') {
                    hits = 2;
                } else {
                    hits = 1;
                }
                int val = value;
                val /= hits;
                switch (val) {
                case 15:
                    if (mSlot15 < 3) {
                        if (mSlot15 + hits <= 3) {
                            mSlot15 += hits;
                        } else {
                            hits -= 3 - mSlot15;
                            mSlot15 = 3;
                            if (mGameWindow->isSlot15free(Player->get_player_name())) {
                                mExtra15 += hits * 15;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot15free(Player->get_player_name())) {
                            mExtra15 += hits * 15;
                        }
                    } break;
                case 16:
                    if (mSlot16 < 3) {
                        if (mSlot16 + hits <= 3) {
                            mSlot16 += hits;
                        } else {
                            hits -= 3 - mSlot16;
                            mSlot16 = 3;
                            if (mGameWindow->isSlot16free(Player->get_player_name())) {
                                mExtra16 += hits * 16;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot16free(Player->get_player_name())) {
                            mExtra16 += hits * 16;
                        }
                    } break;
                case 17:
                    if (mSlot17 < 3) {
                        if (mSlot17 + hits <= 3) {
                            mSlot17 += hits;
                        } else {
                            hits -= 3 - mSlot17;
                            mSlot17 = 3;
                            if (mGameWindow->isSlot17free(Player->get_player_name())) {
                                mExtra17 += hits * 17;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot17free(Player->get_player_name())) {
                            mExtra17 += hits * 17;
                        }
                    } break;
                case 18:
                    if (mSlot18 < 3) {
                        if (mSlot18 + hits <= 3) {
                            mSlot18 += hits;
                        } else {
                            hits -= 3 - mSlot18;
                            mSlot18 = 3;
                            if (mGameWindow->isSlot18free(Player->get_player_name())) {
                                mExtra18 += hits * 18;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot18free(Player->get_player_name())) {
                            mExtra18 += hits * 18;
                        }
                    } break;
                case 19:
                    if (mSlot19 < 3) {
                        if (mSlot19 + hits <= 3) {
                            mSlot19 += hits;
                        } else {
                            hits -= 3 - mSlot19;
                            mSlot19 = 3;
                            if (mGameWindow->isSlot19free(Player->get_player_name())) {
                                mExtra19 += hits * 19;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot19free(Player->get_player_name())) {
                            mExtra19 += hits * 19;
                        }
                    } break;
                case 20:
                    if (mSlot20 < 3) {
                        if (mSlot20 + hits <= 3) {
                            mSlot20 += hits;
                        } else {
                            hits -= 3 - mSlot20;
                            mSlot20 = 3;
                            if (mGameWindow->isSlot20free(Player->get_player_name())) {
                                mExtra20 += hits * 20;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot20free(Player->get_player_name())) {
                            mExtra20 += hits * 20;
                        }
                    } break;
                case 25:
                    if (mSlot25 < 3) {
                        if (mSlot25 + hits <= 3) {
                            mSlot25 += hits;
                        } else {
                            hits -= 3 - mSlot25;
                            mSlot25 = 3;
                            if (mGameWindow->isSlot25free(Player->get_player_name())) {
                                mExtra25 += hits * 25;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot25free(Player->get_player_name())) {
                            mExtra25 += hits * 25;
                        }
                    }
                }
                computeScore();
                if (areSlotsFull() && mGameWindow->isScoreBigger(mScore)) {
                    Stop = true;  // Game shot
                    QSound *gameshotsound = new QSound("qrc:/resources/sounds/gameshot.wav");
                    gameshotsound->play();
                    if (gameshotsound->isFinished()) delete gameshotsound;
                }
                Counter--;
                if (Counter == 0) {
                    Stop = true;
                }
            }
        } else if (areSlotsFull() && mGameWindow->isScoreBigger(mScore)) {
            QMessageBox::warning(this, "Warning", "You have already won this leg!");
        } else {
            QMessageBox::warning(this, "Warning", "You only have three darts!");
        }
    } else {
        if (!Stop) {
            if (Counter > 0) {
                setScore(value, type);
                int hits;
                if (type == 't') {
                    hits = 3;
                } else if (type == 'd') {
                    hits = 2;
                } else {
                    hits = 1;
                }
                int val = value;
                val /= hits;
                switch (val) {
                case 15:
                    if (mSlot15 < 3) {
                        if (mSlot15 + hits <= 3) {
                            mSlot15 += hits;
                        } else {
                            hits -= 3 - mSlot15;
                            mSlot15 = 3;
                            if (mGameWindow->isSlot15free(Player->get_player_name())) {
                                mExtra15 += hits * 15;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot15free(Player->get_player_name())) {
                            mExtra15 += hits * 15;
                        }
                    } break;
                case 16:
                    if (mSlot16 < 3) {
                        if (mSlot16 + hits <= 3) {
                            mSlot16 += hits;
                        } else {
                            hits -= 3 - mSlot16;
                            mSlot16 = 3;
                            if (mGameWindow->isSlot16free(Player->get_player_name())) {
                                mExtra16 += hits * 16;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot16free(Player->get_player_name())) {
                            mExtra16 += hits * 16;
                        }
                    } break;
                case 17:
                    if (mSlot17 < 3) {
                        if (mSlot17 + hits <= 3) {
                            mSlot17 += hits;
                        } else {
                            hits -= 3 - mSlot17;
                            mSlot17 = 3;
                            if (mGameWindow->isSlot17free(Player->get_player_name())) {
                                mExtra17 += hits * 17;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot17free(Player->get_player_name())) {
                            mExtra17 += hits * 17;
                        }
                    } break;
                case 18:
                    if (mSlot18 < 3) {
                        if (mSlot18 + hits <= 3) {
                            mSlot18 += hits;
                        } else {
                            hits -= 3 - mSlot18;
                            mSlot18 = 3;
                            if (mGameWindow->isSlot18free(Player->get_player_name())) {
                                mExtra18 += hits * 18;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot18free(Player->get_player_name())) {
                            mExtra18 += hits * 18;
                        }
                    } break;
                case 19:
                    if (mSlot19 < 3) {
                        if (mSlot19 + hits <= 3) {
                            mSlot19 += hits;
                        } else {
                            hits -= 3 - mSlot19;
                            mSlot19 = 3;
                            if (mGameWindow->isSlot19free(Player->get_player_name())) {
                                mExtra19 += hits * 19;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot19free(Player->get_player_name())) {
                            mExtra19 += hits * 19;
                        }
                    } break;
                case 20:
                    if (mSlot20 < 3) {
                        if (mSlot20 + hits <= 3) {
                            mSlot20 += hits;
                        } else {
                            hits -= 3 - mSlot20;
                            mSlot20 = 3;
                            if (mGameWindow->isSlot20free(Player->get_player_name())) {
                                mExtra20 += hits * 20;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot20free(Player->get_player_name())) {
                            mExtra20 += hits * 20;
                        }
                    } break;
                case 25:
                    if (mSlot25 < 3) {
                        if (mSlot25 + hits <= 3) {
                            mSlot25 += hits;
                        } else {
                            hits -= 3 - mSlot25;
                            mSlot25 = 3;
                            if (mGameWindow->isSlot25free(Player->get_player_name())) {
                                mExtra25 += hits * 25;
                            }
                        }
                    } else {
                        if (mGameWindow->isSlot25free(Player->get_player_name())) {
                            mExtra25 += hits * 25;
                        }
                    }
                }
                QVector<int> temp15s = mGameWindow->computeExtra15s(mExtra15, Player->get_player_name());
                QVector<int> temp16s = mGameWindow->computeExtra16s(mExtra16, Player->get_player_name());
                QVector<int> temp17s = mGameWindow->computeExtra17s(mExtra17, Player->get_player_name());
                QVector<int> temp18s = mGameWindow->computeExtra18s(mExtra18, Player->get_player_name());
                QVector<int> temp19s = mGameWindow->computeExtra19s(mExtra19, Player->get_player_name());
                QVector<int> temp20s = mGameWindow->computeExtra20s(mExtra20, Player->get_player_name());
                QVector<int> temp25s = mGameWindow->computeExtra25s(mExtra25, Player->get_player_name());
                QVector<int> scores(temp15s.size());
                for (int i = 0; i < mExtra15s.size(); i++) {
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
                for (int i = 0; i < scores.size(); i++) {
                    result = result && scores[i] >= mScore;
                }
                if (areSlotsFull() && result) {
                    Stop = true;  // Game shot
                    QSound *gameshotsound = new QSound("qrc:/resources/sounds/gameshot.wav");
                    gameshotsound->play();
                    if (gameshotsound->isFinished()) delete gameshotsound;
                }
                Counter--;
                if (Counter == 0) {
                    Stop = true;
                }
            }
        } else if (areSlotsFull() && result) {
            QMessageBox::warning(this, "Warning", "You have already won this leg!");
        } else {
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

    void cricketinput::signalUndoButtonPressed()
    {
        if (Counter < 3) {
            Dart[2-Counter] = "";
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
            if (mCutThroat) {
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

            if (Counter == 2) {
                labelScoreDart1->setText("---");
            } else if (Counter == 1) {
                labelScoreDart2->setText("---");
            } else if (Counter == 0) {
                labelScoreDart3->setText("---");
            }
            Counter++;
            Stop = false;
        }
    }

    void cricketinput::signalSubmitButtonPressed()
    {
        if (Stop) {
            int numberofdarts = 3 - Counter;
            emit signalSubmitButtonPressed2(numberofdarts, Dart);
        } else {
            QMessageBox::warning(this, "Score incomplete", "Please enter all darts.");
        }
    }
