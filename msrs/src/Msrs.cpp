/*
 * Msrs.cpp
 *
 *  Created on: 25/10/2010
 *      Author: Edwin Miguel
 */

#include "Msrs.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>
#include <pthread.h>
#include <iostream>
//#include <fstream>

using namespace std;

Msrs* Msrs::instance=0;

Msrs::Msrs()
	{
	//pthread_mutex_init(&m_mutexSt, NULL);
	//pthread_mutex_init(&m_mutexLd, NULL);
	//pthread_mutex_init(&m_mutexId, NULL);
	setLiveDecoding(FALSE);
	setIsolatedDecoding(FALSE);
	config=NULL;
	ps=NULL;
	tempClient=NULL;
	}

Msrs::~Msrs()
	{
	//pthread_mutex_destroy(&m_mutexSt);
	//pthread_mutex_destroy(&m_mutexLd);
	//pthread_mutex_destroy(&m_mutexId);
	if(ps!=NULL){
		ps_free(ps);
	}
	}

//Singleton pattern for Msrs class
Msrs* Msrs::getInstance(){
	if(!instance){
		instance = new Msrs();
	}
	return instance;
}

bool Msrs::setConfig(const char* lm, const char* hmm, const char* dict, const char* samprate, bool isJsgf){
	if(ps!=NULL){
		ps_free(ps);
		ps =  NULL;
	}
	char *lmopt = "-jsgf";
	if(!isJsgf){
		lmopt = "-lm";
	}
	
	//ofstream logfile;
	//logfile.open ("logfile.txt");
	//logfile.close();

	
	config = cmd_ln_init(NULL, cont_args_def, TRUE, "-hmm", hmm, "-samprate", samprate,
			"-dict", dict, lmopt, lm, "-logfn", "E://logfile.txt", NULL);
	if(config!=NULL){
		setStatus(CONFIGURED);
		return true;
	}else{
		setStatus(FAIL);
		return false;
	}
}

bool Msrs::initDecoder(){
	if(ps!=NULL){
		ps_free(ps);
		ps =  NULL;
	}
	ps = ps_init(config);
		if(ps!=NULL){
		setStatus(INITIALIZED);
		return true;
	}else{
		setStatus(FAIL);
		return false;
	}
}

bool Msrs::startLiveDecoding(bool isolated){
	if(ps==NULL){
		setStatus(FAIL);
		return FALSE;
	}
	if(isLiveDecoding()){
		return FALSE;
	}
	setIsolatedDecoding(isolated);
	if (setjmp(jbuf) == 0) {
		go();
	}
	return TRUE;
}

void Msrs::stopLiveDecoding(){
	setLiveDecoding(FALSE);
	pthread_join(m_thread, NULL);
}

void Msrs::setLiveDecoding(bool decoding){
	//pthread_mutex_lock(&m_mutexLd);
	liveDecoding = decoding;
	//pthread_mutex_unlock(&m_mutexLd);
}

bool Msrs::isLiveDecoding()const {
	return liveDecoding;
}

void Msrs::setIsolatedDecoding(bool isolated){
	//pthread_mutex_lock(&m_mutexId);
	this->isolatedDecoding = isolated;
	//pthread_mutex_unlock(&m_mutexId);
}

bool Msrs::isIsolatedDecoding()const {
	return isolatedDecoding;
}

void Msrs::setLastSentence(char const * newSentence){
	lastSentence = newSentence;
}

char const * Msrs::getLastSentence(){
	return lastSentence;
}


/* Sleep for specified msec */
void Msrs::sleep_msec(int32 ms)
	{
#if (defined(WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
	Sleep(ms);
#else
	/* ------------------- Unix ------------------ */
	struct timeval tmo;

	tmo.tv_sec = 0;
	tmo.tv_usec = ms * 1000;

	select(0, NULL, NULL, NULL, &tmo);
#endif
	}

void Msrs::setStatus(int newStatus){
//	pthread_mutex_lock(&m_mutexSt);
	status=newStatus;
//	pthread_mutex_unlock(&m_mutexSt);

	Notify();
}

int Msrs::getStatus(){
	return status;
}

