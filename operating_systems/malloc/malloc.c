#ifndef STRATEGY /* if no strategy defined, use first fit */
#define STRATEGY 1
#endif

#include <unistd.h>
#include <string.h> 
#include <errno.h> 
#include <sys/mman.h>
#include "malloc.h"
#include <stdio.h>
#include "brk.h"

#define NALLOC 1024                                     /* minimum #units to request */

typedef long Align;                                     /* for alignment to long boundary */

union header {                                          /* block header */
  struct {
    union header *ptr;                                  /* next block if on free list */
    size_t size;                                      /* size of this block  - what unit? */ /* unit is Header */
  } s;
  Align x;                                              /* force alignment of blocks */
};

typedef union header Header;

static Header base;                                     /* empty list to get started */
static Header *freep = NULL;                            /* start of free list */

#if STRATEGY == 4

#ifndef NRQUICKLISTS /* default number of different sized blocks */
#define NRQUICKLISTS 5
#endif

#ifndef LOWESTPOWER /* default size of smallest block in quick-fit lists */
#define LOWESTPOWER 6
#endif

static Header * QuickfitSizelist[NRQUICKLISTS] = {NULL}; /* List of different sized blocks */
unsigned int get_index(size_t);

#endif

/* free: put block ap in the free list */
void free(void * ap)
{
	Header *bp, *p;
	if(ap == NULL) return;                                /* Nothing to do */
  	bp = (Header *) ap - 1;                               /* point to block header */
	
	#if STRATEGY == 4 /* if we use quick-fit, we need new kind of free */
	{
    		int n = get_index(bp->s.size); /* get index in the list */

    		if (n < NRQUICKLISTS) /* if the block is small enough, place it as the first one in the right entry */ 
    		{
        		bp->s.ptr = QuickfitSizelist[n];
        		QuickfitSizelist[n] = bp;
        		return;
    		}
    	}
	#endif
	{
  		for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    			if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      				break;                                            /* freed block at atrt or end of arena */

  		if(bp + bp->s.size == p->s.ptr) 
  		{                     						/* join to upper nb */
    			bp->s.size += p->s.ptr->s.size;
    			bp->s.ptr = p->s.ptr->s.ptr;
  		}
  		else
    		{ bp->s.ptr = p->s.ptr; }
  	
  		if(p + p->s.size == bp) 
  		{                             					/* join to lower nbr */
    			p->s.size += bp->s.size;
    			p->s.ptr = bp->s.ptr;
  		}
  		else
  		{ p->s.ptr = bp; }
  	
  		freep = p;
  	}
}

/* morecore: ask system for more memory */

#ifdef MMAP

static void * __endHeap = 0;

void * endHeap(void)
{
  if(__endHeap == 0) __endHeap = sbrk(0);
  return __endHeap;
}
#endif

