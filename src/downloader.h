#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <string>
#include <vector>
#include <curl/curl.h>

struct DownloadItem
{
};

class Downloader : public QObject
{
	Q_OBJECT
	CURL *curl;
	const std::string url;
	std::vector<char> data;
	
public:
    Downloader(std::string url);
    virtual ~Downloader();

	void start();
	unsigned long fetchSize();

signals:
	void progressChanged(unsigned long downloaded, unsigned long total);
	void downloadComplete();
	void downloadError();
};


#endif /* DOWNLOADER_H */
