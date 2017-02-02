#include "shell.h"

int digenv(char *argv[], char* envp[])
{	
	int argc;
	int pipa[2];
	int pipa2[2];
	int pipa3[2];
	int status, i;
	int nchildren = 0;
	
	for(argc=0; argv[argc] != 0; argc++)
	{/* count arguments */}
	
	/* Create pipes 1 and 2 */
	if(pipe(pipa) == -1)
	{exit(EXIT_FAILURE);}
	if(pipe(pipa2) == -1)
	{exit(EXIT_FAILURE);}
	
	/* Fork */
	pid_t pid = 1;
	if((pid = fork()) == -1)
	{exit(EXIT_FAILURE);}
	
	if(pid == 0) // CHILD (grep)
	{	
		if(-1 == dup2(pipa[READ],STDIN_FILENO)) /* redirect pipe1 to stdin */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
		
		if(-1 == dup2(pipa2[WRITE],STDOUT_FILENO)) /* redirect stdout to pipe2 */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa[WRITE])) /* Close write, we dont need it */
		{perror("Cannot close pipe (write-end)[GREP]"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa[READ])) /* Close read on pipe1 */
		{perror("Cannot close pipe (read-end)[GREP]"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa2[READ])) /* Close read on pipe2, we dont need it */
		{perror("Cannot close pipe2 (read-end)[GREP]"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa2[WRITE])) /* Close write on pipe2, we we have it on stdout now */
		{perror("Cannot close pipe2 (write-end)[GREP]"); exit(EXIT_FAILURE);}
  		
  		if(argc > 1) /* filter or no */
  		{
  			argv[0] = "grep";
  			(void) execvp( "grep", argv);
 			perror( "Cannot exec grep" ); exit( 1 );
 		}
 		exit(0);
	}

	/* PARENT */
	if(argc < 2) /* No arguments? */
	{
		if(-1 == dup2(pipa2[WRITE],pipa[WRITE])) /* Redirect stdin to pipe2 write (which is in STDOUT_FILENO) */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
	}
	
	//Send data to grep
	for(i=0; envp[i] != 0; i++)
	{
		write(pipa[WRITE],envp[i],strlen(envp[i]));
		write(pipa[WRITE],"\n",1);
	}
	
	/* Create 3rd pipe */
	if(pipe(pipa3) == -1)
	{exit(EXIT_FAILURE);}
	
	/* Close 1st pipe */
	if(-1 == close(pipa[WRITE]))
	{perror("Cannot close pipe (write-end)"); exit(EXIT_FAILURE);}
	if(-1 == close(pipa[READ]))
	{perror("Cannot close pipe (read-end)"); exit(EXIT_FAILURE);}
	/* Close write from second pipe */
	if(-1 == close(pipa2[WRITE]))
	{perror("Cannot close pipe (read-end)"); exit(EXIT_FAILURE);}
		
	/* Check that grep actually printed out something to sort */
	pid = wait( &status );
	if( -1 == pid )
 	{ perror( "wait() failed unexpectedly" ); exit( 1 ); }
 	if( WIFEXITED( status ) ) /* grep has completed*/
 	{
		int child_status = WEXITSTATUS( status );
	 	if( 0 != child_status ) /* grep had problems */
 		{fprintf( stderr, "Grep (pid %ld) failed with exit code %d. Did you write the parameters correctly?\n",(long int) pid, child_status); exit(1);}
 	}
	
	/* fork again */
	if((pid = fork()) == -1) /* ERROR */
	{exit(EXIT_FAILURE);}
	
	if(pid == 0) // CHILD (sort)
	{	
		if(-1 == dup2(pipa2[READ],STDIN_FILENO)) /* redirect pipe2 read to stdin */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
		
		if(-1 == dup2(pipa3[WRITE],STDOUT_FILENO)) /* redirect stdout to pipe3 write */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa2[READ])) /* Close write, we have it on stdin now */
		{perror("Cannot close pipe2 (read-end)[SORT]"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa3[READ])) /* Close read on pipe3, we dont need it */
		{perror("Cannot close pipe3 (read-end)[SORT]"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa3[WRITE])) /* Close write on pipe3, we have it on stdout now*/
		{perror("Cannot close pipe3 (write-end)[SORT]"); exit(EXIT_FAILURE);}
		
		(void) execlp( "sort", "", NULL );
 		perror( "Cannot exec sort" ); exit( 1 );
	}
	
	/* PARENT */
	if(-1 == close(pipa2[READ])) /* Close read from pipe2 */
	{perror("Cannot close pipe (read-end)[PARENT]"); exit(1);}
	if(-1 == close(pipa3[WRITE])) /* Close write from pipe3*/
	{perror("Cannot close pipe (write-end)[PARENT]"); exit(1);}
	
	nchildren++;
	
	/* one last fork */
	if((pid = fork()) == -1) /* ERROR */
	{exit(EXIT_FAILURE);}
	
	if(pid == 0) // CHILD (less)
	{
		if(-1 == dup2(pipa3[READ],STDIN_FILENO)) /* redirect pipe3 to stdin */
		{perror("Cannot dup"); exit(EXIT_FAILURE);}
		
		if(-1 == close(pipa3[READ])) /* Close read, we have it on stdin now */
		{perror("Cannot close pipe3 (read-end)[LESS]"); exit(EXIT_FAILURE);}
		
		(void) execlp( "less", "", NULL );
 		perror( "Cannot exec less" ); exit( 1 );
	}
	
	/* PARENT */
	if(-1 == close(pipa3[READ])) /* Close read from pipe3 */
	{perror("Cannot close pipe3 (read-end)[PARENT]"); exit(1);}
	
	nchildren++;
	
	/* Wait for children to finish */
	for(i=0; i<nchildren; i++)
	{
		pid = wait( &status );
		 if( -1 == pid )
 		{ printf("someone reaped my child already!\n"); } /* no children to wait */
 		
 		 if(WIFEXITED(status)) /* Childprocess finished */
 		{
			int child_status = WEXITSTATUS(status);
	 		if( 0 != child_status ) /* child had problems */
 			{fprintf( stderr, "Child (pid %d) failed with exit code %d\n", pid, child_status);}
 		}
 	
 		else
 		{
			if(WIFSIGNALED(status)) /* child was terminated with signal */
			{
				int child_signal = WTERMSIG(status);
		 		fprintf( stderr, "Child (pid %d) was terminated by signal no. %d\n", pid, child_signal );
 			}
 		}	
	}

	/* Exit */
	exit(EXIT_SUCCESS);
}
