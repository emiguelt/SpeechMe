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
	client = NULL;
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionRequest()));
	connect(this, SIGNAL(newSentenceReady()), SLOT(on_newSentenceReady()), Qt::BlockingQueuedConnection);
}

SpeechRemote::~SpeechRemote(){
//	msrs->Detach(this);
//	delete server;
}

void SpeechRemote::Update(Subject* subject){	
}

void SpeechRemote::UpdateSentence(Subject* subject){
	if(client!=NULL && client->isRegistered()){
		QString* s = new QString(msrs->getLastSentence());
		s->append("-");
		string sentence = s->toStdString();
		this->sentence=sentence.c_str();
		emit newSentenceReady();
	}
}

void SpeechRemote::on_newSentenceReady(){
	client->sendmsg(sentence);
}

void SpeechRemote::newConnectionRequest(){
	QTcpSocket* newClient = server->nextPendingConnection();
	if(client == NULL){
		client = new RemoteClient();
		client->setSocket(newClient);
	}else if(!client->isRegistered()){
		client->setSocket(newClient);
	}else if(newClient != client->getSocket()){
		newClient->close();//msrs is used for other app
		return;
	}
	client->connect(client->getSocket(), SIGNAL(readyRead()), this, SLOT(dataReadytoRead()));
}

void SpeechRemote::dataReadytoRead(){
	bool check=false;
	QString text = client->getMsg();
	//client->sendmsg("cmd_rcv-");
	int req = text.toInt(&check, 10);
	if(check){
		emit newRequestArrived(req);
	}//not a command
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
		closeClient();
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

void SpeechRemote::registerClient(){
	client->registerClient();
}

void SpeechRemote::removeClient(){
	closeClient();
	//client = NULL;
}

