/*
 * DecoderControl.h
 *
 *  Created on: 11/10/2010
 *      Author: Edwin Miguel
 */

#ifndef DECODERCONTROL_H_
#define DECODERCONTROL_H_

#include <iostream>
#include <Msrs.h>
#include <Observer.h>
#include "QtSpeech.h"

using namespace std;

class QtSpeech;
class DecoderControl: public Observer
	{
private:
	Msrs* msrs;
	QtSpeech* myview;
public:
	DecoderControl(QtSpeech* newView);
	virtual ~DecoderControl();
	void initDecoder(string hmm, string lm, string dict);
	void attachObserver(Observer *observer);
	virtual void Update(Subject* subject);
	virtual void UpdateSentence(Subject* subject);
	};


#endif /* DECODERCONTROL_H_ */
