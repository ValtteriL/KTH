#ifndef MALLOC_INCLUDE
#define MALLOC_INCLUDE

#include <stdio.h>

/* Prototypes */
void * malloc(size_t nbytes);
void free(void * ap);
void *realloc(void *ptr, size_t nbytes);

#endif