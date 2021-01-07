#ifndef CRICKETCLASS_H
#define CRICKETCLASS_H

#include <QObject>
#include "cricketmainwindow.h"

class CricketMainWindow;

class cricketclass : public QObject
{
    Q_OBJECT
public:
    cricketclass(QWidget* parent, int numberofsets, int numberoflegs, int playername, bool cutthroat = false);
    void undo();
    double compute_hits_per_round(int numberofdarts, int totalhits);
    int get_legs();
    int get_sets();
    void update_history();
    void update_darts(QVector<QString> darts);
    void reset_score();
    void reset_legs();
    void undoStep();
    int get_player_name();
    int getSlot15();
    int getSlot16();
    int getSlot17();
    int getSlot18();
    int getSlot19();
    int getSlot20();
    int getSlot25();
    void setSlot15(int hits);
    void setSlot16(int hits);
    void setSlot17(int hits);
    void setSlot18(int hits);
    void setSlot19(int hits);
    void setSlot20(int hits);
    void setSlot25(int hits);
    void setExtra15(int points);
    void setExtra16(int points);
    void setExtra17(int points);
    void setExtra18(int points);
    void setExtra19(int points);
    void setExtra20(int points);
    void setExtra25(int points);
    int getExtra15();
    int getExtra16();
    int getExtra17();
    int getExtra18();
    int getExtra19();
    int getExtra20();
    int getExtra25();
    int getTotalHits();
    bool areSlotsFull();
    void setScore();
    int getScore();
    double get_hits_per_round();
    QVector<QVector<QString>> getMScoreLegs();
    QVector<QVector<QVector<QString>>> getmScoringHistory();
    void setLegWinArray(bool finished);
    void setTotalHits(int hits);
    void setTotalDarts(int darts);
    int getTotalDarts();

public slots:
    bool increase_setslegs();
signals:
    void signal_game_won(int playername);
private:
    CricketMainWindow* mGameWindow;
    int mSets;   //  sets won
    int mLegs;   //  legs won per set
    int mTotalLegs;  //  total legs won
    bool CutThroat;
    QVector<QVector<QString>> mScoreLegs;  //  all rounds of current leg
    QVector<QVector<QVector<QString>>> mScoringHistory;  //  complete scoring history of the current game
    QVector<QVector<QString>> mTotalScores;  //  all rounds in one vector
    int mNumberOfLegs;  //  best of "numberoflegs" legs mode
    int mNumberOfSets;  //  best of "numberofsets" sets mode
    int mMarginLegs, mMarginSets;  //  required legs/sets to win for one set
    int mPlayerName;  //  instance of player
    QVector<int> mNumberOfDartsArray;
    int mTotalDarts;  //  total amount of thrown darts
    int mTotalHits;  // total amount of hits
    QVector<int> mNumberOfHitsArray;
    double mHitsPerRound;
    int mSlot15, mSlot16, mSlot17, mSlot18, mSlot19, mSlot20, mSlot25;
    int mExtra15, mExtra16, mExtra17, mExtra18, mExtra19, mExtra20, mExtra25;
    int mScore;
    QVector<int> mSlot15Array;
    QVector<int> mSlot16Array;
    QVector<int> mSlot17Array;
    QVector<int> mSlot18Array;
    QVector<int> mSlot19Array;
    QVector<int> mSlot20Array;
    QVector<int> mSlot25Array;
    QVector<int> mExtra15Array;
    QVector<int> mExtra16Array;
    QVector<int> mExtra17Array;
    QVector<int> mExtra18Array;
    QVector<int> mExtra19Array;
    QVector<int> mExtra20Array;
    QVector<int> mExtra25Array;
    QVector<int> mScoreArray;
    QVector<bool> mLegWinArray;
};

#endif // CRICKETCLASS_H
