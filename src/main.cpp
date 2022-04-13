#include <iostream>
#include <vector>
#include <QApplication>
#include <curl/curl.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	curl_global_init(CURL_GLOBAL_ALL);
	int result = app.exec();
	curl_global_cleanup();

	return result;
}
