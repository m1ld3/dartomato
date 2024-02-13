#include "leg_scores_model.h"

CLegScoresModel::CLegScoresModel(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts, QObject * iParent)
  : mLegScores(iLegScores)
  , mLegDarts(iLegDarts)
{}

int CLegScoresModel::rowCount(const QModelIndex & iParent) const
{
  if (iParent.isValid()) return 0;
  return mLegScores.count();
}

int CLegScoresModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 3;
}

QVariant CLegScoresModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mLegScores.size() || iIndex.row() >= mLegDarts.size() || iIndex.column() >= 3)
  {
    return QVariant();
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return QString::number(iIndex.row() + 1) + ":";
    }
    else if (iIndex.column() == 1)
    {
      return QString::number(mLegScores.at(iIndex.row()));
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

void CLegScoresModel::update(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts)
{
  mLegScores = iLegScores;
  mLegDarts = iLegDarts;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
  emit layoutChanged();
}
