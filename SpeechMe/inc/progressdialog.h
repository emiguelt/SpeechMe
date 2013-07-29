#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
    class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0, QString title=tr(""));
    ~ProgressDialog();
    void updateProgressStatus(QString text, int progress);

private:
    Ui::ProgressDialog *ui;

signals:
    void updateProgressValue(int);
};

#endif // PROGRESSDIALOG_H
