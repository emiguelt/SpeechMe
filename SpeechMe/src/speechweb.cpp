#include "inc/speechweb.h"
#include "ui_speechweb.h"
#include <QtWebKit/qwebview.h>
#include <qurl.h>

SpeechWeb::SpeechWeb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeechWeb)
{
    ui->setupUi(this);
    speechMe = (SpeechMe*)parent;
    Msrs::getInstance()->Attach(this);
    connect(this, SIGNAL(sentenceArrived(const char *)), 
    		this, SLOT(on_sentence_arrived(const char *)), Qt::BlockingQueuedConnection);
}

SpeechWeb::~SpeechWeb()
{
	Msrs::getInstance()->Detach(this);
    delete ui;
}

void SpeechWeb::UpdateSentence(Subject *subject){
	Msrs* msrs = (Msrs*)subject;
	emit sentenceArrived(msrs->getLastSentence());
}

void SpeechWeb::Update(Subject *subject){

}

void SpeechWeb::on_backButton_clicked(){
}

void SpeechWeb::on_speechButton_clicked(){
	if(!speechMe->isLiveDecoding()){
		speechMe->initLocalDecoding(true);
	}
}

void SpeechWeb::on_sentence_arrived(const char * sentence){
	QString url = "http://www.google.com/search?q=";
	url += sentence;
	ui->webView->load(QUrl(url));
}
