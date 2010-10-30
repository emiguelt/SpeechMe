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
#include <pthread.h>
#include "Observer.h"


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
private:
	Msrs();
	static Msrs *instance;
	jmp_buf jbuf;
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE* rawfd;
	bool liveDecoding;
	char const *lastSentence;
	int status;
	pthread_t m_thread;
	pthread_mutex_t m_mutex;
	
	void setStatus(int newStatus);
	void sleep_msec(int32 ms);
	void	recognize_from_microphone();
	void	sighandler(int signo);
public:
	static Msrs *getInstance();
	static void *start_thread(void *obj);
	bool setConfig(cmd_ln_t *inout_cmdln, const arg_t *defn, int32 strict, ...);
    bool initDecoder();
    bool startLiveDecoding();
    void stopLiveDecoding();
    bool isLiveDecoding() const;
    void setLiveDecoding(bool decoding);
    char const * getLastSentence();
    void setLastSentence(char const* newSentence);
    int getStatus();
    void go();
    virtual ~Msrs();
    
public:
    //static variables
	static const int READY ;
	static const int LISTENING;
	static const int PROCESSING;
	static const int STOPPED;
	static const int FAIL;
};

#endif /* MSRS_H_ */