static Header *morecore(unsigned nu)
{
  void *cp;
  Header *up;
#ifdef MMAP
  unsigned noPages;
  if(__endHeap == 0) __endHeap = sbrk(0);
#endif

  if(nu < NALLOC)
    nu = NALLOC;
#ifdef MMAP
  noPages = ((nu*sizeof(Header))-1)/getpagesize() + 1;
  cp = mmap(__endHeap, noPages*getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  nu = (noPages*getpagesize())/sizeof(Header);
  __endHeap += noPages*getpagesize();
#else
  cp = sbrk(nu*sizeof(Header));
#endif
  if(cp == (void *) -1){                                 /* no space at all */
    perror("failed to get more memory");
    return NULL;
  }
  up = (Header *) cp;
  up->s.size = nu;
  free((void *)(up+1));
  return freep;
}

#if STRATEGY == 4
Header * init_list(int index)
{
	size_t block_size = 1<<(index + LOWESTPOWER);
    	size_t size = block_size;
    	size_t nunits = block_size/sizeof(Header);
    	char * cp;
    	int i;
    	Header *p, *prev;

    	if (size < NALLOC)
        { size = NALLOC; }
        
        #ifdef MMAP
  	if(__endHeap == 0)
  	{ __endHeap = sbrk(0); }
  	
  	cp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  	__endHeap += size;

  	#else
  	cp = sbrk(size);
	#endif
    	
    	if (cp == (char *) -1)
        { return NULL; }

    	p = (Header *) cp;
    	prev = p;
    	QuickfitSizelist[index] = p;
    	
    	for (i = 1; i*block_size < size; ++i) /* Measure the block and update information accordingly */
    	{
        	p->s.size = nunits;
        	prev->s.ptr = p;
        	prev = p;
        	p += nunits;
    	}
    	
    	prev->s.ptr = p;
    	p->s.size = nunits;
    	p->s.ptr = NULL;
    	return QuickfitSizelist[index];
}

unsigned int get_index(size_t nunits)
{
    	size_t size = nunits * sizeof(Header);
    	int i;
    	for (i = 0; i < NRQUICKLISTS; ++i) 
    	{
        	if (size <= 1<<(i+LOWESTPOWER)) 
        	{ return i; }
    	}
    	return NRQUICKLISTS;
}
#endif
#if STRATEGY == 4 || STRATEGY == 1
Header *morecore_quick(size_t nunits) 
{
    	char *cp;
    	Header *up;

    	if (nunits < NALLOC) /* allocate at least blocks with size NALLOC */
        { nunits = NALLOC; }
    
        #ifdef MMAP
  	unsigned noPages;
  	if(__endHeap == 0)
  	{ __endHeap = sbrk(0); }
  	
  	noPages = (nunits * sizeof(Header)-1)/getpagesize() + 1;
  	cp = mmap(__endHeap, noPages*getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  	__endHeap += noPages*getpagesize();
  	#else
  	cp = sbrk(nunits * sizeof(Header));
	#endif
    
    	if (cp == (char *) -1) 
    	{ return NULL; } /* no space at all */

    	up = (Header *) cp;
    	up->s.size = nunits;
    	free((void *)(up+1));
    	return freep;
}

#endif

/* malloc */
void * malloc(size_t nbytes)
{
	Header *p;
  	Header *prevp;
  	Header * morecore(unsigned);
  	size_t nunits;

  	if(nbytes == 0) return NULL;

  	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) +1;

  	if((prevp = freep) == NULL) 
  	{
    		base.s.ptr = freep = prevp = &base;
    		base.s.size = 0;
  	} 

#if STRATEGY == 2
	/* WORST FIT */
	Header *largestBlock = NULL, *prevlargestBlock = NULL;
	unsigned largestBlockSize = 0;
	
	for(p= prevp->s.ptr;  ; prevp = p, p = p->s.ptr)
	{		
		if(p->s.size >= nunits) {           /* big enough */
			if(largestBlockSize < p->s.size) /* bigger block? */
  			{
				largestBlock = p;
				largestBlockSize = p->s.size;
				prevlargestBlock = prevp;			
			}
		}
	
  		if(p == freep) /* wrapped around free list */
      		{
      			if(largestBlock != NULL) break; 
			
			if((p = morecore(nunits)) == NULL)
			{ return NULL; }
		}
	}
	if(largestBlock->s.size == nunits) /* exactly */
	{ prevlargestBlock->s.ptr = largestBlock->s.ptr; }
	else /* we need to allocate */
	{
		largestBlock->s.size -= nunits;
		largestBlock += largestBlock->s.size;
		largestBlock->s.size = nunits;
	}
	freep = prevlargestBlock;
	return (void *)(largestBlock+1);

#elif STRATEGY == 3
	/* BEST FIT */
	int fitFound = 0;                                   /* int to see if a fit has been found */
  	Header *bestfitprevp;
  	for(p= prevp->s.ptr;  ; prevp = p, p = p->s.ptr) {    /* look through the list of free blocks */
    
    		if(p->s.size >= nunits) 
    		{                           /* checks to see if a free block is big enough */
    			if (fitFound == 0) 
    			{                                			/* marks that at least one big enough block has been found, */
      				fitFound = 1;                                     /* used for bestfit alogrithm. */
      				bestfitprevp = prevp;                             /* sets the previous pointer to the previous best fit pointer */
    			}

    			/* here we check if the current big enough free block is the best fit */
    			/* if a best fit is found sets the bestfit pointer to that block */
    			if (((p->s.size-nunits) < (p->s.size-nunits)) && (fitFound == 1)  && ((p->s.size-nunits) > 0) ) 
    			{
        			bestfitprevp = prevp;                           /* sets the previous pointer to the previous best fit pointer */
    			}
    
      			if (p->s.size == nunits)  
      			{                        				/* exact space has been found */
        			prevp->s.ptr = p->s.ptr;                         /* unlink node from list */
        			freep = prevp;
        			return (void *)(p+1);                             /* returns directly */
    			}
  		}

    		if((p == freep) && (fitFound == 1))  
    		{                					/* looked through whole list for best fit, best fit found */
      			p = bestfitprevp->s.ptr;                            /* sets pointe p to point to the bestfitpointer found */
      			p->s.size -= nunits;                                /* allocate tail end */
      			p += p->s.size;
      			p->s.size = nunits; 
      			freep = prevp;
      			return (void *)(p+1);
    		} else if (p == freep) {                                 		/* wrapped around free list, no fit found */
      			if((p = morecore(nunits)) == NULL)
      				{ return NULL; }                                          /* none left */
  		}
	}
#elif STRATEGY == 4
	/* QUICK FIT */	
	int index = get_index(nunits); /* get the index where the block belongs */

    	if (index < NRQUICKLISTS) /* check whether the block is small enough for quick fit */
    	{ 
        	if ((p = QuickfitSizelist[index]) == NULL) /* is there any blocks of this size? */
        	{
            		if ((p = init_list(index)) == NULL)
            		{ return NULL; }
        	}

        	QuickfitSizelist[index] = QuickfitSizelist[index]->s.ptr;
        	return (void *)(p+1);
    	}
    
    	/* too large block - lets use first fit! */
#endif
#if STRATEGY == 4 || STRATEGY == 1
	/* FIRST FIT */
    	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
    	{
        	if (p->s.size >= nunits) /* is the block large enough? */
        	{
    			if (p->s.size == nunits) 
    			{ 
    				prevp->s.ptr = p->s.ptr; /* exactly */
    			} 
    			else 
    			{
        			p->s.size -= nunits; /* we need to allocate */
        			p += p->s.size;
        			p->s.size = nunits;
    			}
    			freep = prevp;
    			return (void *)(p+1);
        	}
        	
        	if (p == freep) /* wrapped around free list - no large enough blocks */
        	{
            		if ((p = morecore(nunits)) == NULL) /* use reqular */
                	{ return NULL; } /* no more memory */
        	}
    	}
#endif

}

/* realloc */
void *realloc(void *ptr, size_t nbytes)
{	
	Header *bp, *p;
	unsigned nunits, oldsize;

	if(ptr == NULL) /* equivalent to malloc() */
	{ return malloc(nbytes); }
	
	if(nbytes == 0) /* equivalent to free() */
	{ free(ptr); return NULL; }
	
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) +1;
	bp = (Header *) ptr - 1; /* point to block header */
	oldsize = (bp->s.size - 1) * sizeof(Header);
	
	
	if(nunits == bp->s.size) /* reallocate to same size... */
	{return ptr;}
	
	if((p = malloc(nbytes)) == NULL) /* mallocate memory */
	{ return NULL; }
	
	memcpy(p, ptr, (nbytes < oldsize) ? nbytes : oldsize); /* copy the smaller amount of bytes to the new block */
	
	free(ptr);
	return (void*) p;
}
