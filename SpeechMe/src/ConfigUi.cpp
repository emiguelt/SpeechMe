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

using namespace std;

ConfigUi::ConfigUi(QWidget *parent, Configuration* conf)
  : QWidget(parent)
{
  ui.setupUi(this);
  this->conf = conf;
  speechMe = (SpeechMe*)parent;
  msrs = Msrs::getInstance();

  const QString folder(conf->getFolder().data());
  ui.languageFolder->setText(folder);
  ui.jsgf->setChecked(conf->isJsgfModel());
  ui.portSphinBox->setValue(conf->getServerPort());
  setServerRunning(speechMe->isSpeechRemoteRunning());

  connect(this, SIGNAL(serverButton_clicked()), parent, SLOT(on_serverButton_clicked()));
  //  connect(this, SIGNAL(decoder_configured(bool)), parent, SLOT(on_decoder_configured(bool)));
}

ConfigUi::~ConfigUi(){
  string folder =ui.languageFolder->text().toStdString();
  conf->setFolder(folder);
  conf->setJsgfModel(ui.jsgf->isChecked());
  conf->setServerPort(ui.portSphinBox->value());
  conf->setSampRate((ui.sampRateCombo->currentText()).toInt());
}

void ConfigUi::on_langButton_clicked(){
  QFileDialog* qfd = new QFileDialog;
  ui.languageFolder->setText(qfd->getExistingDirectory(this, tr("Language folder"), "c:\\", QFileDialog::ShowDirsOnly));
  delete qfd;
}

void ConfigUi::on_loadButton_clicked(){
  
  if(msrs==NULL){
    return;
  }
  ui.loadButton->setEnabled(FALSE);

  string folder = ui.languageFolder->text().toStdString();
  conf->setFolder(folder.data());
  conf->setJsgfModel(ui.jsgf->isChecked());
  
  string hmm = (ui.languageFolder->text() + "/hmm").toStdString();
  string samprate = (ui.sampRateCombo->currentText()).toStdString();
  string lm;
  if(ui.jsgf->isChecked()){
    lm = (ui.languageFolder->text() + "/lm.jsgf").toStdString();
  }else{
    lm = (ui.languageFolder->text() + "/lm.dmp").toStdString();
  }
  string dict = (ui.languageFolder->text() + "/dict.dic").toStdString();

  if(msrs->setConfig(lm.data(), hmm.data(), dict.data(), samprate.data(), ui.jsgf->isChecked())){
//    emit decoder_configured(msrs->initDecoder());
    msrs->initDecoder();
  }
  ui.loadButton->setEnabled(TRUE);
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
