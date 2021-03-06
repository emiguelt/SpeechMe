/*
============================================================================
 Name		: SpeechMe.cpp
 Author	  : Edwin Miguel Triana G�mez
 Copyright   : 
 Description : Declares Class
============================================================================
*/

#ifndef SPEECHME_H
#define SPEECHME_H

#include <qmainwindow.h>
#include <Msrs.h>
#include <Observer.h>
#include "Configuration.h"
#include "ui_SpeechMe2.h"
#include "speechremote.h"
#include "decoderthread.h"
#include "progressdialog.h"

namespace Ui {
    class SpeechMe2;
}

class SpeechPad;
class SpeechWeb;
class ConfigUi;

class SpeechMe : public QMainWindow,  public Observer
{
    Q_OBJECT

public:
	SpeechMe(QMainWindow *parent = 0);
    ~SpeechMe();
    virtual void Update(Subject* subject);
    virtual void UpdateSentence(Subject* subject);
    void initLocalDecoding(bool opt);
    void stopLiveDecoding();
    bool isLiveDecoding();
    bool isDecoderInitialized();
    bool isSpeechRemoteRunning();
    
    static const int CMD_ISOLATED_RECOGNITION=1;
    static const int CMD_CONTINUOUS_RECOGNITION=2;
    static const int CMD_REGISTER_CLIENT=3;
    static const int CMD_REMOVE_CLIENT=4;
    

private:
    Ui::SpeechMe2 ui;
    Configuration* conf;
    ConfigUi* configui;
    SpeechPad* speechPad;
    SpeechWeb* speechWeb;
    SpeechRemote* speechRemote;
    DecoderThread* decoderTh;
    ProgressDialog* prgDialog;
    
    //void initExtraUi();
    void createConnections();
    void initDecoding(RemoteClient * client, bool opt);
    void updateMenu(QAction * menuOpt);
    void loadSettings();
    void writeSettings();

public slots:
	void on_configAction_triggered();
    void on_testAction_triggered();
    void on_webAction_triggered();
    void on_hideAction_triggered();
    void on_serverButton_clicked();
    void on_newrequest_arrived(RemoteClient* client, int request);
    void on_registerClient(RemoteClient* client);
    void on_clientQueueModified();
    void on_mic_calibrated(bool status);
    void on_decoder_initialized(bool status);
    void on_decoder_configured(bool status);
    void on_load_decoder();
    void on_decoderth_started();
    
private slots:
    void on_updated_decoder_status(int statusNumber);
    
signals:
    void newStatusMessage(const QString &);
    void startLiveDecoding(bool);
    void decoderStatusUpdated(int);
    void loadDecoder();
};

#endif // SPEECHME_H
