#include "shell.h"

/* prototypes */
void child_interrupt(int);
void kill_everyone(void);
int digenv(char *argv[], char* envp[]);

int main(int argc, char *argv[], char* envp[]){

	if(atexit(kill_everyone))/* kill all children if shell exits */
	{perror("atexit() installation failed"); exit(1);}

	struct rusage res_usage, old_res_usage;
	char userInput[255]; /* char array to hold user input */
	int status;
	
	if(-1 == (long int)sigset(SIGINT, SIG_IGN))
	{perror("SIGSET failed!"); exit(1);} /* ignore <ctrl-c> TODO: error checking */
	/* TODO: the interrupt will kill all processes that dont ignore it... will kill all bg processes for example */
	
#ifdef SIGNALDETECTION
	/* TODO - error checking */
	sigset(SIGCHLD, child_interrupt);
#endif
	
	while(1)
	{
		if(sigrelse(SIGCHLD)) /* Release child signals to see if they have changed state */
		{ perror("Error releasing signals"); exit(1);}
		if(sighold(SIGCHLD))
		{ perror("Error holding signals"); exit(1); }
	
		printf("> ");
		fgets(userInput,255,stdin); /* get user input from terminal */
		
		char *pos;
   		if ((pos=strchr(userInput, '\n')) != NULL) /* null the line feed character */
    		{*pos = '\0';}
    	
    		char* newArray[6] = {NULL};
   		int i = 0;   	
		char* token;
	
   		for(token = strtok(userInput, " ");  token != NULL; token = strtok(NULL, " ")) 
   		{
   			newArray[i++] = token; /* Split up char array of commands into char pointer array */
   		}
   				
   		if(newArray[0] == NULL)
   		{/* Do nothing, just line feed was entered */}
   		
   		else if(!strcmp(newArray[0], "exit")) /* exit */
   		{exit(0);} 
   		
   		else if(!strcmp(newArray[0], "cd")) /* cd */
   		{
   			if(-1 == chdir(newArray[1]))
   			{
   				if(newArray[1] != NULL)
   				{printf("%s : no such directory\n",newArray[1]);}
				chdir(getenv("HOME"));
   			}
   		}
   		
   		else
   		{	
   			char isBackground = 0;
   			for(i=0; i<5 && newArray[i] != NULL; i++)/* foreground? background? */
   			{if(!strcmp(newArray[i], "&")){isBackground = 1; newArray[i] = NULL;}}
   			
			pid_t pid = 1;
			pid_t statuspid;
			if((pid = fork()) == -1) /* Fork */
			{exit(EXIT_FAILURE);}
			
			if(pid == 0) /* child */
			{
				if(!isBackground)
				{sigset(SIGINT, SIG_DFL);} // TODO error checking
				
				if(!strcmp(newArray[0],"digenv")) /* digenv */
				{digenv(newArray,envp);}
				
				if(execvp(newArray[0], newArray) == -1)
				{perror("execvp failed"); exit(1);}
			}
			else /* parent */
			{				
				if(isBackground)
				{	
					printf(GREEN "Process with PID %ld started on background\n" RESET, (long int)pid);
				}
				else
				{
					printf(GREEN "Process with PID %ld started on foreground\n" RESET, (long int)pid);
					
					if(getrusage(RUSAGE_CHILDREN, &old_res_usage))/* getrusage comparison point */
					{perror("Error on getrusage"); exit(1);}
					
					statuspid = waitpid(pid, &status, 0); /* wait for the child (wait4 returns resource usage info) */
		 			if( -1 == statuspid )
 					{ perror( "waitpid failed unexpectedly" ); exit( 1 ); }
 					
 					if(getrusage(RUSAGE_CHILDREN, &res_usage))/* getrusage somehow */
 					{perror("Error on getrusage"); exit(1);}
 					
 					printf("PID %ld utime = %ld.%06lds stime= %ld.%06lds\n",
 					(long int)pid,
 					res_usage.ru_utime.tv_sec - old_res_usage.ru_utime.tv_sec,
 					res_usage.ru_utime.tv_usec - old_res_usage.ru_utime.tv_usec,
 					res_usage.ru_stime.tv_sec - old_res_usage.ru_stime.tv_sec,
 					res_usage.ru_stime.tv_usec - old_res_usage.ru_stime.tv_usec);
				}
			}
   		}
   		
#ifndef SIGNALDETECTION
		pid_t bgpid = 1;
		while(bgpid && bgpid != -1)
		{
   			bgpid = waitpid(0, &status, WNOHANG); /* polling to check if any background process have terminated */
   			if(bgpid && bgpid != -1)
   			{
   			   	if(WIFEXITED(status))
   				{
   					int child_status = WEXITSTATUS(status);
		 			if( 0 != child_status ) /* child had problems */
 					{fprintf( stderr, RED "Child (pid %ld) failed with exit code %d\n" RESET, (long int)bgpid, child_status);}
 					else {printf(YELLOW "Process with PID %ld terminated\n" RESET, (long int)bgpid);}
   				}
   				else
   				{
   					if(WIFSIGNALED(status)) /* child was terminated with signal */
					{
						int child_signal = WTERMSIG(status);
			 			fprintf( stderr,YELLOW "Child (pid %ld) was terminated by signal no. %d\n" RESET, (long int)bgpid, child_signal );
 					}
   				}
   			}
   		}
#endif
	}
}

/* child interrupt handler */
void child_interrupt(int a)
{
	int status;
	pid_t pid = 1;
	
	while(pid && pid != -1)
	{
		pid = waitpid(0, &status, WNOHANG);
		if(pid && pid != -1) /* wait for the child */
		{	 
			if(WIFEXITED(status))
   			{
   				int child_status = WEXITSTATUS(status);
 				if( 0 != child_status ) /* child had problems */
 				{fprintf( stderr, RED "Child (pid %ld) failed with exit code %d\n" RESET, (long int)pid, child_status);}
 				else {printf(YELLOW "Process with PID %ld terminated\n" RESET ,(long int)pid);}
   			}
   			else
   			{
   				if(WIFSIGNALED(status)) /* child was terminated with signal */
				{
					int child_signal = WTERMSIG(status);
			 		fprintf( stderr, YELLOW "Child (pid %ld) was terminated by signal no. %d\n" RESET, (long int)pid, child_signal );
 				}
   			}
   		}
   	}
}

/* atexit */
void kill_everyone(void)
{
	kill(-getpid(), SIGINT); /* Send interrupt signal to children to terminate them, could send SIGTERM aswell */
}