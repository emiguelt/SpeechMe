#include "inc/decoderthread.h"
#include <qapplication.h>
#include <iostream>

using namespace std;

DecoderThread* DecoderThread::instance=0;

MsrsThread::MsrsThread(QObject* parent, Configuration* conf):QObject(parent){
	decoderInitialized = false;
	speechconf = conf;
	msrs = Msrs::getInstance();
}


bool MsrsThread::isIsolated(){
	return isolated;
}

void MsrsThread::setIsolated(bool opt){
	isolated = opt;
}

void MsrsThread::setTempClient(Observer* client){
	msrs->setTempClient(client);
}

void MsrsThread::stopLiveDecoding(){
	msrs->stopLiveDecoding();
}
bool MsrsThread::isLiveDecoding(){
	return msrs->isLiveDecoding();
}
bool MsrsThread::isDecoderInitialized(){
	return decoderInitialized;
}

void MsrsThread::on_load_decoder(){
	string file = speechconf->getConfigFile();
	if(msrs->setConfig(file.data())){
		emit decConfigured(true);
		decoderInitialized = msrs->initDecoder();
		emit decInitialized(decoderInitialized);
	}else{
		emit decConfigured(false);
	}
}

void MsrsThread::on_calib_mic(){
	emit micCalibrated(msrs->calibrate_device());
}

void MsrsThread::on_start_live_decoding(bool isolated){
	msrs->startLiveDecoding(isolated);
}

void MsrsThread::Attach(Observer* observer){
	msrs->Attach(observer);
}

void MsrsThread::Detach(Observer* observer){
	msrs->Detach(observer);
}

DecoderThread::DecoderThread(QObject *parent, Configuration* conf) :
										QThread(parent){
	speechconf = conf;
}

DecoderThread* DecoderThread::getInstance(QObject *parent, Configuration* conf){
	if(!instance){
		instance = new DecoderThread(parent, conf);
	}
	return instance;
}

void DecoderThread::run(){
	worker = new MsrsThread(NULL, speechconf);
	connect(worker, SIGNAL(decConfigured(bool)), parent(), SLOT(on_decoder_configured(bool)));
	connect(worker, SIGNAL(decInitialized(bool)), parent(), SLOT(on_decoder_initialized(bool)));
	connect(worker, SIGNAL(micCalibrated(bool)), parent(), SLOT(on_mic_calibrated(bool)));
	connect(parent(), SIGNAL(startLiveDecoding(bool)), worker, SLOT(on_start_live_decoding(bool)));
	connect(parent(), SIGNAL(loadDecoder()), worker , SLOT(on_load_decoder()));
	emit decoderthStarted();
	exec();
}

MsrsThread* DecoderThread::getMsrsWorker(){
	return worker;
}
