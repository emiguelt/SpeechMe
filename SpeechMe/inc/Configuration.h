/*
 * Configuration.h
 *
 *  Created on: 1/11/2010
 *      Author: Edwin Miguel
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <QtGui/QWidget>
#include "ui_config.h"
#include <Msrs.h>

class Configuration: public QWidget
	{
	Q_OBJECT
private:
	Ui::ConfigUi ui;
	Msrs* msrs;

public:
	Configuration(QWidget *parent = 0);
	void setMsrs(Msrs* msrs);
	virtual ~Configuration();
	int getServerPort();
	void setServerRunning(bool opt);
	
public slots:
        void on_langButton_clicked();
	void on_loadButton_clicked();
        void on_testButton_clicked();
	void on_serverButton_clicked();
	
signals:
	void serverButton_clicked();
        void decoder_configured(bool status);
	};

#endif /* CONFIGURATION_H_ */
