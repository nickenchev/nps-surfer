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

	DataLoader loader;
	loader.load(titles);

	titlesModel = std::make_unique<TitleTableModel>(titles);
	ui->titlesTableView->setModel(titlesModel.get());

	// handle title selection
	QItemSelectionModel *selectionModel = ui->titlesTableView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);
}

void MainWindow::downloadsComplete()
{
	dialog.getLabel()->setText(QString("Refresh successful."));
	dialog.getButton()->setEnabled(true);
}

void MainWindow::downloadError()
{
	dialog.getLabel()->setText(QString("Error downloading data files."));
	dialog.getButton()->setEnabled(true);
}

void MainWindow::showEvent(QShowEvent *event)
{
	// download files
	dialog.getLabel()->setText(QString("Refreshing data files"));
	dialog.getButton()->setText(QString("Please Wait"));
	dialog.open();

	downloader = std::make_unique<Downloader>();
	connect(downloader.get(), &Downloader::progressChanged, &dialog, &ProgressDialog::downloadProgress);
	connect(downloader.get(), &Downloader::downloadsComplete, this, &MainWindow::downloadsComplete);
	connect(downloader.get(), &Downloader::downloadError, this, &MainWindow::downloadError);

	QtConcurrent::run([this]() {
		downloader->start();
	}).then([this]() {
		dialog.getButton()->setText(QString("Close"));
	});
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
