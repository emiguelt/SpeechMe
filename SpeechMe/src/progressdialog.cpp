#include "inc/progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent,  QString title) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
void ProgressDialog::updateProgressStatus(QString text, int progress){
    ui->task->setText(text);
    if(progress>=0 && progress <=100){
        ui->progress->setValue(progress);
    }
}
