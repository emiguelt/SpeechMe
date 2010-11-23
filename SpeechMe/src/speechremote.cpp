#include "inc/speechremote.h"
#include <Observer.h>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qabstractsocket.h>

SpeechRemote::SpeechRemote(QObject *parent) :
    QObject(parent), Observer()
{
	server = new QTcpServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionRequest()));
}

SpeechRemote::~SpeechRemote(){
//	msrs->Detach(this);
//	delete server;
}

void SpeechRemote::Update(Subject* subject){	
}

void SpeechRemote::UpdateSentence(Subject* subject){
	if(client!=NULL){
	const char * sentence = msrs->getLastSentence();
	QTextStream textStream(client);
	textStream<<sentence;
	textStream.flush();
	}
}

void SpeechRemote::newConnectionRequest(){
	client = server->nextPendingConnection();
	client->connect(client, SIGNAL(readyRead()), this, SLOT(dataReadytoRead()));
	client->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));
}

void SpeechRemote::dataReadytoRead(){
	QTextStream textStream(client);
	bool check=false;
	QString text = textStream.readAll();
	int req = text.toInt(&check, 10);
	if(check){
		//emit newRequestArrived(req);
	}//not a command
	
	textStream<<"Recibido-";
	textStream.flush();
}
bool SpeechRemote::startServer(int port){
	if(server->isListening()){
		if(server->serverPort()==port){
			return TRUE;
		}else{
			return FALSE;
		}
	}else{
		return server->listen(QHostAddress::LocalHost, port);
	}
	
}

void SpeechRemote::stopServer(){
	if(server->isListening()){
		server->close();
	}
}

void SpeechRemote::closeClient(){
	client->close();
}

bool SpeechRemote::isRunning(){
	return server->isListening();
}

void SpeechRemote::setMsrs(Msrs* msrs){
	this->msrs=msrs;
	msrs->Attach(this);
}
