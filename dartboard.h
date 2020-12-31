#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QDialog>
#include <QGraphicsView>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include "undobutton.h"
#include "submitbutton.h"
#include <QGraphicsSvgItem>
#include <QLabel>


class DartBoard : public QWidget
{
    Q_OBJECT
public slots:
    void signalSegmentPressed(int &value, QChar &type);
    void signalUndoButtonPressed();
    void signalSubmitButtonPressed();
signals:
    void signalSubmitButtonPressed2GameWindow(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);
    void signalSetScore2GroupBoxPlayer();
public:
    DartBoard(QGraphicsView * iGraphicsViewDartBoard, int iStartVal, int iScore, bool iSingleIn, bool iSingleOut,
              bool iDoubleIn, bool iDoubleOut, bool iMasterIn, bool iMasterOut);
    void setScore(int value, QChar type, int checkout);
    void initDartBoard(int score);
private:
    QGraphicsView* view;
    int StartVal, Score, Counter, OldScore;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Stop, Busted;
    QLCDNumber *currentscore;
    QLCDNumber *scoreDart1;
    QLCDNumber *scoreDart2;
    QLCDNumber *scoreDart3;
    UndoButton *undoButton;
    SubmitButton *submitButton;
    QVector<int> Undo{0,0,0};
    QVector<QString> Dart;
    QVector<int> CheckoutAttempts;
    QGraphicsScene *mscene;
    QLabel *labelDart1;
    QLabel *labelDart2;
    QLabel *labelDart3;
};

#endif // DARTBOARD_H
