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
    
    on_decoder_configured(speechMe->isDecoderInitialized());
    connect(this, SIGNAL(newSentenceReady(const QString*)), this, SLOT(on_new_sentence_ready(const QString*)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(updateDecoderStatus(bool)), this, SLOT(on_update_decoder_status(bool)), Qt::BlockingQueuedConnection);
}

SpeechPad::~SpeechPad()
{
	Msrs::getInstance()->Detach(this);
    //delete ui;
}

void SpeechPad::Update(Subject* subject){
	switch(Msrs::getInstance()->getStatus()){
		case Msrs::STOPPED:
			emit updateDecoderStatus(TRUE);
			break;
		case Msrs::FAIL:
			emit updateDecoderStatus(TRUE);
			break;	
	}
}

void SpeechPad::on_update_decoder_status(bool status){
	enableButtons(status);
}

void SpeechPad::UpdateSentence(Subject* subject){
	const QString sent(Msrs::getInstance()->getLastSentence());
	emit newSentenceReady(&sent);
}

void SpeechPad::on_new_sentence_ready(const QString* stce){
	ui->textSP->appendPlainText(*stce);
	ui->textSP->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SpeechPad::on_decoder_configured(bool status){
  enableButtons(status);
}

void SpeechPad::on_isolatedButton_clicked(){
	enableButtons(FALSE);
	decode(true);
}

void SpeechPad::on_contButton_clicked(){
	updateContButton(!speechMe->isLiveDecoding());
	decode(false);
}

void SpeechPad::updateContButton(bool opt){
  if(opt){
    ui->contButton->setText(tr("Stop decoding"));
  }else{
    ui->contButton->setText(tr("Continuous"));
  }
  ui->isolatedButton->setEnabled(!opt);
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

void SpeechPad::enableButtons(bool opt){
	ui->isolatedButton->setEnabled(opt);
	ui->contButton->setEnabled(opt);
}

