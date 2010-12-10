/*
 * RemoteClient.h
 *
 *  Created on: 23/11/2010
 *      Author: Edwin Miguel
 */

#ifndef REMOTECLIENT_H_
#define REMOTECLIENT_H_

#include <QObject>
#include <QtNetwork/qtcpsocket.h>
#include <QTextStream>
#include <Observer.h>
#include <Msrs.h>

class RemoteClient : public QObject, public Observer
{
    Q_OBJECT
    
private:
    QTcpSocket* socket;
    QTextStream* stream;
    bool registered;
    
    const char* getCharStatus(int i);
    
    static const char* RSP_DEC_CONFIGURED;
    static const char* RSP_DEC_INITIALIZED;
   	static const char* RSP_DEC_READY;
   	static const char* RSP_DEC_LISTENING;
   	static const char* RSP_DEC_PROCESSING;
   	static const char* RSP_DEC_STOPPED;
   	static const char* RSP_DEC_FAIL;
   	static const char* RSP_CMD_REG_OK;
   	static const char* RSP_CMD_REG_FAIL;
   	static const char* RSP_CMD_REM_OK;
   	static const char* RSP_CMD_REM_FAIL;
	
public:
	RemoteClient(QObject *parent = 0);
	virtual ~RemoteClient();
	void setSocket(QTcpSocket* socket);
	void close();
	void sendmsg(const char * message);
	QTcpSocket* getSocket();
	bool isRegistered();
	void registerClient();
	virtual void Update(Subject *subject);
    virtual void UpdateSentence(Subject *subject);
    void decoderBusy();

public slots:
	void dataReadytoRead();
	
private slots:
	void on_newSentenceReady(const char*);
	
signals:
	void newRequestArrived(RemoteClient* client, int req);
	void newSentenceReady(const char*);
	void unregistry(RemoteClient* client);

};
#endif /* REMOTECLIENT_H_ */
