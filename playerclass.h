#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H

#include <QVector>
#include <QObject>

class PlayerClass : public QObject
{
    Q_OBJECT
public:
    PlayerClass(int startval, int numberofsets, int numberoflegs, int playername);
    int set_score(int& score);
    void set_darts(QVector<QString> darts);
    QVector<QVector<QString>> get_darts();
    QVector<int> get_total_scores();
    void undo();
    void compute_averages(int numberofdarts);
    void compute_checkout(int checkoutattempts, int success);
    double get_avg1dart();
    double get_avg3dart();
    double get_checkout();
    QVector<int> get_LegScores();
    QVector<QVector<QString>> get_LegDarts();
    QVector<QVector<int>> get_TotalScores();
    QVector<QVector<QVector<QString>>> getThrownDartsOfAllLegs();
    int get_remaining();
    QVector<int> get_LegRemaining();
    QVector<QVector<int>> get_RemainingOfAllLegs();
    int get_legs();
    int get_sets();
    void update_history();
    void resetScore();
    void resetLegs();
    int getPlayerName();
    void undoStep();
    QVector<int> getScoreLeg();
    QString getCheckoutAttempts();

public slots:
    bool increase_setslegs();

signals:
    void signalGameWon(int playername);

private:
    int sets;   //  sets won
    int legs;   //  legs won per set
    int totallegs;  //  total legs won
    int remaining;  //  current remaining points
    double avg1dart;  //  1-dart average
    double avg3dart;  //  3-dart average
    double checkout;  //  checkout percentage
    int CheckoutAttempts;  // checkout attempts
    int Checkouts;  //  successful checkouts
    int numberoflegs;  //  best of "numberoflegs" legs mode
    int numberofsets;  //  best of "numberofsets" sets mode
    int margin_legs, margin_sets;  //  required legs/sets to win for one set
    int PlayerName;  //  instance of player
    int StartVal;
    int TotalDarts;  //  total amount of thrown darts
    QVector<int> mScoresOfCurrentLeg;  //  all scores of current leg
    QVector<int> mRemainingPointsOfCurrentLeg;  //  all intermediate remaining points of current leg
    QVector<QVector<int>> mRemainingPointsOfAllLegs;
    QVector<QVector<int>> mAllScoresOfAllLegs;  //  complete scoring history of the current game
    QVector<int> mAllScoresFlat;  //  all scores in one vector
    QVector<int> numberofdartsarray;
    QVector<int> checkoutattemptarray;
    QVector<int> checkoutarray;
    QVector<QVector<QString>> mThrownDartsOfCurrentLeg;
    QVector<QVector<QString>> mThrownDartsOfAllLegsFlat;
    QVector<QVector<QVector<QString>>> mThrownDartsOfAllLegs;
};

#endif // PLAYERCLASS_H
