#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "debug.h"

int msglevel=D_BASE; /* the higher, the more messages... */

#if defined(NDEBUG) && defined(__GNUC__)
/* Nothing. pmesg has been "defined away" in debug.h already. */
#else
void pmesg(int level, char* format, ...) {
#ifdef NDEBUG
	/* Empty body, so a good compiler will optimise calls
	   to pmesg away */
#else
        va_list args;

        level &= msglevel;  // mask level with allowed debuging levels

        if (level){

           FILE *f_log;
           if ((f_log = fopen(LOG_FILE, "a")) == NULL){
              fprintf(stderr, "cant open log file in append mode, exiting.\n");
              exit(EXIT_FAILURE);
           }

           va_start(args, format);
           vfprintf(f_log, format, args);
           va_end(args);

           fclose(f_log);
        }

#endif /* NDEBUG */
#endif /* NDEBUG && __GNUC__ */
}
        
