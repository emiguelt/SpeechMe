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
    
    static const int CMD_ISOLATED_RECOGNITION=1;
    static const int CMD_CONTINUOUS_RECOGNITION=2;
    static const int CMD_REGISTER_CLIENT=3;
    static const int CMD_REMOVE_CLIENT=4;
    

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
    void on_newrequest_arrived(int request);
};

#endif // SPEECHME_H
