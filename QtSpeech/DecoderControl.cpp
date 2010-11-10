/*
 * DecoderControl.cpp
 *
 *  Created on: 11/10/2010
 *      Author: Edwin Miguel
 */

#include "DecoderControl.h"
#include <iostream>
#include <Msrs.h>
#include <Observer.h>
#include <errno.h>
#include <string>

using namespace std;

DecoderControl::DecoderControl(QtSpeech* newView)
	{
	msrs = Msrs::getInstance();
	msrs->Attach(this);
	myview = newView;
	}

DecoderControl::~DecoderControl()
	{
	msrs->Detach(this);
	delete msrs;
	}

void DecoderControl::initDecoder(string hmm, string lm, string dict){
	if(msrs->isLiveDecoding()){
		msrs->stopLiveDecoding();
		return;
	}
	if(msrs->setConfig(NULL, cont_args_def, TRUE, "-hmm", hmm.data(), "-jsgf", lm.data(), "-dict", dict.data(), NULL)){
		myview->addSentence("Decoder configured\n");
		if(msrs->initDecoder()){
			myview->addSentence("Decoder initialized\n");
			if(msrs->startLiveDecoding()){
				myview->addSentence("Decoder ready\n");
			}else{
				myview->addSentence("Decoder not ready\n");
				myview->addSentence(strerror(errno));
			}
		}else{
			myview->addSentence("Decoder not initialized\n");
			myview->addSentence(strerror(errno));
		}
	}else{
		myview->addSentence("Decoder not configured\n");
		myview->addSentence(strerror(errno));
	}
}

void DecoderControl::Update(Subject* subject){
	switch(msrs->getStatus()){
		case 0:
			myview->addSentence("Configured");
			break;
		case 1:
			myview->addSentence("Ready...");
			break;
		case 2:
			myview->addSentence("listening...");
			break;
		case 3:
			myview->addSentence("processing...");
			break;
		case 4:
			myview->addSentence("stopped");
			break;
		case 99:
			myview->addSentence("fail...");
			break;
		default:
			myview->addSentence("State unknown");
	}
}

void DecoderControl::UpdateSentence(Subject* subject){
	myview->addSentence(msrs->getLastSentence());
}
