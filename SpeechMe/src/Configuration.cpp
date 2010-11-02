/*
 * Configuration.cpp
 *
 *  Created on: 1/11/2010
 *      Author: Edwin Miguel
 */

#include "Configuration.h"
#include <qfiledialog.h>
#include <iostream>
#include <QtGui/QMainWindow>

using namespace std;

Configuration::Configuration(QWidget *parent)
	: QWidget(parent)
	{
	ui.setupUi(this);
	}

Configuration::~Configuration()
	{
	// TODO Auto-generated destructor stub
	}

void Configuration::on_hmmButton_clicked(){
	QFileDialog* qfd = new QFileDialog;
	ui.hmmLineEdit->setText(qfd->getExistingDirectory(this, "Hmm folder", "c:\\", QFileDialog::ShowDirsOnly));
	delete qfd;
}
void Configuration::on_lmButton_clicked(){
	QFileDialog* qfd = new QFileDialog;
	ui.lmLineEdit->setText(qfd->getOpenFileName(this, "LM file", "c:\\", "*"));
	delete qfd;
}
void Configuration::on_dictButton_clicked(){
	QFileDialog* qfd = new QFileDialog;
	ui.dictLineEdit->setText(qfd->getOpenFileName(this, "Dict file", "c:\\", "*"));
	delete qfd;
}

void Configuration::on_loadButton_clicked(){
	if(msrs==NULL){
		return;
	}
	string hmm = ui.hmmLineEdit->text().toStdString();
	string lm = ui.lmLineEdit->text().toStdString();
	string dict = ui.dictLineEdit->text().toStdString();
	
	if(msrs->setConfig(NULL, cont_args_def, TRUE, "-hmm", hmm.data(), "-jsgf", lm.data(), "-dict", dict.data(), NULL)){
		if(msrs->initDecoder()){
			msrs->startLiveDecoding();
		}
	}
			
}
void Configuration::on_restoreButton_clicked(){
	//TODO restore configuration
}

void Configuration::setMsrs(Msrs* msrs){
	this->msrs=msrs;
}
