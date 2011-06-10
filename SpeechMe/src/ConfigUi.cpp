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

ConfigUi::ConfigUi(QWidget *parent, Configuration* conf) :
			QWidget(parent)
		{
	ui.setupUi(this);
	this->conf = conf;
	speechMe = (SpeechMe*) parent;
	DecoderThread* decoder = DecoderThread::getInstance(NULL, conf);

	const QString filepath(conf->getConfigFile().data());
	ui.configFile->setText(filepath);
	ui.portSphinBox->setValue(conf->getServerPort());
	setServerRunning(speechMe->isSpeechRemoteRunning());

	connect(this, SIGNAL(serverButton_clicked()), parent,
			SLOT(on_serverButton_clicked()));
	connect(this, SIGNAL(loadDecoder()), parent, SLOT(on_load_decoder()));
	connect(decoder->getMsrsWorker(), SIGNAL(decInitialized(bool)), this,
			SLOT(on_decoder_initialized(bool)));
	connect(decoder->getMsrsWorker(), SIGNAL(decConfigured(bool)), this,
			SLOT(on_decoder_configured(bool)));
	connect(ui.configFile, SIGNAL(textChanged(QString)), this, SLOT(on_configFile_textChanged()));
	connect(ui.portSphinBox, SIGNAL(valueChanged(int)), this, SLOT(on_portSphinBox_changed()));
		}

ConfigUi::~ConfigUi()
	{
	on_configFile_textChanged();
	on_portSphinBox_changed();
	}

void ConfigUi::on_configFileButton_clicked()
	{
	QFileDialog* qfd = new QFileDialog;
	QString file(qfd->getOpenFileName(this, tr("Config. file"),"E:\\"));
	if(!file.isEmpty()){
		ui.configFile->setText(file);
	}
	delete qfd;
	}

void ConfigUi::on_loadButton_clicked()
	{
	DecoderThread* decoder = DecoderThread::getInstance(NULL, conf);
	ui.loadButton->setEnabled(FALSE);

	on_configFile_textChanged();

	emit loadDecoder();
	}

int ConfigUi::getServerPort()
	{
	return ui.portSphinBox->value();
	}

void ConfigUi::setServerRunning(bool opt)
	{
	if (opt)
		{
		ui.serverButton->setText(tr("Stop server"));
		}
	else
		{
		ui.serverButton->setText(tr("Start server"));
		}
	ui.portSphinBox->setEnabled(!opt);
	}

void ConfigUi::on_serverButton_clicked()
	{
	on_portSphinBox_changed();
	emit serverButton_clicked();
	}

void ConfigUi::on_decoder_initialized(bool status)
	{
	ui.loadButton->setEnabled(true);
	}

void ConfigUi::on_decoder_configured(bool status)
	{
	ui.loadButton->setEnabled(true);
	}

void ConfigUi::on_configFile_textChanged()
	{
	string filepath = ui.configFile->text().toStdString();
	conf->setConfigFile(filepath.data());
	}
void ConfigUi::on_portSphinBox_changed()
	{
	conf->setServerPort(ui.portSphinBox->value());
	}

void ConfigUi::setCurrentConnections(int value){
	ui.connections->setText(QString::number(value));
}
