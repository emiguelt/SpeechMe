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
	
public slots:
	void on_hmmButton_clicked();
	void on_lmButton_clicked();
	void on_dictButton_clicked();
	void on_loadButton_clicked();
	void on_restoreButton_clicked();
	};

#endif /* CONFIGURATION_H_ */
