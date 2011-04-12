/*
 * Configuration.h
 *
 *  Created on: 1/11/2010
 *      Author: Edwin Miguel
 */

#ifndef CONFIGUI_H_
#define CONFIGUI_H_

#include <QtGui/QWidget>
#include "ui_config.h"
#include "SpeechMe.h"
//#include "Configuration.h"

class ConfigUi: public QWidget
{
  Q_OBJECT
private:
  Ui::ConfigUi ui;
  SpeechMe* speechMe;
  Configuration* conf;

public:
  ConfigUi(QWidget *parent, Configuration* conf);
  virtual ~ConfigUi();
  int getServerPort();
  void setServerRunning(bool opt);

public slots:
  void on_configFileButton_clicked();
  void on_loadButton_clicked();
  void on_serverButton_clicked();
  void on_decoder_initialized(bool status);
  void on_decoder_configured(bool status);
  void on_configFile_textChanged();
  void on_portSphinBox_changed();

signals:
  void serverButton_clicked();
  void loadDecoder();
};

#endif /* CONFIGUI_H_ */
