#ifndef CALLTIMEPLAYERSTATSMODEL_H
#define CALLTIMEPLAYERSTATSMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CAlltimePlayerStatsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CAlltimePlayerStatsModel(const QVector<CGameDataHandler::SGameData> & iGameData, const QVector<QString> iSelectedPlayers, bool iIsCricket, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update_selected_players(const QVector<QString> iSelectedPlayers) { mSelectedPlayers = iSelectedPlayers; };
  void update_selected_game(bool iIsCricket) { mIsCricket = iIsCricket; }

private:

  void prepare_data();

  const QVector<CGameDataHandler::SGameData> & mGameData;
  QVector<QString> mSelectedPlayers;
  bool mIsCricket = false;
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