void Msrs::recognize_from_microphone()
	{
	ad_rec_t *ad;
	int16 adbuf[4096];
	int32 k, ts, rem;
	char const *hyp;
	char const *uttid;
	cont_ad_t *cont;
	char word[256];

	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
			(int)cmd_ln_float32_r(config, "-samprate"))) == NULL){
		E_ERROR("Failed top open audio device\n");
		setStatus(FAIL);
	}else{
		//setStatus(23);
		if ((cont = cont_ad_init(ad, ad_read)) == NULL){/* Initialize continuous listening module */
			E_ERROR("Failed to initialize voice activity detection\n");       
			setStatus(FAIL);
			ad_close(ad);
		}else{
			//setStatus(24);
			if(ad_start_rec(ad) < 0){
				cont_ad_close(cont);
				ad_close(ad);
				E_ERROR("Failed to start recording\n");
				setStatus(FAIL);
			}else{
				//setStatus(25);
				if (cont_ad_calib(cont) < 0){
					cont_ad_close(cont);
					ad_close(ad);
					E_ERROR("Failed to calibrate voice activity detection\n");
					setStatus(FAIL);
				}else{
					//setStatus(21);
					setLiveDecoding(TRUE);
					do{
						/* Indicate listening for next utterance */
						setStatus(READY);

						/* Wait data for next utterance */
						while ((k = cont_ad_read(cont, adbuf, 4096)) == 0 && liveDecoding)
							sleep_msec(100);
						if(!liveDecoding){
							break;
						}

						if (k < 0){
							E_ERROR("Failed to read audio\n");
							setLiveDecoding(FALSE);
							setStatus(FAIL);
							break;
						}

						/*
						 * Non-zero amount of data received; start recognition of new utterance.
						 * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
						 */
						if (ps_start_utt(ps, NULL) < 0){
							E_ERROR("Failed to start utterance\n");
							setStatus(FAIL);
							break;
						}
						ps_process_raw(ps, adbuf, k, FALSE, FALSE);
						setStatus(LISTENING);

						/* Note timestamp for this first block of data */
						ts = cont->read_ts;

						/* Decode utterance until end (marked by a "long" silence, >1sec) */
						for (;liveDecoding;) {
							/* Read non-silence audio data, if any, from continuous listening module */
							if ((k = cont_ad_read(cont, adbuf, 4096)) < 0){
								E_ERROR("Failed to read audio\n");
								setLiveDecoding(FALSE);
								setStatus(FAIL);
								break;
							}
							if (k == 0) {
								/*
								 * No speech data available; check current timestamp with most recent
								 * speech to see if more than 1 sec elapsed.  If so, end of utterance.
								 */
								if ((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
									break;
							}
							else {
								/* New speech data received; note current timestamp */
								ts = cont->read_ts;
							}

							/*
							 * Decode whatever data was read above.
							 */
							rem = ps_process_raw(ps, adbuf, k, FALSE, FALSE);

							/* If no work to be done, sleep a bit */
							if ((rem == 0) && (k == 0))
								sleep_msec(20);
						}
						if(!liveDecoding){
							break;
						}

						/*
						 * Utterance ended; flush any accumulated, unprocessed A/D data and stop
						 * listening until current utterance completely decoded
						 */
						ad_stop_rec(ad);
						while (ad_read(ad, adbuf, 4096) >= 0 && liveDecoding);
						if(!liveDecoding){
							break;
						}
						cont_ad_reset(cont);

						setStatus(PROCESSING);

						/* Finish decoding, obtain and print result */
						ps_end_utt(ps);
						hyp = ps_get_hyp(ps, NULL, &uttid);

						setLastSentence(hyp);
						NotifyNewSentece();

						/* Resume A/D recording for next utterance */
						if (ad_start_rec(ad) < 0){
							E_ERROR("Failed to start recording\n");
							setLiveDecoding(FALSE);
							setStatus(FAIL);
							break;//todo: revisar
						}
						if(isolatedDecoding){
							break;
						}
					}while(liveDecoding);

					setLiveDecoding(FALSE);
					setStatus(STOPPED);
					cont_ad_close(cont);
					ad_close(ad);

					if(tempClient!=NULL){
						Detach(tempClient);
						tempClient=NULL;
					}
				}
			}}}
	}

void Msrs::sighandler(int signo)
	{
	longjmp(jbuf, 1);
	}

void Msrs::go(){
	//setStatus(20);
	recognize_from_microphone();
	//pthread_create(&m_thread,NULL,Msrs::start_thread,(void*)this);
}


void* Msrs::start_thread(void *obj){
	//Msrs::getInstance()->setStatus(27);
	Msrs::getInstance()->recognize_from_microphone();
	//Msrs::getInstance()->setStatus(30);
	//	reinterpret_cast<Msrs *>(obj)->recognize_from_microphone();
}

void Msrs::setTempClient(Observer* client){
	if(!isLiveDecoding()){
		tempClient = client;
		Attach(tempClient);
	}
}

