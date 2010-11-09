#ifndef SPEECHREMOTE_H
#define SPEECHREMOTE_H

#include <QObject>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
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
    QTcpSocket* client;
    
    void closeClient();
    
signals:
    void newCommandArrived(const QString &command);

public slots:
	void newConnectionRequest();
	void dataReadytoRead();

};

#endif // SPEECHREMOTE_H
