#include <stdio.h>
#include <stdlib.h>
#include "common.h"


void dputs(char *str){
	// prints debug info
#ifdef DEBUG
	FILE *f_log;
	if ((f_log = fopen("log", "a+")) == NULL){
		fprintf(stderr, "cant create log file, exiting");
		exit(EXIT_FAILURE);
	}
	fprintf(f_log, "<debugg> %s\n", str);
	fclose(f_log);
#endif
}
