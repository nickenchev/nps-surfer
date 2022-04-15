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
	ui->titlesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	progressItems = std::vector<ProgressItem> {
		ProgressItem("PS Vita Games"),
		ProgressItem("PS Vita DLCs"),
		ProgressItem("PS Vita Themes"),
		ProgressItem("PS Vita Updates"),
		ProgressItem("PS Vita Demos")
	};

	downloaders.push_back(std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_GAMES.tsv"));
	downloaders.push_back(std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_DLCS.tsv"));
	downloaders.push_back(std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_THEMES.tsv"));
	downloaders.push_back(std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_UPDATES.tsv"));
	downloaders.push_back(std::make_unique<Downloader>("https://nopaystation.com/tsv/PSV_DEMOS.tsv"));

	for (int i = 0; i < downloaders.size(); ++i)
	{
		connect(downloaders[i].get(), &Downloader::progressChanged, this, &MainWindow::downloadProgress);
		connect(downloaders[i].get(), &Downloader::downloadComplete, this, &MainWindow::downloadComplete);
		connect(downloaders[i].get(), &Downloader::downloadError, this, &MainWindow::downloadError);
	}

	// create and configure progress dialog
	dialog = std::make_unique<ProgressDialog>(this);
	connect(dialog->getButton(), &QPushButton::clicked, [this]() {
		dialog->close();
	});

	// download files
	QAbstractItemModel *model = dialog->createItemsModel(progressItems);
	dialog->getButton()->setText(QString("Please Wait"));
	dialog->open();

	// create a future watcher and ensure post-DL work happens on the main-thread
	connect(&futureWatcher, &QFutureWatcher<int>::finished, [this]() {
		dialog->getButton()->setText(QString("Close"));
		dialog->getButton()->setEnabled(true);

		//titlesModel = std::make_unique<TitleTableModel>(titles);
		ui->titlesTableView->setModel(titlesModel.get());

		// handle title selection
		QItemSelectionModel *selectionModel = ui->titlesTableView->selectionModel();
		connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);
	});

	// start watching the download future
	futureWatcher.setFuture(
		QtConcurrent::run([this, model]() -> int {
			totalSize = 0;
			totalDownloaded = 0;

			// get total download size for all files
			for (const auto &downloader : downloaders)
			{
				totalSize += downloader->fetchSize();
			}

			// perform all downloads
			int row = 0;
			for (const auto &downloader : downloaders)
			{
				downloader->start();
				progressItems[row].state = ProgressState::Success;
				QModelIndex index = model->index(row++, 1);
				emit model->dataChanged(index, index);
			}

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
