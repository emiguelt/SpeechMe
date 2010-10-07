#include <stdio.h>
#include <string.h>
#if defined(WIN32) && !defined(GNUWINCE)
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include "pocketsphinx.h"

static ps_decoder_t *ps;
static cmd_ln_t *config;
static FILE* rawfd;

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

void pause(char * c){
	int move_on;
	printf("%s - press any key to continue", c);
	    move_on=getchar();
}

int main(int argc, char *argv[]){       
	config = cmd_ln_init(NULL, cont_args_def, TRUE, "-hmm", ".\\model\\hmm", "-dict", ".\\model\\dic.dic", "-lm", ".\\model\\lm.DMP", NULL);
	pause("revisar config");
	if(config == NULL){
		pause("config null");
		return 1;
	}
	pause("Revisar ps_init");
	ps = ps_init(config);
	    if (ps == NULL){
	    	pause("ps null");
	        return 1;
	    }
	    
    E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);
    
    
    
    pause("all fine");
	return 0;
}
