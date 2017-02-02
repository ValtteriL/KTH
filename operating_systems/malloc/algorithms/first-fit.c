/* FIRST FIT */
  for(p= prevp->s.ptr;  ; prevp = p, p = p->s.ptr) {
    if(p->s.size >= nunits) {                           /* big enough */
      if (p->s.size == nunits)                          /* exactly */
	prevp->s.ptr = p->s.ptr;
      else {                                            /* allocate tail end */
	p->s.size -= nunits;
	p += p->s.size;
	p->s.size = nunits;
      }
      freep = prevp;
      return (void *)(p+1);
    }
    if(p == freep)                                      /* wrapped around free list */
      if((p = morecore(nunits)) == NULL)
	return NULL;                                    /* none left */
  }