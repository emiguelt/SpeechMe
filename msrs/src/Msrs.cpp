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
#include <iostream>

using namespace std;

Msrs* Msrs::instance=0;

Msrs::Msrs()
	{
	setLiveDecoding(FALSE);
	setIsolatedDecoding(FALSE);
	config=NULL;
	ps=NULL;
	tempClient=NULL;
	}

Msrs::~Msrs()
	{
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

bool Msrs::setConfig(const char* conffile){
	if(ps!=NULL){
		ps_free(ps);
		ps = NULL;
		config = NULL;
	}
	if(conffile == NULL){
		setStatus(FAIL);
		return false;
	}

	config = cmd_ln_parse_file_r(config, cont_args_def, conffile, FALSE);

	if(config!=NULL){
		setStatus(CONFIGURED);
		return true;
	}else{
		setStatus(FAIL);
		return false;
	}
}

bool Msrs::initDecoder(){
	if(config==NULL){
		setStatus(FAIL);
		return false;
	}
	if(ps!=NULL){
		ps_free(ps);
		ps = NULL;
	}
	ps = ps_init(config);

	if(ps!=NULL){//&& init_cont_module()
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
		recognize_from_microphone();
	}
	return TRUE;
}

void Msrs::stopLiveDecoding(){
	setLiveDecoding(FALSE);
}

void Msrs::setLiveDecoding(bool decoding){
	liveDecoding = decoding;
}

bool Msrs::isLiveDecoding()const {
	return liveDecoding;
}

void Msrs::setIsolatedDecoding(bool isolated){
	this->isolatedDecoding = isolated;
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
	status=newStatus;

	Notify();
}

int Msrs::getStatus(){
	return status;
}
bool Msrs::init_cont_module(){
	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
			(int)cmd_ln_float32_r(config, "-samprate"))) == NULL){
		E_ERROR("Failed top open audio device\n");
		setStatus(FAIL);
		return false;
	}else if((cont = cont_ad_init(ad, ad_read)) == NULL){/* Initialize continuous listening module */
		E_ERROR("Failed to initialize voice activity detection\n");       
		setStatus(FAIL);
		ad_close(ad);
		return false;
	}
	return true;

}

bool Msrs::calibrate_device(){
	if(init_cont_module()){
		if(ad_start_rec(ad) < 0){
			close_cont_module();
			E_ERROR("Failed to start recording\n");
			setStatus(FAIL);
			return false;
		}else if (cont_ad_calib(cont) < 0){
			close_cont_module();
			E_ERROR("Failed to calibrate voice activity detection\n");
			setStatus(FAIL);
			return false;
		}
		close_cont_module();
		return true;
	}else{
		setStatus(FAIL);
		return false;
	}
}

void Msrs::close_cont_module(){
	cont_ad_close(cont);
	ad_close(ad);
}

void Msrs::recognize_from_microphone()
	{

	int16 adbuf[4096];
	int32 k, ts, rem;
	char const *hyp;
	char const *uttid;

	if(init_cont_module())
		//if(calibrate_device())
		if(ad_start_rec(ad) < 0){
			close_cont_module();
			E_ERROR("Failed to start recording\n");
			setStatus(FAIL);
		}else{
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

				/* Resume A/D recording for next utterance */
				if (ad_start_rec(ad) < 0){
					E_ERROR("Failed to start recording\n");
					setLiveDecoding(FALSE);
					setStatus(FAIL);
					break;//todo: revisar
				}
				NotifyNewSentece();
				if(isolatedDecoding){
					break;
				}
			}while(liveDecoding);

			//ad_stop_rec(ad);
			close_cont_module();
			setLiveDecoding(FALSE);
			setStatus(STOPPED);

			if(tempClient!=NULL){
				Detach(tempClient);
				tempClient=NULL;
			}
		}
	}

void Msrs::sighandler(int signo)
	{
	longjmp(jbuf, 1);
	}

void Msrs::setTempClient(Observer* client){
	if(!isLiveDecoding()){
		tempClient = client;
		Attach(tempClient);
	}
}

