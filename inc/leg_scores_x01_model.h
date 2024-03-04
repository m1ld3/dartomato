#ifndef CLEGSCORESX01MODEL_H
#define CLEGSCORESX01MODEL_H

#include <QAbstractTableModel>

class CLegScoresX01Model : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegScoresX01Model(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts);

private:

  QVector<uint32_t> mLegScores;
  QVector<QVector<QString>> mLegDarts;
};

#endif  // CLEGSCORESX01MODEL_H
