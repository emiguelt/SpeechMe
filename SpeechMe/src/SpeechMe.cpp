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
#include <QMessageBox>


SpeechMe::SpeechMe(QMainWindow *parent)
: QMainWindow(parent){
	ui.setupUi(this);
	conf = new Configuration();
	decoderTh = DecoderThread::getInstance(this, conf);
	connect(decoderTh, SIGNAL(decoderthStarted()), this, SLOT(on_decoderth_started()));
	decoderTh->start();
}

void SpeechMe::on_decoderth_started(){
	Msrs::getInstance()->Attach(this);
	speechRemote = new SpeechRemote(this);
	createConnections();
	on_configAction_triggered();
}

SpeechMe::~SpeechMe()
	{
	Msrs::getInstance()->Detach(this);
	//delete msrs;
	}

void SpeechMe::createConnections(){
	connect(ui.actionConfig, SIGNAL(triggered()),this , SLOT(on_configAction_triggered()));
	connect(ui.actionSpeechPad, SIGNAL(triggered()),this , SLOT(on_testAction_triggered()));
	connect(ui.actionSpeechWeb, SIGNAL(triggered()), this, SLOT(on_webAction_triggered()));
	connect(this, SIGNAL(newStatusMessage(const QString &)), ui.statusbar, SLOT(showMessage(const QString &)));
	connect(speechRemote, SIGNAL(registerClient(RemoteClient*)), this, SLOT(on_registerClient(RemoteClient*)));
	connect(this, SIGNAL(decoderStatusUpdated(int)), this, SLOT(on_updated_decoder_status(int)), Qt::BlockingQueuedConnection);
}

void SpeechMe::Update(Subject* subject){
	Msrs* msrs = Msrs::getInstance();
	emit decoderStatusUpdated(msrs->getStatus());
}

void SpeechMe::UpdateSentence(Subject* subject){
}

void SpeechMe::on_updated_decoder_status(int statusNumber){
	QString* status;
	Msrs* msrs = Msrs::getInstance();
	switch(msrs->getStatus()){
		case Msrs::CONFIGURED:
			status = new QString(tr("Configured"));
			break;
		case Msrs::INITIALIZED:
			status = new QString(tr("Initialized"));
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
	updateMenu(ui.actionConfig);
}

void SpeechMe::on_testAction_triggered(){
	speechPad = new SpeechPad(this);
	setCentralWidget(speechPad);
	updateMenu(ui.actionSpeechPad);
}

void SpeechMe::on_webAction_triggered(){
	speechWeb = new SpeechWeb(this);
	setCentralWidget(speechWeb);
	updateMenu(ui.actionSpeechWeb);
}

void SpeechMe::updateMenu(QAction * menuOpt){
	ui.actionConfig->setEnabled(true);
	ui.actionSpeechPad->setEnabled(true);
	ui.actionSpeechWeb->setEnabled(true);
	menuOpt->setEnabled(false);
}

void SpeechMe::on_serverButton_clicked(){
	if(speechRemote->isRunning()){
		speechRemote->stopServer();
		configui->setServerRunning(FALSE);
	}else{
		if(speechRemote->startServer(conf->getServerPort())){
			configui->setServerRunning(TRUE);
		}else{
			QMessageBox::critical(this, "Speech server", 
					tr("Unable to start server - %1").arg(speechRemote->getServerError()));
			configui->setServerRunning(FALSE);
		}
	}
}

bool SpeechMe::isSpeechRemoteRunning(){
	return speechRemote->isRunning();
}

void SpeechMe::initDecoding(RemoteClient * client, bool opt)
	{
	emit newStatusMessage(tr("Init decoding..."));
	if(!decoderTh->getMsrsWorker()->isLiveDecoding()){
		decoderTh->getMsrsWorker()->setTempClient(client);
		emit startLiveDecoding(opt);
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
		default:
			break;
	}
}

void SpeechMe::on_registerClient(RemoteClient* client){
	connect(client, SIGNAL(newRequestArrived(RemoteClient*, int)), 
			this, SLOT(on_newrequest_arrived(RemoteClient*, int)));
	client->registerClient();
}

void SpeechMe::initLocalDecoding(bool opt){
	emit newStatusMessage(tr("Local decoding..."));
	if(!isLiveDecoding()){
		emit startLiveDecoding(opt);
	}
}

bool SpeechMe::isLiveDecoding(){
	return decoderTh->getMsrsWorker()->isLiveDecoding();
}

void SpeechMe::stopLiveDecoding(){
	decoderTh->getMsrsWorker()->stopLiveDecoding();
}

bool SpeechMe::isDecoderInitialized(){
	return decoderTh->getMsrsWorker()->isDecoderInitialized();
}

void SpeechMe::on_decoder_configured(bool status){
	if(prgDialog==NULL){
		return;
	}
	if(status){
		prgDialog->updateProgressStatus(tr("Decoder configured"), 50);
	}else{
		prgDialog->close();
		delete prgDialog;
	}
}

void SpeechMe::on_decoder_initialized(bool status){
	if(prgDialog==NULL){
		return;
	}
	if(status){
		prgDialog->updateProgressStatus(tr("Decoder initialized"), 100);
		prgDialog->close();
		delete prgDialog;
	}
}

void SpeechMe::on_hideAction_triggered(){
	this->showMinimized();
}

void SpeechMe::on_load_decoder(){
	emit loadDecoder();
	prgDialog = new ProgressDialog(this, tr("Decoder"));
	prgDialog->setModal(true);
	prgDialog->updateProgressStatus(tr("Initializing"), 0);
	prgDialog->show();
}

void SpeechMe::on_mic_calibrated(bool status){
	if(status){
		ui.statusbar->showMessage(tr("Device calibrated"), 2000);
	}else{
		ui.statusbar->showMessage(tr("Error during calibration"), 2000);
	}
}
