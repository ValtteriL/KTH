#include <unistd.h>
#include <string.h> 
#include <errno.h> 
#include <sys/mman.h>
#include <stdio.h>
#include "malloc.h"

void *realloc(void *ptr, size_t nbytes)
{
	/* TODO: check that the blocks have been allocated using malloc (if needed) */	
	
	if(ptr == NULL) /* equivalent to malloc() */
	{ return malloc(nbytes); }
	
	if(nbytes == 0) /* equivalent to free() */
	{ return free(ptr); }
	
	Header *bp, p;
	unsigned nunits;
	
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) +1;
	bp = (Header *) ap - 1; /* point to block header */
	
	if(nunits == bp->s.size) /* reallocate to same size... */
	{return ptr;}
	
	if((p = malloc(nbytes)) == NULL) /* mallocate memory */
	{ return NULL; }
	
	if(nunits < bp->s.size) /* size was increased */
	{ memcpy(p, ptr, nbytes); }
	else /* size was decreased */
	{ memcpy(p, ptr, bp->s.size); }
	
	free(ptr);
	return (void*) p;
}