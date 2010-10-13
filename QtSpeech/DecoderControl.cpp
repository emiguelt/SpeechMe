/*
 * DecoderControl.cpp
 *
 *  Created on: 11/10/2010
 *      Author: Edwin Miguel
 */

#include "DecoderControl.h"
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>
#include <sphinxbase/cmd_ln.h>
#include <iostream>

using namespace std;

DecoderControl::DecoderControl()
	{
	// TODO Auto-generated constructor stub

	}

DecoderControl::~DecoderControl()
	{
	// TODO Auto-generated destructor stub
	}

void DecoderControl::initDecoder(string hmm, string lm, string dict){
	config = cmd_ln_init(NULL, cont_args_def, TRUE, "-hmm", hmm.data(), 
				"-dict", dict.data(), "-lm", lm.data(), NULL);
	if(config ==NULL){
		return;
	}
	ps = ps_init(config);
	if(ps == NULL){
		return;
	}
}
