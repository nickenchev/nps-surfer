#include <curl/curl.h>
#include <iostream>
#include "downloader.h"

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
	Downloader *downloader = static_cast<Downloader *>(clientp);
	emit downloader->progressChanged(dlnow, dltotal);

	double pct = (dltotal > 0) ? dlnow / static_cast<double>(dltotal) * 100 : 0;
	return 0;
}

Downloader::Downloader()
{
}

void Downloader::start()
{
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://nopaystation.com/tsv/PSV_GAMES.tsv");

	std::vector<char> data(1024);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xfer_callback);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

	if (curl_easy_perform(curl) == CURLE_OK)
	{
		double cl = 0;
		if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl) == CURLE_OK)
		{
			emit downloadsComplete();
		}
		else
		{
			emit downloadError();
		}
		const std::string s(data.data());
	}
}
