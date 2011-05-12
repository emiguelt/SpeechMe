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
    QString getServerError();
    int numberOfConnections();
    
private:
    QTcpServer* server;
    QList<RemoteClient*> remClients;
    
    void closeClients();
    
signals:
    void newSentenceReady();
    void registerClient(RemoteClient* client);
    void clientUnregistered();

public slots:
	void newConnectionRequest();
    void on_unregistered_client(RemoteClient* client);

};

#endif // SPEECHREMOTE_H
