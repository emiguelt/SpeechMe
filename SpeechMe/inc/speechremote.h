#ifndef SPEECHREMOTE_H
#define SPEECHREMOTE_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <Observer.h>
#include <Msrs.h>

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
    
private:
    QTcpServer* server;
    Msrs* msrs;
    
signals:

public slots:
	void newConnectionRequest();

};

#endif // SPEECHREMOTE_H
