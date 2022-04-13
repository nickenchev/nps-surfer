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
#include "progressdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	// download files
	// QFuture future = QtConcurrent::run()
	// future.then([]() {
	// });
	
	DataLoader loader;
	loader.load(titles);

	titlesModel = std::make_unique<TitleTableModel>(titles);
	ui->titlesTableView->setModel(titlesModel.get());

	// handle title selection
	QItemSelectionModel *selectionModel = ui->titlesTableView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);

	ProgressDialog dialog(this);
	dialog.exec();
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
