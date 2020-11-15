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
    QVector<QVector<int>> get_TotalScores();
    int get_remaining();
    int get_legs();
    int get_sets();
    void update_history();
    void resetScore();
//    void updateStats(int score180, int plus160, int plus140, int plus120, int plus100, int score26,
//                     int score11, int score7, int score3, int t20, int t19, int t18, int t17, int t16,
//                     int t15, int bull, int ones, int miss);
//    int get_180();
//    int get_160();
//    int get_140();
//    int get_120();
//    int get_100();
//    int get_26();
//    int get_11();
//    int get_7();
//    int get_3();
//    int get_t20();
//    int get_t19();
//    int get_t18();
//    int get_t17();
//    int get_t16();
//    int get_t15();
//    int get_bull();
//    int get_ones();
//    int get_miss();
    void resetLegs();
    int getPlayerName();
    void undoStep();

    QVector<QVector<int>> getScoreTotal();
    QVector<int> getScoreLeg();

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
    QVector<int> score_leg;  //  all scores of current leg
    QVector<int> points_leg;  //  all intermediate remaining points of current leg
    QVector<QVector<int>> score_total;  //  complete scoring history of the current game
    QVector<int> totalscores;  //  all scores in one vector
    QVector<int> numberofdartsarray;
    QVector<int> checkoutattemptarray;
    QVector<int> checkoutarray;
    QVector<QVector<QString>> mThrownDarts;
};

#endif // PLAYERCLASS_H
