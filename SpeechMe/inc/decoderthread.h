#ifndef DECODERTHREAD_H
#define DECODERTHREAD_H

#include <QThread>
#include "Msrs.h"
#include "Configuration.h"

class MsrsThread : public QObject{
	Q_OBJECT
private:
	bool isolated;
	Msrs* msrs;
	Configuration* speechconf;
	bool decoderInitialized;
	void initDecoder();
	
public:
	MsrsThread(QObject * parent=0, Configuration* conf=NULL);
	void setIsolated(bool opt);
	bool isIsolated();
	void stopLiveDecoding();
	bool isLiveDecoding();
	bool isDecoderInitialized();
	void setTempClient(Observer* client);
	void Attach(Observer* observer);
	void Detach(Observer* observer);

	signals:
	void decConfigured(bool status);
	void decInitialized(bool status);
	void micCalibrated(bool status);

public slots:
	void on_load_decoder();
	void on_calib_mic();
	void on_start_live_decoding(bool isolated);
};

class DecoderThread : public QThread{
	Q_OBJECT
private:
	MsrsThread* worker;
	DecoderThread(QObject *parent = 0, Configuration* conf=NULL);
	Configuration* speechconf;
	static DecoderThread* instance;
public:
	static DecoderThread* getInstance(QObject *parent, Configuration* conf);
	void run();
	MsrsThread* getMsrsWorker();
signals:
	void decoderthStarted();
};

#endif // DECODERTHREAD_H
