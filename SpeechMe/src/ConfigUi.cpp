/*
 * ConfigUi.cpp
 *
 *  Created on: 1/11/2010
 *      Author: Edwin Miguel
 */

#include "ConfigUi.h"
#include <qfiledialog.h>
#include <iostream>
#include <QtGui/QMainWindow>
#include "decoderthread.h"
#include <qmessagebox.h>

using namespace std;

ConfigUi::ConfigUi(QWidget *parent, Configuration* conf)
: QWidget(parent){
	ui.setupUi(this);
	this->conf = conf;
	speechMe = (SpeechMe*)parent;
	DecoderThread* decoder = DecoderThread::getInstance(NULL, conf);

	const QString filepath(conf->getConfigFile().data());
	ui.configFile->setText(filepath);
	ui.portSphinBox->setValue(conf->getServerPort());
	setServerRunning(speechMe->isSpeechRemoteRunning());

	connect(this, SIGNAL(serverButton_clicked()), parent, SLOT(on_serverButton_clicked()));
	connect(this, SIGNAL(loadDecoder()), parent, SLOT(on_load_decoder()));
	connect(decoder->getMsrsWorker(), SIGNAL(decInitialized(bool)), this, SLOT(on_decoder_initialized(bool)));
	connect(decoder->getMsrsWorker(), SIGNAL(decConfigured(bool)), this, SLOT(on_decoder_configured(bool)));
}

ConfigUi::~ConfigUi(){
	string filepath =ui.configFile->text().toStdString();
	conf->setConfigFile(filepath);
	conf->setServerPort(ui.portSphinBox->value());
}

void ConfigUi::on_configFileButton_clicked(){
	QFileDialog* qfd = new QFileDialog;
	ui.configFile->setText(qfd->getOpenFileName(this, tr("Config. file"), "E:\\"));
	delete qfd;
}

void ConfigUi::on_loadButton_clicked(){
	DecoderThread* decoder = DecoderThread::getInstance(NULL, conf);
	ui.loadButton->setEnabled(FALSE);

	string filepath = ui.configFile->text().toStdString();
	conf->setConfigFile(filepath.data());

	emit loadDecoder();
}

int ConfigUi::getServerPort(){
	return ui.portSphinBox->value();
}

void ConfigUi::setServerRunning(bool opt){
	if(opt){
		ui.serverButton->setText(tr("Stop server"));
	}else{
		ui.serverButton->setText(tr("Start server"));
	}
	ui.portSphinBox->setEnabled(!opt);
}

void ConfigUi::on_serverButton_clicked(){
	conf->setServerPort(ui.portSphinBox->value());
	emit serverButton_clicked();
}

void ConfigUi::on_decoder_initialized(bool status){
	ui.loadButton->setEnabled(true);
}

void ConfigUi::on_decoder_configured(bool status){
	ui.loadButton->setEnabled(true);
}
