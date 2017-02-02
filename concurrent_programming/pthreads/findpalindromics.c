#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>


double start_time, end_time; /* start and end times */
pthread_mutex_t palindromicsMutex; /* mutex for writing the file palindromics*/
pthread_mutex_t wordsMutex; /* mutex for reading the file words */

struct thread_data {
    long noPalindromes;
    FILE* wordsfd;
    FILE* palindromicsfd;
};

/* timer */
double read_timer() 
{
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

/* reverse string */
void reverseStr(char* str) 
{
    if(str) {
        int i, length, last_pos;
        length = strlen(str);
        last_pos = length - 1;

        for(i=0; i<length/2; i++) {
            char tmp = str[i];
            str[i] = str[last_pos - i];
            str[last_pos - i] = tmp;
        }
    }
}

/* worker */
void* work(void *threadarg) 
{
    struct thread_data* my_data;
    my_data = (struct thread_data *) threadarg;
    
    /* open words for reading */
    FILE* fd = fopen("words","r");
    if (!fd) { 
        perror("Error opening file in thread: "); 
        pthread_exit(NULL); 
    }

    while(1) {
    
        ssize_t read, read2;
        char* line = NULL;
        char* line2 = NULL;
        size_t len = 0;
        size_t len2 = 0;

        pthread_mutex_lock (&wordsMutex);
        read = getline(&line, &len, my_data->wordsfd);
        pthread_mutex_unlock (&wordsMutex);

        if (read == -1) {
            fclose(fd); // close the file
            pthread_exit(NULL); // we're at the end of the file
        }

        line[read-1] = 0; // eliminate newline character
        reverseStr(line); // reverse the string

        while ((read2 = getline(&line2, &len2, fd)) != -1) {
            line2[read2-1] = 0;
            if (!strcasecmp(line, line2)) {

                my_data->noPalindromes += 1;
                // match found!
                reverseStr(line);
                pthread_mutex_lock (&palindromicsMutex);

                fprintf(my_data->palindromicsfd, "%s\n", line); // write to the file of palindromics

                pthread_mutex_unlock (&palindromicsMutex);
                rewind(fd); // set file position to beginning
                break;
            }
            /* cleanup */
            free(line2);
            line2 = NULL;
            len2 = 0;
        }
        rewind(fd); // set file position to beginning
        free(line); // free
    }
}

int main(int argc, char* argv[])
{
    /* validate parameters */
    if (argc != 2) {
        printf("Usage: %s <number of worker processes>\n", argv[0]);
        exit(0);
    }

    /* variable definitions */
    int noThreads = atoi(argv[1]);
    pthread_t threads[noThreads];
    pthread_attr_t attr;
    struct thread_data thread_data_array[noThreads];
    int i, rc;
    void *status;

    /* create file for palindromics */
    FILE* fd = fopen("palindromics", "w");
    if(!fd) {
        perror("Error opening palindromics file in master: ");
        exit(-1);
    }

    /* open words file for reading */
    FILE* wordsFile = fopen("words", "r");
    if(!wordsFile) {
        perror("Error opening words file in master: ");
        exit(-1);
    }

    /* Initialize and set thread detached attribute explicitly */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* get start time */
    start_time = read_timer();

    /* create threads */
    for (i=0; i<noThreads; i++) 
    {
        /* assemble the data that is given to the thread */
        thread_data_array[i].noPalindromes = 0;
        thread_data_array[i].wordsfd = wordsFile;
        thread_data_array[i].palindromicsfd = fd;

        /* create thread */
        rc = pthread_create(&threads[i], &attr, work, (void *)&thread_data_array[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    long noPalindromics = 0;

    /* join the threads */
    for(i=0; i<noThreads; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: thread %d found %ld palindromics\n",i,thread_data_array[i].noPalindromes);
        noPalindromics += thread_data_array[i].noPalindromes;
    }

    /* get end time */
    end_time = read_timer();

    /* close the palindromics and words file */
    fclose(wordsFile);
    fclose(fd);

    printf("Found total of %ld palindromics\n", noPalindromics);

    /* print results & exit*/
    printf("The execution time is %g sec\n", end_time - start_time);
    exit(0);
}
