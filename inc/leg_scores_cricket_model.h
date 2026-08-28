#ifndef CLEGSCORESCRICKETMODEL_H
#define CLEGSCORESCRICKETMODEL_H

#include <QAbstractTableModel>

class CLegScoresCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegScoresCricketModel(const QVector<QVector<QString>>& iLegDarts, QObject * iParent = nullptr);
  [[nodiscard]] int rowCount(const QModelIndex & iParent) const override;
  [[nodiscard]] int columnCount(const QModelIndex & iParent) const override;
  [[nodiscard]] QVariant data(const QModelIndex & iIndex, int iRole) const override;
  void update(const QVector<QVector<QString>>& iLegDarts);

private:

  QVector<QVector<QString>> mLegDarts;
};

#endif  // CLEGSCORESCRICKETMODEL_H
