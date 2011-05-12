#include "speechremote.h"
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qabstractsocket.h>

SpeechRemote::SpeechRemote(QObject *parent) :
    QObject(parent)
{
	server = new QTcpServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionRequest()));
}

SpeechRemote::~SpeechRemote(){
//	delete server;
}

void SpeechRemote::newConnectionRequest(){
	QTcpSocket* newClient = server->nextPendingConnection();
	RemoteClient* client = new RemoteClient(this);
	connect(client, SIGNAL(clientUnregistered(RemoteClient *)), this, SLOT(on_unregistered_client(RemoteClient *)));
	remClients.append(client);
	client->setSocket(newClient);
	emit registerClient(client);
}

QString SpeechRemote::getServerError(){
    return server->errorString();
}

bool SpeechRemote::startServer(int port){
	if(server->isListening()){
		if(server->serverPort()==port){
			return TRUE;
		}else{
			return FALSE;
		}
	}else{
            return (server->listen(QHostAddress::LocalHost, port));
	}
	
}

void SpeechRemote::stopServer(){
	if(server->isListening()){
		server->close();
		closeClients();
	}
}

void SpeechRemote::closeClients(){
	int i;
	RemoteClient * client;
	for(i = 0;  i < remClients.size(); i++){
		client = remClients.at(i);
		client->close();
	}
}

bool SpeechRemote::isRunning(){
	return server->isListening();
}

void SpeechRemote::on_unregistered_client(RemoteClient * client){
	remClients.removeOne(client);
	emit clientUnregistered();
}

int SpeechRemote::numberOfConnections(){
	return remClients.size();
}

