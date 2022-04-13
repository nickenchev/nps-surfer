#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui { class ProgressDialog; }

class ProgressDialog : public QDialog
{
	Q_OBJECT
    Ui::ProgressDialog *ui;

public:
	ProgressDialog(QWidget *parent);
    ~ProgressDialog() {}
};


#endif /* PROGRESSDIALOG_H */
