/*
 * DecoderControl.h
 *
 *  Created on: 11/10/2010
 *      Author: Edwin Miguel
 */

#ifndef DECODERCONTROL_H_
#define DECODERCONTROL_H_

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>
#include <sphinxbase/cmd_ln.h>
#include <iostream>

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

class DecoderControl
	{
private:
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE* rawfd;
public:
	DecoderControl();
	virtual ~DecoderControl();
	void initDecoder(string hmm, string lm, string dict);
	};


#endif /* DECODERCONTROL_H_ */
