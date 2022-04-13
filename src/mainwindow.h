#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <memory>
#include <QMainWindow>
#include <QItemSelection>
#include "title.h"
#include "titletablemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;

	std::vector<Title> titles;
	std::unique_ptr<TitleTableModel> titlesModel;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};
#endif // MAINWINDOW_H
