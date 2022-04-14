#include <iostream>
#include <QItemSelectionModel>
#include <QTableView>
#include <QtConcurrent/QtConcurrent>
#include <memory>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dataloader.h"
#include "title.h"
#include "titletablemodel.h"
#include "downloader.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), dialog(this)
{
    ui->setupUi(this);

	// configure progress dialog
	connect(dialog.getButton(), &QPushButton::clicked, [this]() {
		dialog.close();
	});

	// download files
	dialog.getLabel()->setText(QString("Refreshing data files"));
	dialog.getButton()->setText(QString("Please Wait"));
	dialog.open();

	psvGames = std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_GAMES.tsv");
	connect(psvGames.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvGames.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvGames.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvDlcs = std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_DLCS.tsv");
	connect(psvDlcs.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvDlcs.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvDlcs.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvThemes = std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_THEMES.tsv");
	connect(psvThemes.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvThemes.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvThemes.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvUpdates = std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_UPDATES.tsv");
	connect(psvUpdates.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvUpdates.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvUpdates.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvDemos = std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_DEMOS.tsv");
	connect(psvDemos.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvDemos.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvDemos.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	// create a future watcher and ensure post-DL work happens on the main-thread
	connect(&futureWatcher, &QFutureWatcher<int>::finished, [this]() {
		dialog.getButton()->setText(QString("Close"));
		dialog.getButton()->setEnabled(true);

		titlesModel = std::make_unique<TitleTableModel>(titles);
		ui->titlesTableView->setModel(titlesModel.get());

		// handle title selection
		QItemSelectionModel *selectionModel = ui->titlesTableView->selectionModel();
		connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);
	});

	// start watching the download future
	futureWatcher.setFuture(
		QtConcurrent::run([this]() -> int {
			totalSize = 0;
			totalDownloaded = 0;
			totalSize += psvGames->fetchSize();
			totalSize += psvDlcs->fetchSize();
			totalSize += psvThemes->fetchSize();
			totalSize += psvUpdates->fetchSize();
			totalSize += psvDemos->fetchSize();

			dialog.getLabel()->setText("PS Vita Games");
			psvGames->start();
			dialog.getLabel()->setText("PS Vita DLC");
			psvDlcs->start();
			dialog.getLabel()->setText("PS Vita Themes");
			psvThemes->start();
			dialog.getLabel()->setText("PS Vita Updates");
			psvUpdates->start();
			dialog.getLabel()->setText("PS Vita Demos");
			psvDemos->start();

			DataLoader loader;
			loader.load(titles);

			return titles.size();
		}));
}

void MainWindow::downloadProgress(unsigned long downloaded, unsigned long total)
{
	this->downloaded = downloaded;
	int pct = (totalDownloaded + downloaded) / static_cast<double>(totalSize) * 100;
	dialog.setProgress(pct);
}

void MainWindow::downloadComplete()
{
	totalDownloaded += downloaded;
}

void MainWindow::downloadError()
{
	dialog.getLabel()->setText(QString("Error downloading data files."));
}

void MainWindow::showEvent(QShowEvent *event)
{
}

void MainWindow::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	size_t index = selected.indexes().begin()->row();
	const Title &title = titles[index];
	ui->titleLabel->setText(QString(title.name.c_str()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
