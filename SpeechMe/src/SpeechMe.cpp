/*
============================================================================
 Name		: SpeechMe.cpp
 Author	  : Edwin Miguel Triana Gómez
 Copyright   : 
 Description : Class Implementation
============================================================================
*/

#include "SpeechMe.h"
#include <errno.h>
#include <string>
#include <QtGui/QBoxLayout>


SpeechMe::SpeechMe(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	msrs = Msrs::getInstance();
	msrs->Attach(this);
	initExtraUi();
	conf = new Configuration(this);
	conf->hide();
	conf->setMsrs(msrs);
	
	speechPad = new SpeechPad(conf);
	speechPad->hide();
	speechPad->setMsrs(msrs);
	
	speechRemote = new SpeechRemote(this);
	speechRemote->setMsrs(msrs);
	
	currentWidget = NULL;
	createConnections();
	
	on_configAction_triggered();

}

SpeechMe::~SpeechMe()
{
	msrs->Detach(this);
	delete msrs;
}

void SpeechMe::initExtraUi(){
	centralLayout = new QVBoxLayout(ui.mainWidget);

  setContextMenuPolicy(Qt::NoContextMenu);

  configAction = new QAction(tr("Config"), this);
  configAction->setSoftKeyRole(QAction::PositiveSoftKey);

  testAction = new QAction(tr("Test"), this);
  testAction->setSoftKeyRole(QAction::PositiveSoftKey);

  addAction(configAction);
  addAction(testAction);

}

void SpeechMe::createConnections(){
  connect(configAction, SIGNAL(triggered()),this , SLOT(on_configAction_triggered()));
  connect(testAction, SIGNAL(triggered()),this , SLOT(on_testAction_triggered()));
  connect(conf, SIGNAL(serverButton_clicked()), this, SLOT(on_serverButton_clicked()));
  connect(speechRemote, SIGNAL(newRequestArrived(int)), this, SLOT(on_newrequest_arrived(int)));
}

void SpeechMe::Update(Subject* subject){
	switch(msrs->getStatus()){
		case Msrs::CONFIGURED:
		    ui.statusBar->setText("Configured");
		    break;
		case Msrs::READY:
			ui.statusBar->setText("Ready");
			break;
		case Msrs::LISTENING:
			ui.statusBar->setText("listening...");
			break;
		case Msrs::PROCESSING:
			ui.statusBar->setText("processing...");
			break;
		case Msrs::STOPPED:
			ui.statusBar->setText("Stopped");
			break;
		case Msrs::FAIL:
			ui.statusBar->setText(strerror(errno));
			break;
		default:
			ui.statusBar->setText("State unknown");
			break;
		}
}

void SpeechMe::UpdateSentence(Subject* subject){
//	const QString* sent = new QString(msrs->getLastSentence());
//	ui.statusBar->setText(*sent);
}

void SpeechMe::setMainWidget(QWidget* widget){
	if(currentWidget!=NULL){
		currentWidget->hide();
		centralLayout->removeWidget(currentWidget);
	}
	centralLayout->addWidget(widget);
	currentWidget = widget;
	currentWidget->show();
}
void SpeechMe::on_configAction_triggered(){
	setMainWidget(conf);
	configAction->setVisible(false);
	testAction->setVisible(true);
}

void SpeechMe::on_testAction_triggered(){
	setMainWidget(speechPad);
	configAction->setVisible(true);
	testAction->setVisible(false);
}

void SpeechMe::on_serverButton_clicked(){
	if(speechRemote->isRunning()){
		speechRemote->stopServer();
		conf->setServerRunning(FALSE);
	}else{
		conf->setServerRunning(speechRemote->startServer(conf->getServerPort()));
	}
	
}

void SpeechMe::on_newrequest_arrived(int request){
	switch(request){
		case ISOLATED_RECOGNITION:
			msrs->startLiveDecoding(TRUE);
			break;
		case CONTINUOUS_RECOGNITION:
			msrs->startLiveDecoding(FALSE);
			break;
		default:
			break;
	}
}
