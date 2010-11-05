#include "inc/speechremote.h"
#include <Observer.h>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <qtextstream.h>

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
	
}

void SpeechRemote::newConnectionRequest(){
	QString command;
	QString client;
	QTcpSocket* conn = server->nextPendingConnection();
	QTextStream in(conn);
	in>>client;
	in>>command;
	//interpretar el comando para ejecutar
	//ejecutar el comando
	//devolver la respueta al cliente
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

bool SpeechRemote::isRunning(){
	return server->isListening();
}

void SpeechRemote::setMsrs(Msrs* msrs){
	this->msrs=msrs;
	msrs->Attach(this);
}
