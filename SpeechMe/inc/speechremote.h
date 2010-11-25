#ifndef SPEECHREMOTE_H
#define SPEECHREMOTE_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QTextStream>
#include <Observer.h>
#include <Msrs.h>
#include "RemoteClient.h"

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
    void registerClient();
    void removeClient();
    
private:
    QTcpServer* server;
    Msrs* msrs;
    RemoteClient* client;
    const char* sentence;
    
    void closeClient();
    
private slots:
    void on_newSentenceReady();
    
signals:
    void newRequestArrived(int request);
    void newSentenceReady();

public slots:
	void newConnectionRequest();
	void dataReadytoRead();

};

#endif // SPEECHREMOTE_H
