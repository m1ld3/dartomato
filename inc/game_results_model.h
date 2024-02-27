#ifndef CGAMERESULTMODEL_H
#define CGAMERESULTMODEL_H

#include <QAbstractTableModel>
#include "game_data_handler.h"

class CGameResultsModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CGameResultsModel(const CGameDataHandler::SGameData & iGameData, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant headerData(int iSection, Qt::Orientation iOrientation, int iRole) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;

private:

  void find_player_ranking_x01();
  void find_player_ranking_cricket();

  const CGameDataHandler::SGameData mGameData;
  QVector<QString> mColumnHeaders {};
  const QVector<QString> mRankLabels {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"};

  struct SPlayerScore
  {
    uint32_t Sets = 0;
    uint32_t Legs = 0;
    uint32_t PlayerId = 0;

    static bool compare(const SPlayerScore & iPlayer1, const SPlayerScore & iPlayer2)
    {
      if (iPlayer1.Sets == iPlayer2.Sets) return iPlayer1.Legs > iPlayer2.Legs;
      else return iPlayer1.Sets > iPlayer2.Sets;
    }
  };

  QVector<SPlayerScore> mScores = {};
  const bool mIsX01 = true;

};

#endif  // CGAMERESULTMODEL_H
