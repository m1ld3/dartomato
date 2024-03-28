#ifndef CALLTIMEPLAYERSTATSMODEL_H
#define CALLTIMEPLAYERSTATSMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CAlltimePlayerStatsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CAlltimePlayerStatsModel(CGameDataHandler & iGameDataHandler, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update_selected_players_x01(const QVector<QString> iSelectedPlayers);
  void update_selected_players_cricket(const QVector<QString> iSelectedPlayers);
  void update_selected_game(bool iIsCricket) { mIsCricket = iIsCricket; }

private:

  struct SPlayerStats
  {
    QString PlayerName = "";
    uint32_t GamesPlayed = 0;
    uint32_t GamesWon = 0;
    double WinRate = 0.0;
    uint32_t LegsPlayed = 0;
    uint32_t LegsWon = 0;
    double LegWinRate = 0.0;
    uint32_t ThrownDarts = 0;
    double Avg3Dart = 0.0;
    double Avg1Dart = 0.0;
    double First9Avg = 0.0;
    double BestLegAvg = 0.0;
    uint32_t BestWonLegDartCount = 0;
    uint32_t WorstWonLegDartCount = 0;
    double CheckoutRate = 0.0;
    uint32_t CheckoutHits = 0;
    uint32_t CheckoutAttempts = 0;
    uint32_t HighestCheckout = 0;
    double DartsPerLegAvg = 0.0;
    uint32_t TotalHits = 0;
    double HitsPerRound = 0.0;
  };

  QVector<QString> mSelectedPlayers;
  bool mIsCricket = false;
  CGameDataHandler & mGameDataHandler;
  QVector<CGameDataHandler::SStatsData> mGameStats = {};
  QVector<SPlayerStats> mSelectedStats = {};

  QStringList mStatLabelsX01 = {   "Games Played"
                                 , "Games Won"
                                 , "Win Rate"
                                 , "Legs Played"
                                 , "Legs Won"
                                 , "Leg Win Rate"
                                 , "Thrown Darts"
                                 , "3-Dart-Average"
                                 , "1-Dart-Average"
                                 , "First-9 Average"
                                 , "Best Leg Average"
                                 , "Best Won Leg"
                                 , "Worst Won Leg"
                                 , "Checkout Rate"
                                 , "Highest Checkout"
                                 , "Darts-Per-Leg Average"
                               };
  QStringList mStatLabelsCricket = {   "Games Played"
                                     , "Games Won"
                                     , "Win Rate"
                                     , "Legs Played"
                                     , "Legs Won"
                                     , "Leg Win Rate"
                                     , "Thrown Darts"
                                     , "Hits Per Round"
                                     , "Total Hits"
                                     , "Best Leg Average"
                                     , "Best Won Leg"
                                     , "Worst Won Leg"
                                     , "Darts-Per-Leg Average"
                                   };  
};

#endif  // CALLTIMEPLAYERSTATSMODEL_H
