#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <memory>
#include <QMainWindow>
#include <QItemSelection>
#include <QFutureWatcher>
#include "title.h"
#include "titletablemodel.h"
#include "progressdialog.h"
#include "downloader.h"
#include "progressitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;

	QFutureWatcher<int> futureWatcher;
	std::vector<Title> titles;
	std::unique_ptr<TitleTableModel> titlesModel;
	std::unique_ptr<ProgressDialog> dialog;
	unsigned long totalSize, downloaded, totalDownloaded;
	std::vector<ProgressItem> progressItems;
	std::vector<std::unique_ptr<Downloader>> downloaders;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void showEvent(QShowEvent *event) override;
	void downloadProgress(unsigned long downloaded, unsigned long total);
	void downloadComplete();
	void downloadError();
};
#endif // MAINWINDOW_H
