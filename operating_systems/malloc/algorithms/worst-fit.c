/* WORST FIT */
Header *largestBlock = prevp;
for(p= prevp->s.ptr;  ; prevp = p, p = p->s.ptr) 
{
  	if(largestBlock->s.size < p->s.size) /* bigger block? */
  	{ largestBlock = p; }
  	
  	if(p == freep) /* wrapped around free list */
      	{ 
      		if(largestBlock->s.size >= nunits) /* big enough */
		{
			if(largestBlock->s.size == nunits) /* exactly */
			{ prevp->s.ptr = largestBlock->s.prt; }
			else /* we need to allocate */
			{
				largestBlock->s.size -= nunits;
				largestBlock += largestBlock->s.size;
				largestBlock->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1);
		}
		else /* no suitable found, ask OS for some */
		{
			if((p = morecore(nunits)) == NULL)
			{ return NULL; }
		}
	}
}