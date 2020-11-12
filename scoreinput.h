#ifndef SCOREINPUT_H
#define SCOREINPUT_H

#include <QDialog>
#include <QGraphicsView>
#include "dartboardsegment.h"
#include <QLCDNumber>
#include "undobutton.h"
#include "submitbutton.h"
#include <QGraphicsSvgItem>
#include <QLabel>

namespace Ui {
class ScoreInput;
}

class ScoreInput : public QDialog
{
    Q_OBJECT
public slots:
    void signalSegmentPressed(int &value, QChar &type);
    void signalUndoButtonPressed();
    void signalSubmitButtonPressed();
signals:
    void signalSubmitButtonPressed2(int &score, int &numberofdarts, int &checkoutattempts, QVector<QString> darts);
public:
    explicit ScoreInput(QWidget *parent = nullptr, int startval = 501, int score = 0, int sets = 1, int legs = 1, bool singleIn = true,
                        bool singleOut = false, bool doubleIn = false, bool doubleOut = true, bool masterIn = false, bool masterOut = false);
    ~ScoreInput();
    void setScore(int value, QChar type, int checkout);

private:
    Ui::ScoreInput *ui;
    QGraphicsView* view;
    int StartVal, Score, Sets, Legs, Counter, OldScore;
    bool SingleIn, SingleOut, DoubleIn, DoubleOut, MasterIn, MasterOut, Stop, Busted;
    QLCDNumber *currentscore;
    QLCDNumber *scoreDart1;
    QLCDNumber *scoreDart2;
    QLCDNumber *scoreDart3;
    UndoButton *undoButton;
    SubmitButton *submitButton;
    QVector<int> Undo{0,0,0};
    QVector<QString> Dart{"","",""};
    QVector<int> CheckoutAttempts;
    QGraphicsScene *mscene;
    QLabel *labelDart1;
    QLabel *labelDart2;
    QLabel *labelDart3;
};

#endif // SCOREINPUT_H
