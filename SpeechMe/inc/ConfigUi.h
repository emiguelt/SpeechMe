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
  Msrs* msrs;
  Configuration* conf;

public:
  ConfigUi(QWidget *parent, Configuration* conf);
  virtual ~ConfigUi();
  int getServerPort();
  void setServerRunning(bool opt);

public slots:
  void on_langButton_clicked();
  void on_loadButton_clicked();
  void on_serverButton_clicked();

signals:
  void serverButton_clicked();
  void decoder_configured(bool status);
};

#endif /* CONFIGUI_H_ */
