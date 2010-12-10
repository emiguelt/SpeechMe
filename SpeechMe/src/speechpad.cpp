#include "inc\speechpad.h"
#include "ui_speechpad.h"
#include <Observer.h>

SpeechPad::SpeechPad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeechPad)
{
    ui->setupUi(this);
    Msrs::getInstance()->Attach(this);
    speechMe = (SpeechMe*)parent;
    localDecoding = false;
    
    on_decoder_configured(speechMe->isDecoderConfigured());
    
}

SpeechPad::~SpeechPad()
{
	Msrs::getInstance()->Detach(this);
    delete ui;
}

void SpeechPad::Update(Subject* subject){
}

void SpeechPad::UpdateSentence(Subject* subject){
	QString sent(Msrs::getInstance()->getLastSentence());
	ui->textSP->appendPlainText(sent);
	ui->textSP->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SpeechPad::on_decoder_configured(bool status){
  ui->isolatedButton->setEnabled(status);
  ui->contButton->setEnabled(status);
}

void SpeechPad::on_isolatedButton_clicked(){
  decode(true);
}

void SpeechPad::on_contButton_clicked(){
  decode(false);
  updateContButton();
}

void SpeechPad::updateContButton(){
  if(speechMe->isLiveDecoding()){
    ui->contButton->setText(tr("Stop decoding"));
  }else{
    ui->contButton->setText(tr("Continuous"));
  }
}

void SpeechPad::decode(bool isolated){
	if(!speechMe->isLiveDecoding()){
		  speechMe->initLocalDecoding(isolated);
		  localDecoding = true;
	  }else if(localDecoding){
		  speechMe->stopLiveDecoding();
		  localDecoding = false;
	  }
}


