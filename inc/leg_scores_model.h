#ifndef CLEGHISTORYLISTMODEL_H
#define CLEGHISTORYLISTMODEL_H

#include <QAbstractTableModel>

class CLegScoresModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegScoresModel(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(QVector<uint32_t> iLegScores, QVector<QVector<QString>> iLegDarts);

private:

  QVector<uint32_t> mLegScores;
  QVector<QVector<QString>> mLegDarts;
};

#endif  // CLEGHISTORYLISTMODEL_H
