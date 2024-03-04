#ifndef CLEGSCORESCRICKETMODEL_H
#define CLEGSCORESCRICKETMODEL_H

#include <QAbstractTableModel>

class CLegScoresCricketModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  explicit CLegScoresCricketModel(QVector<QVector<QString>> iLegDarts, QObject * iParent = nullptr);
  int rowCount(const QModelIndex & iParent = QModelIndex()) const override;
  int columnCount(const QModelIndex & iParent = QModelIndex()) const override;
  QVariant data(const QModelIndex & iIndex, int iRole = Qt::DisplayRole) const override;
  void update(QVector<QVector<QString>> iLegDarts);

private:

  QVector<QVector<QString>> mLegDarts;
};

#endif  // CLEGSCORESCRICKETMODEL_H
