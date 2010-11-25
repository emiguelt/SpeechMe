#include "inc\speechpad.h"
#include "ui_speechpad.h"
#include <Observer.h>

SpeechPad::SpeechPad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeechPad)
{
    ui->setupUi(this);
}

SpeechPad::~SpeechPad()
{
    delete ui;
}

void SpeechPad::Update(Subject* subject){
}

void SpeechPad::UpdateSentence(Subject* subject){
	QString sent(msrs->getLastSentence());
	ui->textSP->appendPlainText(sent);
	ui->textSP->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SpeechPad::setMsrs(Msrs* msrs){
	this->msrs=msrs;
	msrs->Attach(this);
}

void SpeechPad::on_decoder_configured(bool status){
  ui->isolatedButton->setEnabled(status);
  ui->contButton->setEnabled(status);
}

void SpeechPad::on_isolatedButton_clicked(){
  msrs->startLiveDecoding(true);
}

void SpeechPad::on_contButton_clicked(){
  if(msrs->isLiveDecoding()){
    msrs->stopLiveDecoding();
    updateContButton();
  }else{
    msrs->startLiveDecoding(false);
    updateContButton();
  }
}

void SpeechPad::updateContButton(){
  if(msrs->isLiveDecoding()){
    ui->contButton->setText(tr("Stop decoding"));
  }else{
    ui->contButton->setText(tr("Continuous"));
  }
}


