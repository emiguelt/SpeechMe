#include "speechremote.h"
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
	Msrs::getInstance()->Attach(this);
}

SpeechRemote::~SpeechRemote(){
	Msrs::getInstance()->Detach(this);
//	delete server;
}

void SpeechRemote::Update(Subject* subject){	
}

void SpeechRemote::UpdateSentence(Subject* subject){
}



void SpeechRemote::newConnectionRequest(){
	QTcpSocket* newClient = server->nextPendingConnection();
	RemoteClient* client = new RemoteClient(this);
	remClients.append(client);
	client->setSocket(newClient);
	emit registerClient(client);
	
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
		closeClients();
	}
}

void SpeechRemote::closeClients(){
	RemoteClient* client;
	while(!remClients.isEmpty()){
		client = remClients.first();
		removeClient(client);
	}
}

bool SpeechRemote::isRunning(){
	return server->isListening();
}

void SpeechRemote::removeClient(RemoteClient* client){
	remClients.removeOne(client);
	client->close();
//	client->disconnect();
}

