#include "progresslistmodel.h"

static auto columns = std::array<std::string, 2> {
	"Item", "Status"
};

ProgressListModel::ProgressListModel(const std::vector<ProgressItem> &items)
{
	this->items = items;
}

QVariant ProgressListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant result;
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		result = QString(columns[section].c_str());
	}
	return result;
}

int ProgressListModel::rowCount(const QModelIndex &parent) const
{
	return items.size();
}

int ProgressListModel::columnCount(const QModelIndex &parent) const
{
	return columns.size();
}

QVariant ProgressListModel::data(const QModelIndex &index, int role) const
{
	QVariant result;
	if (role == Qt::DisplayRole)
	{
		const ProgressItem &item = items[index.row()];
		switch (index.column())
		{
			case 0:
			{
				result = QString(item.title.c_str());
				break;
			}
			case 1:
			{
				result = QString("Waiting");
				break;
			}
		}
	}
	return result;
}
