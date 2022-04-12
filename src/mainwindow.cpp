#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "title.h"
#include "titletablemodel.h"

MainWindow::MainWindow(const std::vector<Title> &titles, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), titlesModel(titles)
{
    ui->setupUi(this);
	ui->titlesTableView->setModel(&titlesModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
