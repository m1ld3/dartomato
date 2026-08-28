#include "leg_scores_x01_model.h"

#include <utility>

CLegScoresX01Model::CLegScoresX01Model(const QVector<uint32_t>& iLegScores, const QVector<QVector<QString>>& iLegDarts, QObject *)
  : mLegScores(iLegScores)
  , mLegDarts(iLegDarts)
{}

int CLegScoresX01Model::rowCount(const QModelIndex & iParent) const
{
  if (iParent.isValid()) return 0;
  return static_cast<int>(mLegScores.count());
}

int CLegScoresX01Model::columnCount(const QModelIndex & iParent) const
{
  Q_UNUSED(iParent);
  return 3;
}

QVariant CLegScoresX01Model::data(const QModelIndex & iIndex, const int iRole) const
{
  if (!iIndex.isValid() || iIndex.row() >= mLegScores.size() || iIndex.row() >= mLegDarts.size() || iIndex.column() >= columnCount({}))
  {
    return {};
  }

  if (iRole == Qt::DisplayRole)
  {
    if (iIndex.column() == 0)
    {
      return QString::number(iIndex.row() + 1) + ":";
    }
    if (iIndex.column() == 1)
    {
      return QString::number(mLegScores.at(iIndex.row()));
    }
    QString row;
    for (const auto & dart : mLegDarts.at(iIndex.row()))
    {
      QChar type = dart[0];
      int val = dart.mid(1).toInt();
      if (type == 't') val = val / 3;
      else if (type == 'd') val = val / 2;
      row.append(type.toUpper() + QString::number(val) + "  ");
    }
    return row;
  }
  return {};
}

void CLegScoresX01Model::update(const QVector<uint32_t>& iLegScores, const QVector<QVector<QString>>& iLegDarts)
{
  mLegScores = iLegScores;
  mLegDarts = iLegDarts;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount({}) - 1, 0));
  emit layoutChanged();
}
