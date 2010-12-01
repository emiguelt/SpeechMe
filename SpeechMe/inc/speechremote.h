#ifndef SPEECHREMOTE_H
#define SPEECHREMOTE_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QTextStream>
#include <Msrs.h>
#include "RemoteClient.h"
#include <QList>

class SpeechRemote : public QObject, Observer
{
    Q_OBJECT
public:
    explicit SpeechRemote(QObject *parent = 0);
    ~SpeechRemote();
    virtual void Update(Subject *subject);
    virtual void UpdateSentence(Subject *subject);
    bool startServer(int port);
    void stopServer();
    bool isRunning();
    void setMsrs(Msrs* msrs);
    void removeClient(RemoteClient* client);
    
private:
    QTcpServer* server;
    Msrs* msrs;
    QList<RemoteClient*> remClients;
    
    void closeClients();
    
signals:
    void newSentenceReady();
    void registerClient(RemoteClient* client);

public slots:
	void newConnectionRequest();
//	void dataReadytoRead();

};

#endif // SPEECHREMOTE_H
