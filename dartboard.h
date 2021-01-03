#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QDialog>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include <QGraphicsSvgItem>
#include <QLabel>
#include "dartboardview.h"


class DartBoard : public QWidget
{
    Q_OBJECT
public slots:
    void signalSegmentPressed(int &value, QChar &type);
signals:
    void signalSubmitButtonPressed2GameWindow(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);
    void signalSetScore2GroupBoxPlayer();
    void signalEraseDart1();
    void signalEraseDart2();
    void signalEraseDart3();
    void signalDisplayDart1(int val);
    void signalDisplayDart2(int val);
    void signalDisplayDart3(int val);
    void signalDisplayScore(int score);
    void signalUpdateFinishes(int score, int numberOfDarts);
    void signalSetFocusToSubmitButton();
public:
    DartBoard(DartBoardView * iGraphicsViewDartBoard, int iStartVal, int iScore, bool iSingleIn, bool iSingleOut,
              bool iDoubleIn, bool iDoubleOut, bool iMasterIn, bool iMasterOut);
    void setScore(int value, QChar type, int checkout);
    void initDartBoard(int score);
    void performUndo();
    void submitScore();
    void displayScore(int score);
    void displayDart1(int dartVal);
    void displayDart2(int dartVal);
    void displayDart3(int dartVal);
    void eraseAllDarts();
    void eraseDart1();
    void eraseDart2();
    void eraseDart3();
private:
    QGraphicsView* view;
    int StartVal, Score, Counter, OldScore;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Stop, Busted;
    QVector<int> Undo{0,0,0};
    QVector<QString> Dart;
    QVector<int> CheckoutAttempts;
    QGraphicsScene *mscene;
};

#endif // DARTBOARD_H
