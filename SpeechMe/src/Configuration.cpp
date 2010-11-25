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

void Configuration::on_langButton_clicked(){
	QFileDialog* qfd = new QFileDialog;
        ui.languageFolder->setText(qfd->getExistingDirectory(this, tr("Language folder"), "c:\\", QFileDialog::ShowDirsOnly));
	delete qfd;
}

void Configuration::on_loadButton_clicked(){
  if(msrs==NULL){
    return;
  }

  string hmm = (ui.languageFolder->text() + "/hmm").toStdString();
  string lm;
  if(ui.jsgf->isChecked()){
    lm = (ui.languageFolder->text() + "/lm.jsgf").toStdString();
      }else{
        lm = (ui.languageFolder->text() + "/lm.dmp").toStdString();
      }
        string dict = (ui.languageFolder->text() + "/dict.dic").toStdString();
	
        if(msrs->setConfig(lm.data(), hmm.data(), dict.data(), "11050", ui.jsgf->isChecked())){
          emit decoder_configured(msrs->initDecoder());
	}		
}
void Configuration::on_testButton_clicked(){
        msrs->startLiveDecoding(true);
}

void Configuration::setMsrs(Msrs* msrs){
	this->msrs=msrs;
}

int Configuration::getServerPort(){
	return ui.portSphinBox->value();
}

void Configuration::setServerRunning(bool opt){
	if(opt){
		ui.serverButton->setText(tr("Stop server"));
	}else{
		ui.serverButton->setText(tr("Start server"));
	}
	ui.portSphinBox->setEnabled(!opt);
}

void Configuration::on_serverButton_clicked(){
	emit serverButton_clicked();
}
