#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

class Downloader : public QObject
{
	Q_OBJECT
	
public:
    Downloader();
    ~Downloader() { };

	void start();

signals:
	void progressChanged(unsigned long downloaded, unsigned long total);
	void downloadsComplete();
	void downloadError();
};


#endif /* DOWNLOADER_H */
