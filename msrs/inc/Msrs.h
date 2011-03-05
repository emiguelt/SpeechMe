/*
 * Msrs.h
 *
 *  Created on: 25/10/2010
 *      Author: Edwin Miguel
 */

#ifndef MSRS_H_
#define MSRS_H_

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>
#include <sphinxbase/cmd_ln.h>
#include "Observer.h"

using namespace std;
static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    { "-argfile",
      ARG_STRING,
      NULL,
      "Argument file giving extra arguments." },
    { "-adcdev", 
      ARG_STRING, 
      NULL, 
      "Name of audio device to use for input." },
    { "-infile", 
      ARG_STRING, 
      NULL, 
      "Audio file to transcribe." },
    { "-time", 
      ARG_BOOLEAN, 
      "no", 
      "Print word times in file transcription." },
    CMDLN_EMPTY_OPTION
};

class Msrs:public Subject
	{
protected:
	Msrs();
	static Msrs *instance;
	jmp_buf jbuf;
	ps_decoder_t *ps;
	cmd_ln_t *config;
	ad_rec_t *ad;
	cont_ad_t *cont;
	FILE* rawfd;
	bool liveDecoding;
	bool isolatedDecoding;
	char const *lastSentence;
	int status;
	pthread_t m_thread;
	pthread_mutex_t m_mutexSt;
	pthread_mutex_t m_mutexLd;
	pthread_mutex_t m_mutexId;
	Observer* tempClient;
	
	void setStatus(int newStatus);
	void sleep_msec(int32 ms);
	void sighandler(int signo);
	void recognize_from_microphone();
	bool init_cont_module();
	void close_cont_module();
	
public:
	static Msrs *getInstance();
	bool setConfig(const char* conffile);
    bool initDecoder();
    bool startLiveDecoding(bool isolated);
    void stopLiveDecoding();
    bool isLiveDecoding() const;
    void setLiveDecoding(bool decoding);
    void setIsolatedDecoding(bool isolated);
    bool isIsolatedDecoding()const;
    char const * getLastSentence();
    void setLastSentence(char const* newSentence);
    int getStatus();
    void setTempClient(Observer* client);
    bool calibrate_device();
    virtual ~Msrs();
    
    //static variables
    static const int INITIALIZED=0;
    static const int CONFIGURED=1;
	static const int READY=2;
	static const int LISTENING=3;
	static const int PROCESSING=4;
	static const int STOPPED=5;
	static const int FAIL=99;
};

#endif /* MSRS_H_ */
