#include <iostream>
#include "progressdialog.h"
#include "./ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProgressDialog)
{
	ui->setupUi(this);
	ui->progressBar->setMaximum(100);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);
	ui->closeButton->setEnabled(false);
}

void ProgressDialog::downloadProgress(unsigned long downloaded, unsigned long total)
{
	int pct = downloaded / static_cast<double>(total) * 100;
	ui->progressBar->setValue(pct);
}

QLabel *ProgressDialog::getLabel()
{
	return ui->infoLabel;
}

QPushButton *ProgressDialog::getButton()
{
	return ui->closeButton;
}
