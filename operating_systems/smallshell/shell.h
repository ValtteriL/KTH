#ifndef HEADER
#define HEADER

//#define SIGNALDETECTION /* signals or polling? */

/* don't change these */
#define READ 0
#define WRITE 1

#define _XOPEN_SOURCE 500 /* for sigset, sigrelse, sighold */

/* Colors for output */
#define RESET   "\x1b[0m"
#define YELLOW  "\x1b[33m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>

#endif

