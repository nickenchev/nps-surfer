#ifndef PROGRESSLISTMODEL_H
#define PROGRESSLISTMODEL_H

#include <QAbstractTableModel>

class ProgressListModel : public QAbstractTableModel
{
public:
    ProgressListModel();

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};


#endif /* PROGRESSLISTMODEL_H */
