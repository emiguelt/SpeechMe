/*
============================================================================
 Name		: SpeechMe.cpp
 Author	  : Edwin Miguel Triana Gómez
 Copyright   : 
 Description : Declares Class
============================================================================
*/

#ifndef SPEECHME_H
#define SPEECHME_H

#include <QtGui/QWidget>
#include <Msrs.h>
#include <Observer.h>
#include "Configuration.h"
#include "speechpad.h"
#include "ui_SpeechMe.h"
#include "speechremote.h"

class SpeechMe : public QWidget,  public Observer
{
    Q_OBJECT

public:
	SpeechMe(QWidget *parent = 0);
    ~SpeechMe();
    virtual void Update(Subject* subject);
    virtual void UpdateSentence(Subject* subject);

private:
    Ui::SpeechMe ui;
    Configuration* conf;
    SpeechPad* speechPad;
    SpeechRemote* speechRemote;
    QAction* configAction;
    QAction* testAction;
    QWidget* currentWidget;
    QVBoxLayout* centralLayout;
    Msrs* msrs;
    
    void setMainWidget(QWidget* widget);
    void initExtraUi();
    void createConnections();
    
public slots:
	void on_configAction_triggered();
    void on_testAction_triggered();
    void on_serverButton_clicked();
};

#endif // SPEECHME_H
