#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QAbstractItemModel>
#include <string>
#include "progressitem.h"

namespace Ui { class ProgressDialog; }

class ProgressDialog : public QDialog
{
	Q_OBJECT
    Ui::ProgressDialog *ui;

public:
	ProgressDialog(QWidget *parent);
	void setProgress(int value);
	QPushButton *getButton();
	QAbstractItemModel *createItemsModel(std::vector<ProgressItem> &items);
};


#endif /* PROGRESSDIALOG_H */
