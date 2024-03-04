#include "leg_scores_cricket_model.h"

CLegScoresCricketModel::CLegScoresCricketModel(QVector<QVector<QString>> iLegDarts, QObject * iParent)
  : mLegDarts(iLegDarts)
{}

int CLegScoresCricketModel::rowCount(const QModelIndex & iParent) const
{
  if (iParent.isValid()) return 0;
  return mLegDarts.count();
}

int CLegScoresCricketModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegScoresCricketModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mLegDarts.size() || iIndex.column() >= columnCount())
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return QString::number(iIndex.row() + 1) + ":";
    }
    else
    {
      QString row;
      for (const auto & dart : mLegDarts.at(iIndex.row()))
      {
        row.append(QString("%1  ").arg(dart.toUpper()));
      }
      return row;
    }
  }
  return QVariant();
}

void CLegScoresCricketModel::update(QVector<QVector<QString>> iLegDarts)
{
  mLegDarts = iLegDarts;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
  emit layoutChanged();
}
