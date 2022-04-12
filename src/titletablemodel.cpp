#include <endian.h>
#include <qvariant.h>
#include <array>
#include <string>
#include "titletablemodel.h"
#include "category.h"

static auto columns = std::array<std::string, 5> {
	"Name", "Title ID", "Region", "Size", "Type"
};

QVariant TitleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant result;
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		result = QString(columns[section].c_str());
	}
	return result;
}

int TitleTableModel::rowCount(const QModelIndex &parent) const
{
	return titles.size();
}

int TitleTableModel::columnCount(const QModelIndex &parent) const
{
	return columns.size();
}

QVariant TitleTableModel::data(const QModelIndex &index, int role) const
{
	QVariant result;
	if (role == Qt::DisplayRole)
	{
		const Title &title = titles[index.row()];
		switch (index.column())
		{
			case 0:
			{
				result = QString(title.name.c_str());
				break;
			}
			case 1:
			{
				result = QString(title.titleId.c_str());
				break;
			}
			case 2:
			{
				result = QString(title.region.c_str());
				break;
			}
			case 3:
			{
				result = QString("TODO");
				break;
			}
			case 4:
			{
				switch(title.category)
				{
					case Category::PS3Game:
					{
						result = QString("PS3 Game");
						break;
					}
					case Category::PSMGame:
					{
						result = QString("PSM Game");
						break;
					}
					case Category::PSPGame:
					{
						result = QString("PSP Game");
						break;
					}
					case Category::PSVitaGame:
					{
						result = QString("PS Vita Game");
						break;
					}
					case Category::PSXGame:
					{
						result = QString("PSX Game");
						break;
					}
				}
				break;
			}
		}
	}
	return result;
}
