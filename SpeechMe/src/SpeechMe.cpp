/*
============================================================================
 Name		: SpeechMe.cpp
 Author	  : Edwin Miguel Triana Gómez
 Copyright   : 
 Description : Class Implementation
============================================================================
*/

#include "SpeechMe.h"
#include <errno.h>
#include <string>
#include "speechpad.h"
#include "speechweb.h"
#include "ConfigUi.h"
#include "decoderthread.h"


SpeechMe::SpeechMe(QMainWindow *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	msrs = Msrs::getInstance();
	msrs->Attach(this);
	//initExtraUi();
	conf = new Configuration();

	speechRemote = new SpeechRemote(this);
	
	createConnections();
	
	decoderConfigured = false;

	on_configAction_triggered();

}

SpeechMe::~SpeechMe()
{
	msrs->Detach(this);
	//delete msrs;
}

void SpeechMe::createConnections(){
  connect(ui.actionConfig, SIGNAL(triggered()),this , SLOT(on_configAction_triggered()));
  connect(ui.actionSpeechPad, SIGNAL(triggered()),this , SLOT(on_testAction_triggered()));
  connect(ui.actionSpeechWeb, SIGNAL(triggered()), this, SLOT(on_webAction_triggered()));
  connect(this, SIGNAL(newStatusMessage(const QString &)), ui.statusbar, SLOT(showMessage(const QString &)));
  connect(speechRemote, SIGNAL(registerClient(RemoteClient*)), this, SLOT(on_registerClient(RemoteClient*)));
  connect(this, SIGNAL(decoderStatusUpdated(int)), this, SLOT(on_updated_decoder_status(int)));
}

void SpeechMe::Update(Subject* subject){
	emit decoderStatusUpdated(msrs->getStatus());
}

void SpeechMe::UpdateSentence(Subject* subject){
}

void SpeechMe::on_updated_decoder_status(int statusNumber){
	QString* status;
	switch(msrs->getStatus()){
		case Msrs::CONFIGURED:
			status = new QString(tr("Configured"));
		    break;
		case Msrs::INITIALIZED:
			status = new QString(tr("Initialized"));
			decoderConfigured = true;
		    break;
		case Msrs::READY:
			status = new QString(tr("Ready"));
			break;
		case Msrs::LISTENING:
			status = new QString(tr("Listening"));
			break;
		case Msrs::PROCESSING:
			status = new QString(tr("Processing..."));
			break;
		case Msrs::STOPPED:
			status = new QString(tr("Stopped - Decoder free"));
			break;
		case Msrs::FAIL:
			status = new QString(tr(strerror(errno)));
			break;
		default:
			status = new QString(tr("State unknown - "));
			status->append(QString::number(msrs->getStatus()));
			break;
	}
	emit newStatusMessage(*status);
}


void SpeechMe::on_configAction_triggered(){
	configui = new ConfigUi(this, conf);
	setCentralWidget(configui);
}

void SpeechMe::on_testAction_triggered(){
	speechPad = new SpeechPad(this);
	setCentralWidget(speechPad);
}

void SpeechMe::on_webAction_triggered(){
	speechWeb = new SpeechWeb(this);
	setCentralWidget(speechWeb);
}



void SpeechMe::on_serverButton_clicked(){
	if(speechRemote->isRunning()){
		speechRemote->stopServer();
		configui->setServerRunning(FALSE);
	}else{
		configui->setServerRunning(speechRemote->startServer(conf->getServerPort()));
	}
}

bool SpeechMe::isSpeechRemoteRunning(){
	return speechRemote->isRunning();
}

void SpeechMe::initDecoding(RemoteClient * client, bool opt)
{
    emit newStatusMessage(tr("Init decoding..."));
	if(!msrs->isLiveDecoding()){
		msrs->setTempClient(client);
		msrs->startLiveDecoding(opt);
	}else{
		client->decoderBusy();
		emit newStatusMessage(tr("Decoder busy"));
	}
}

void SpeechMe::on_newrequest_arrived(RemoteClient* client, int request){
	switch(request){
		case CMD_ISOLATED_RECOGNITION:
			initDecoding(client, TRUE);
			break;
		case CMD_CONTINUOUS_RECOGNITION:
			initDecoding(client, FALSE);
			break;
		case CMD_REGISTER_CLIENT:
			break;
		case CMD_REMOVE_CLIENT:
			speechRemote->removeClient(client);
			break;
		default:
			break;
	}
}

void SpeechMe::on_registerClient(RemoteClient* client){
	connect(client, SIGNAL(newRequestArrived(RemoteClient*, int)), this, SLOT(on_newrequest_arrived(RemoteClient*, int)));
	client->registerClient();
}

void SpeechMe::initLocalDecoding(bool opt){
  DecoderThread* decThread = new DecoderThread();
	emit newStatusMessage(tr("Local decoding..."));
	if(!msrs->isLiveDecoding()){
      decThread->setIsolated(opt);
      decThread->start();
//		msrs->startLiveDecoding(opt);
	}
}

bool SpeechMe::isLiveDecoding(){
	return msrs->isLiveDecoding();
}

void SpeechMe::stopLiveDecoding(){
	msrs->stopLiveDecoding();
}

bool SpeechMe::isDecoderConfigured(){
	return decoderConfigured;
}

void SpeechMe::on_decoder_configured(bool status){
	decoderConfigured = status;
}

void SpeechMe::on_hideAction_triggered(){
 this->showMinimized();
}
