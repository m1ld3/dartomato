#ifndef CLEGSCORESX01MODEL_H
#define CLEGSCORESX01MODEL_H

#include <QAbstractTableModel>

class CLegScoresX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegScoresX01Model(const QVector<uint32_t>& iLegScores, const QVector<QVector<QString>>& iLegDarts, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;
  void update(const QVector<uint32_t>& iLegScores, const QVector<QVector<QString>>& iLegDarts);

private:

  QVector<uint32_t> mLegScores;
  QVector<QVector<QString>> mLegDarts;
};

#endif  // CLEGSCORESX01MODEL_H
