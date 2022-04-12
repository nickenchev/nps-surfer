#ifndef TITLETABLEMODEL_H
#define TITLETABLEMODEL_H

#include <vector>
#include <QAbstractTableModel>
#include "title.h"

class TitleTableModel : public QAbstractTableModel
{
	const std::vector<Title> &titles;

public:
	TitleTableModel(const std::vector<Title> &titles, QObject *parent = nullptr) : titles(titles), QAbstractTableModel(parent)
	{
	}
	
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif /* TITLETABLEMODEL_H */
