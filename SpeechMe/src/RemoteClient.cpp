/*
 * RemoteClient.cpp
 *
 *  Created on: 23/11/2010
 *      Author: Edwin Miguel
 */

#include "RemoteClient.h"

const char* RemoteClient::RSP_DEC_CONFIGURED="rsp_dec_conf-";
const char* RemoteClient::RSP_DEC_INITIALIZED="rsp_dec_init-";
const char* RemoteClient::RSP_DEC_READY="rsp_dec_ready-";
const char* RemoteClient::RSP_DEC_LISTENING="rsp_dec_lst-";
const char* RemoteClient::RSP_DEC_PROCESSING="rsp_dec_proc-";
const char* RemoteClient::RSP_DEC_STOPPED="rsp_dec_stop-";
const char* RemoteClient::RSP_DEC_FAIL="rsp_dec_fail-";
const char* RemoteClient::RSP_CMD_REG_OK="rsp_cmd_reg_ok-";
const char* RemoteClient::RSP_CMD_REG_FAIL="rsp_cmd_reg_fail-";
const char* RemoteClient::RSP_CMD_REM_OK="rsp_cmd_rem_ok-";
const char* RemoteClient::RSP_CMD_REM_FAIL="rsp_cmd_rem_fail-";

RemoteClient::RemoteClient(QObject *parent) :
	    QObject(parent), Observer()
	{
	//todo start all in null
	socket = NULL;
	stream = NULL;
	registered = false;
	connect(this, SIGNAL(newSentenceReady(const char*)), SLOT(on_newSentenceReady(const char*)), Qt::BlockingQueuedConnection);
	}

RemoteClient::~RemoteClient(){
}


void RemoteClient::setSocket(QTcpSocket* socket){
	this->socket = socket;
	stream = new QTextStream(socket);
	socket->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));
	connect(this->socket, SIGNAL(readyRead()), this, SLOT(dataReadytoRead()));
	connect(this->socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
}

void RemoteClient::close(){
	sendmsg(RemoteClient::RSP_CMD_REM_OK);
	socket->disconnect(SIGNAL(readyRead()));
    socket->close();
}

void RemoteClient::on_disconnected(){
	registered = false;
	emit clientUnregistered(this);
}

void RemoteClient::sendmsg(const char * message){
	try{
	*stream<<message;
	stream->flush();
	}catch(std::exception& e){
		qDebug()<<e.what();
	}
}

QTcpSocket* RemoteClient::getSocket(){
	return socket;
}

bool RemoteClient::isRegistered(){
	return registered;
}

void RemoteClient::registerClient(){
	registered = true;
	sendmsg(RemoteClient::RSP_CMD_REG_OK);
}

void RemoteClient::dataReadytoRead(){
	bool check=false;
	QString text = stream->readAll();
	//sendmsg("rsp_rcv-");
	int req = text.toInt(&check, 10);
	if(check){
		emit newRequestArrived(this, req);
	}//not a command
}

void RemoteClient::Update(Subject* subject){
	Msrs* msrs = (Msrs*)subject;
	emit newSentenceReady(getCharStatus(msrs->getStatus()));
}

void RemoteClient::UpdateSentence(Subject* subject){
	Msrs* msrs = (Msrs*)subject;
	QString* s = new QString(msrs->getLastSentence());
	s->append("-");
	string sentence = s->toStdString();
	emit newSentenceReady(sentence.c_str());
}

void RemoteClient::on_newSentenceReady(const char* sentence){
	sendmsg(sentence);
}

void RemoteClient::decoderBusy(){
	on_newSentenceReady(RSP_DEC_FAIL);
}

const char* RemoteClient::getCharStatus(int i){
	switch(i){
		case Msrs::CONFIGURED:
			return RSP_DEC_CONFIGURED;
		case Msrs::INITIALIZED:
			return RSP_DEC_INITIALIZED;
		case Msrs::READY:
			return RSP_DEC_READY;
		case Msrs::LISTENING:
			return RSP_DEC_LISTENING;
		case Msrs::PROCESSING:
			return RSP_DEC_PROCESSING;
		case Msrs::STOPPED:
			return RSP_DEC_STOPPED;
		default:
			return RSP_DEC_FAIL;
	}
}
