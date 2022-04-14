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

Downloader::Downloader(std::string url) : url(url), data(1024)
{
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xfer_callback);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
}

Downloader::~Downloader()
{
	curl_easy_cleanup(curl);
}

unsigned long Downloader::fetchSize()
{
	curl_off_t size = 0;

	curl_easy_setopt(curl, CURLOPT_NOBODY, true);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, true);
	if (curl_easy_perform(curl) == CURLE_OK)
	{
		if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size) != CURLE_OK)
		{
			size = 0;
		}
	}
	else
	{
		emit downloadError();
	}
	return size;
}

void Downloader::start()
{
	data.clear();
	curl_easy_setopt(curl, CURLOPT_NOBODY, false);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
	if (curl_easy_perform(curl) == CURLE_OK)
	{
		emit downloadComplete();
		const std::string s(data.data());
	}
	else
	{
		emit downloadError();
	}
}
