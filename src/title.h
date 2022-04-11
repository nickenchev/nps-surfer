#ifndef TITLE_H
#define TITLE_H

#include <string>
#include <fstream>
#include "category.h"

struct Title
{
	Category category;
	std::string titleId, region, name, pkgDirectLink, contentId, lastModifyDate, sha256;
	unsigned long fileSize;

	Title()
	{
		fileSize = 0;
	}
};

std::istream &operator>>(std::istream &inputStream, Title &title);

#endif /* TITLE_H */
