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
#include "progresslistmodel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	psvGames = std::make_unique<WorkItem>("PS Vita Games", "https://nopaystation.com/tsv/PSV_GAMES.tsv");
	connect(psvGames->downloader.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvGames->downloader.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvGames->downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvDlcs = std::make_unique<WorkItem>("PS Vita DLCs", "https://nopaystation.com/tsv/PSV_DLCS.tsv");
	connect(psvDlcs->downloader.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvDlcs->downloader.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvDlcs->downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvThemes = std::make_unique<WorkItem>("PS Vita Themes", "https://nopaystation.com/tsv/PSV_THEMES.tsv");
	connect(psvThemes->downloader.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvThemes->downloader.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvThemes->downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvUpdates = std::make_unique<WorkItem>("PS Vita Updates", "https://nopaystation.com/tsv/PSV_UPDATES.tsv");
	connect(psvUpdates->downloader.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvUpdates->downloader.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvUpdates->downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	psvDemos = std::make_unique<WorkItem>("PS Vita Demos", "https://nopaystation.com/tsv/PSV_DEMOS.tsv");
	connect(psvDemos->downloader.get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
	connect(psvDemos->downloader.get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
	connect(psvDemos->downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	// create and configure progress dialog
	dialog = std::make_unique<ProgressDialog>(this);
	connect(dialog->getButton(), &QPushButton::clicked, [this]() {
		dialog->close();
	});

	// download files
	dialog->getButton()->setText(QString("Please Wait"));
	dialog->open();


	// create a future watcher and ensure post-DL work happens on the main-thread
	connect(&futureWatcher, &QFutureWatcher<int>::finished, [this]() {
		dialog->getButton()->setText(QString("Close"));
		dialog->getButton()->setEnabled(true);

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
			totalSize += psvGames->downloader->fetchSize();
			totalSize += psvDlcs->downloader->fetchSize();
			totalSize += psvThemes->downloader->fetchSize();
			totalSize += psvUpdates->downloader->fetchSize();
			totalSize += psvDemos->downloader->fetchSize();

			psvGames->downloader->start();
			psvDlcs->downloader->start();
			psvThemes->downloader->start();
			psvUpdates->downloader->start();
			psvDemos->downloader->start();

			DataLoader loader;
			loader.load(titles);

			return titles.size();
		}));
}

void MainWindow::downloadProgress(unsigned long downloaded, unsigned long total)
{
	this->downloaded = downloaded;
	int pct = (totalDownloaded + downloaded) / static_cast<double>(totalSize) * 100;
	dialog->setProgress(pct);
}

void MainWindow::downloadComplete()
{
	totalDownloaded += downloaded;
}

void MainWindow::downloadError()
{
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
