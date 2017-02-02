/* QuickfitSizelist viittaa siis koko blokin kokoon, nunits, kun header on mukana */

/* Initializations */
#if STRATEGY == 4
#define NLISTS 5 /* number of lists */
#define LOWESTPOWER 10 /* THIS IS THE SMALLEST SIZE OF BLOCK (NUNITS) PAGE SIZE */
static Header* QuickfitSizelist[NLISTS];
#endif

/* free */
	{
		Header *bp;
		int n;

		if(ap == NULL) return; /* Nothing to do */
		
	
	}



/* Malloc */
{
	int n;
	Header* SizelistMember;

	/* check the size to determine the index in the list */
	for(n=0; n<NLISTS; n++)
	{
		if(nunits <= pow(2,(LOWESTPOWER+n)))
		{
			break; /* n is the index */
		}
	}

	/* check if we can allocate that large blocks */
	if(n+1 > NLISTS)
	{
		fprintf(stderr, "Cannot allocate that large memory\n");
		return NULL;
	}

	if(QuickfitSizelist[n] == NULL)
	{
		if((SizelistMember = morecore(nunits)) == NULL)
		{ return NULL; }
		return (void*)SizelistMember+1;
	}

	/* There are blocks of needed size, take the first one */
	SizelistMember = QuickfitSizelist[n];
	QuickfitSizelist[n] = QuickfitSizelist[n]->s.ptr;
	return (void*)SizelistMember+1;
}




{
	/* QUICK-FIT */
	Header *sizeListMember;
	int n;

	if(nunits > pow(2,(LOWESTPOWER + NLISTS))) /* check if we can allocate that much */
	{ perror("Cannot allocate that large memory"); return NULL; }
	
	/* check the size */
	for(n=0; n<NLISTS; n++)
	{
		if(nunits <= pow(2,(LOWESTPOWER + n))) /* choose the smallest block that can be used */
		{ break; }
	}

	fprintf(stderr, "nunits = %d, pow(2,%d+%d) = %d",nunits,LOWESTPOWER,n,(int)pow(2,LOWESTPOWER+n)); /* DELETE THIS */

	/* check the entry in the list of different-sized blocks */
	if((sizeListMember = QuickfitSizeList[n]) == NULL) /* is the entry empty? */
	{
	/*##########################################*/
		fprintf(stderr, "\t...entry empty\n");
		if((sizeListMember = morecore(pow(2, LOWESTPOWER+n))) == NULL)
		{ return NULL; } /* none left */
		return (void *)(sizeListMember+1);
	/*##########################################*/ /* return kutsuu vitun freetä!!!!! */
	}
	
	else /* there is blocks of needed size, we take the first one */
	{
		fprintf(stderr, "\t...blocks available!\n");
		QuickfitSizeList[n] = sizeListMember->s.ptr;
		return (void *)(sizeListMember+1);
	}
}