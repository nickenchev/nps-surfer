#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <string>

namespace Ui { class ProgressDialog; }

class ProgressDialog : public QDialog
{
	Q_OBJECT
    Ui::ProgressDialog *ui;

public:
	ProgressDialog(QWidget *parent);
	void setProgress(int value);
	QPushButton *getButton();
};


#endif /* PROGRESSDIALOG_H */
