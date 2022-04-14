#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <curl/curl.h>
#include "dataloader.h"
#include "title.h"

using namespace std;

// trim from start (in place)
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}).base(), s.end());
}

void DataLoader::load(vector<Title> &allTitles) const
{
	array<TSVFile, 5> tsvFiles = {
		TSVFile { Category::PS3Game, "/home/nenchev/Downloads/PS3_GAMES.tsv" },
		TSVFile { Category::PSMGame, "/home/nenchev/Downloads/PSM_GAMES.tsv" },
		TSVFile { Category::PSPGame, "/home/nenchev/Downloads/PSP_GAMES.tsv" },
		TSVFile { Category::PSVitaGame, "/home/nenchev/Downloads/PSV_GAMES.tsv" },
		TSVFile { Category::PSXGame, "/home/nenchev/Downloads/PSX_GAMES.tsv" }
	};

	for (TSVFile &fileInfo : tsvFiles)
	{
		ifstream tsvFile(fileInfo.path);
	
		if (tsvFile)
		{
			// parse the headings
			std::vector<string> headings;
			string headingsLine;
			getline(tsvFile, headingsLine);

			// map file columns
			ColumnMap columnMap{-1};
			stringstream hs(headingsLine);
			string heading;
			int columnIndex = 0;
			while (getline(hs, heading, '\t'))
			{
				ltrim(heading);
				rtrim(heading);

				if (heading == "Title ID")
				{
					columnMap.titleId = columnIndex;
				}
				else if (heading == "Region")
				{
					columnMap.region = columnIndex;
				}
				else if (heading == "Name")
				{
					columnMap.name = columnIndex;
				}
				else if (heading == "PKG direct link")
				{
					columnMap.pkgDirectLink = columnIndex;
				}
				else if (heading == "Content ID")
				{
					columnMap.contentId = columnIndex;
				}
				else if (heading == "Last Modification Date")
				{
					columnMap.lastModificationDate = columnIndex;
				}
				else if (heading == "File Size")
				{
					columnMap.fileSize = columnIndex;
				}
				else if (heading == "SHA256")
				{
					columnMap.sha256 = columnIndex;
				}
				columnIndex++;
			}

			// read through each line and parse
			while (tsvFile.good())
			{
				string line;
				getline(tsvFile, line);
				rtrim(line);
				stringstream iss(line);

				Title title;
				title.category = fileInfo.category;

				string value;
				short columnIndex = 0;
				while (getline(iss, value, '\t'))
				{
					if (columnIndex == columnMap.titleId)
					{
						title.titleId = value;
					}
					else if (columnIndex == columnMap.region)
					{
						title.region = value;
					}
					else if (columnIndex == columnMap.name)
					{
						title.name = value;
					}
					else if (columnIndex == columnMap.pkgDirectLink)
					{
						title.pkgDirectLink = value;
					}
					else if (columnIndex == columnMap.contentId)
					{
						title.contentId = value;
					}
					else if (columnIndex == columnMap.lastModificationDate)
					{
						title.lastModifyDate = value;
					}
					else if (columnIndex == columnMap.fileSize)
					{
						if (value.length())
						{
							title.fileSize = stoul(value);
						}
					}
					else if (columnIndex == columnMap.sha256)
					{
						title.sha256 = value;
					}
					columnIndex++;
				}

				allTitles.push_back(title);
			}
			tsvFile.close();
		}
		else
		{
			throw std::runtime_error("Unable to open file");
		}
	}
}
