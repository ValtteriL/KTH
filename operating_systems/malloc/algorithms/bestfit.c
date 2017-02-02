/*
    malloc best fit algorithm
    It should work.
    TODO: This has no compile time errors,
          not tested if it works.

*/



void * mallocBestFit(size_t nbytes)
{
  Header *p, *prevp, *bestfitptr, *bestfitprevp;
  Header * morecore(unsigned);
  unsigned nunits;

  int fitFound = 0;                                   /* int to see if a fit has been found */

  if(nbytes == 0) return NULL;                        /* not changed  from orginal malloc */

  nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) +1;   /* not changed  from orginal malloc */

  if((prevp = freep) == NULL) {                       /* not changed  from orginal malloc */
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }


  for(p= prevp->s.ptr;  ; prevp = p, p = p->s.ptr) {    /* look through the list of free blocks */
    
    if(p->s.size >= nunits) {                           /* checks to see if a free block is big enough */

    if (fitFound == 0) {                                /* marks that at least one big enough block has been found, */
      fitFound = 1;                                     /* used for bestfit alogrithm. */
      bestfitptr = p;                                   /* sets the best current pointer to bestfit  */
      bestfitprevp = prevp;                             /* sets the previous pointer to the previous best fit pointer */
    }


    /* here we check if the current big enough free block is the best fit */
    /* if a best fit is found sets the bestfit pointer to that block */
    if (((p->s.size-nunits) < (p->s.size-nunits)) && (fitFound == 1)  && ((p->s.size-nunits) > 0) ) {
        bestfitptr = p;                                 /* sets the best current pointer to bestfit  */
        bestfitprevp = prevp;                           /* sets the previous pointer to the previous best fit pointer */
    }
    
      if (p->s.size == nunits)  {                        /* exact space has been found */
        prevp->s.ptr = p->s.ptr;                         /* unlink node from list */
        freep = prevp;
        return (void *)(p+1);                             /* returns directly */
    }
  }

    if((p == freep) && (fitFound == 1))  {                /* looked through whole list for best fit, best fit found */
      p = bestfitprevp->s.ptr;                            /* sets pointe p to point to the bestfitpointer found */

      p->s.size -= nunits;                                /* allocate tail end */
      p += p->s.size;
      p->s.size = nunits; 
      freep = prevp;
      return (void *)(p+1);
    } else if (p == freep)                                 /* wrapped around free list, no fit found */
      if((p = morecore(nunits)) == NULL)

      return NULL;                                          /* none left */
  }
}