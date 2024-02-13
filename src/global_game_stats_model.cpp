#include "global_game_stats_model.h"

CGlobalGameStatsModel::CGlobalGameStatsModel(const CStatsWindow::SGlobalGameStatsData & iData, QObject * iParent)
  : QAbstractTableModel(iParent)
  , mData(iData)
  , mCol1Data({mData.Avg3Dart, mData.Avg1Dart})
{}

int CGlobalGameStatsModel::rowCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

int CGlobalGameStatsModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 4;
}

QVariant CGlobalGameStatsModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= 2 || iIndex.column() >= 4)
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return mCol0Labels.at(iIndex.row());
    }
    else if (iIndex.column() == 1)
    {
      return QString::number(mCol1Data.at(iIndex.row()), 'f', 2);
    }
    else if (iIndex.column() == 2)
    {
      return mCol2Labels.at(iIndex.row());
    }
    else if (iIndex.column() == 3 && iIndex.row() == 0)
    {
      QString checkoutRateStr = "--";
      if (mData.CheckoutAttempts > 0)
      {
        double checkoutRate = static_cast<double>(mData.CheckoutHits) / mData.CheckoutAttempts * 100;
        checkoutRateStr = QString::number(checkoutRate, 'f', 2) + " %";
      }
      return checkoutRateStr + "    (" + QString::number(mData.CheckoutHits) + " / " + QString::number(mData.CheckoutAttempts) + ")";
    }
    else
    {
      return QString::number(mData.HighestCheckout);
    }
  }
  else
  {
    return QVariant();
  }
}
