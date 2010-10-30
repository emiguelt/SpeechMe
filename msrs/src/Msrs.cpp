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

Msrs* Msrs::instance=0;
const int Msrs::READY=1;
const int Msrs::LISTENING=2;
const int Msrs::PROCESSING=3;
const int Msrs::STOPPED=4;
const int Msrs::FAIL=5;

Msrs::Msrs()
	{
	pthread_mutex_init(&m_mutex, NULL);
	setLiveDecoding(FALSE);
	}

Msrs::~Msrs()
	{
	pthread_mutex_destroy(&m_mutex);
	// TODO free resources
	}

//Singleton pattern for Msrs class
Msrs* Msrs::getInstance(){
	if(!instance){
		instance = new Msrs();
	}
	return instance;
}

bool Msrs::setConfig(cmd_ln_t *inout_cmdln, arg_t const *defn, int32 strict, ...){
//	config = cmd_ln_init(inout_cmdln, defn,strict);
	config = cmd_ln_init(NULL, cont_args_def, TRUE, "-hmm", "c:/commands/hmm", "-samprate", "11050",
				"-dict", "c:/commands/commands.dic", "-jsgf", "c:/commands/commands.jsgf", "-latsize", "1000" , NULL);
	return config!=NULL;
}

bool Msrs::initDecoder(){
	ps = ps_init(config);
	return ps!=NULL;
}

bool Msrs::startLiveDecoding(){
	if(!ps)
		return FALSE;
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
	pthread_mutex_lock(&m_mutex);
	liveDecoding = decoding;
	pthread_mutex_unlock(&m_mutex);
}

bool Msrs::isLiveDecoding()const {
	return liveDecoding;
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
        return;
    }

    /* Initialize continuous listening module */
    if ((cont = cont_ad_init(ad, ad_read)) == NULL){
        E_ERROR("Failed to initialize voice activity detection\n");
        setStatus(FAIL);
        return;
    }
    if (ad_start_rec(ad) < 0){
        E_ERROR("Failed to start recording\n");
        setStatus(FAIL);
        return;
    }
    if (cont_ad_calib(cont) < 0){
        E_ERROR("Failed to calibrate voice activity detection\n");
        setStatus(FAIL);
        return;
    }

    setLiveDecoding(TRUE);
    for (;liveDecoding;) {
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
            continue;
        }

        /*
         * Non-zero amount of data received; start recognition of new utterance.
         * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
         */
        if (ps_start_utt(ps, NULL) < 0){
            E_ERROR("Failed to start utterance\n");
            setStatus(FAIL);
            continue;
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
                break;//todo: revisar
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
    }

    setStatus(STOPPED);
    cont_ad_close(cont);
    ad_close(ad);
}

void Msrs::sighandler(int signo)
{
    longjmp(jbuf, 1);
}

void Msrs::go(){
	pthread_create(&m_thread,0,Msrs::start_thread,(void*)this);
}


void* Msrs::start_thread(void *obj){
	reinterpret_cast<Msrs *>(obj)->recognize_from_microphone();
}

