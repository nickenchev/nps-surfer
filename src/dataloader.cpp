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

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	std::vector<char> *data = static_cast<std::vector<char> *>(userp);
	size_t writeSize = size * nmemb;

	for (size_t i = 0; i < writeSize; ++i)
	{
		char val = static_cast<char *>(buffer)[i];
		data->push_back(val);
	}

	return size * nmemb;
}

int xfer_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	double pct = (dltotal > 0) ? dlnow / static_cast<double>(dltotal) * 100 : 0;
	std::cout << static_cast<int>(pct) << "%, " << std::flush;
	return 0;
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

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://nopaystation.com/tsv/PSV_GAMES.tsv");

	std::vector<char> data(1024);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xfer_callback);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

	if (curl_easy_perform(curl) == CURLE_OK)
	{
		double cl = 0;
		if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl) == CURLE_OK)
		{
			printf("Size: %.0f\n", cl);
		}

		const std::string s(data.data());
		//std::cout << s << std::endl;
	}

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
