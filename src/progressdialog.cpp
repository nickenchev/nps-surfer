#include <iostream>
#include "progressdialog.h"
#include "./ui_progressdialog.h"
#include "progresslistmodel.h"


ProgressDialog::ProgressDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProgressDialog)
{
	ui->setupUi(this);
	ui->progressBar->setMaximum(100);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);
	ui->closeButton->setEnabled(false);

	ui->itemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QPushButton *ProgressDialog::getButton()
{
	return ui->closeButton;
}

void ProgressDialog::setProgress(int value)
{
	ui->progressBar->setValue(value);
}

QAbstractItemModel *ProgressDialog::createItemsModel(std::vector<ProgressItem> &items)
{
	QAbstractItemModel *model = new ProgressListModel(items);
	ui->itemsTable->setModel(model);

	return model;
}
