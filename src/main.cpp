#include <iostream>
#include "dataloader.h"
#include "title.h"
#include <vector>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DataLoader loader;
	const auto titles = loader.load();

	MainWindow window(titles);
	window.show();

	return app.exec();
}
