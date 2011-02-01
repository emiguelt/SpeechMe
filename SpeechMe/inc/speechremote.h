#ifndef SPEECHREMOTE_H
#define SPEECHREMOTE_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QTextStream>
#include "RemoteClient.h"
#include <QList>

class SpeechRemote : public QObject
{
    Q_OBJECT
public:
    explicit SpeechRemote(QObject *parent = 0);
    ~SpeechRemote();
    bool startServer(int port);
    void stopServer();
    bool isRunning();
    void removeClient(RemoteClient* client);
    
private:
    QTcpServer* server;
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
