#include "leg_scores_cricket_model.h"

CLegScoresCricketModel::CLegScoresCricketModel(const QVector<QVector<QString>>& iLegDarts, QObject * iParent)
  : mLegDarts(iLegDarts)
{}

int CLegScoresCricketModel::rowCount(const QModelIndex & iParent) const
{
  if (iParent.isValid()) return 0;
  return static_cast<int>(mLegDarts.count());
}

int CLegScoresCricketModel::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 2;
}

QVariant CLegScoresCricketModel::data(const QModelIndex & iIndex, int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mLegDarts.size() || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return QString::number(iIndex.row() + 1) + ":";
    }
    QString row;
    for (const auto & dart : mLegDarts.at(iIndex.row()))
    {
      if (dart.mid(1).toInt() == 0) row.append("0      ");
      else row.append(QString("%1  ").arg(dart.toUpper()));
    }
    return row;
  }
  return {};
}

void CLegScoresCricketModel::update(const QVector<QVector<QString>>& iLegDarts)
{
  mLegDarts = iLegDarts;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount({}) - 1, 0));
  emit layoutChanged();
}
