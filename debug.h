#ifndef DEBUG_H
#define DEBUG_H
#include <stdarg.h>

#define LOG_FILE "log.txt"

// define different debug level as bits in msglevel, so one
// can compose debug listings
enum {D_BASE        = 1,      // things from main.c
      D_AI          = 2,      // AI things
      D_ARENA       = 4,      // playground
      D_SNAKE       = 8,        
      D_MENU        = 16,     
      //D_UNUSED      = 32,
      D_ERROR       = 64,     // for race conditions
      D_EXPERIMENTAL= 128,    // for experiments
      D_ALL=255
};

#if defined(NDEBUG) && defined(__GNUC__)
/* gcc's cpp has extensions; it allows for macros with a variable number of
   arguments. We use this extension here to preprocess pmesg away. */
#define pmesg(level, format, args...) ((void)0)
#else
void pmesg(int level, char *format, ...);
/* print a message, if it is considered significant enough.
      Adapted from [K&R2], p. 174 */
#endif

#endif /* DEBUG_H */

