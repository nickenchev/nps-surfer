#ifndef PROGRESSLISTMODEL_H
#define PROGRESSLISTMODEL_H

#include <vector>
#include <QAbstractTableModel>
#include "progressitem.h"

class ProgressListModel : public QAbstractTableModel
{
	std::vector<ProgressItem> items;
public:
    ProgressListModel(const std::vector<ProgressItem> &items);

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};


#endif /* PROGRESSLISTMODEL_H */
