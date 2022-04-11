#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>

#include "category.h"
#include "title.h"

struct TSVFile
{
	Category category;
	std::string path;
};

struct ColumnMap
{
	short titleId, region, name, pkgDirectLink, contentId, lastModificationDate, fileSize, sha256;
};

class DataLoader
{
public:
	std::vector<Title> load() const;
};

#endif /* DATALOADER_H */
