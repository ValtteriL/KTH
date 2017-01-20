/*
 * gcc text.c -pthread -o test -Wall
 */

/*
 * create new file for palindromics (sync writing to this with mutex)
 *
 * open words file
 * for each word
 *     if palindromic
 *         put into another file
 *         counter++ (in each thread)
 * close new file
 * close words file
 * print total number of palindromic words
 * print palindromic words found by each thread
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

double start_time, end_time; /* start and end times */
pthread_mutex_t palindromicsMutex; /* mutex for the file of palindromics*/

struct thread_data {
    int thread_id;
    int noPalindromes;
};

/* timer */
double read_timer() {
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

/* worker */
void* work(void *threadarg) 
{
    /* open words for reading */
    FILE* fd = fopen("words","r");
    if (!fd) { 
        perror("Error opening file in thread: "); 
        pthread_exit(NULL); 
    }

    /* 
     * open da file
     *
     * wait for string.. (or some signal of termination, in which case we close the file)
     * reverse the string 
     * read file and try to find the string
     *       found? counter++, write to another file?
     *       EOF? nuthin
     * get next string
     *
     */

    struct thread_data my_data;
    my_data = *(struct thread_data *) threadarg;
    int thread_id = my_data.thread_id;
    int noPalindromes = my_data.noPalindromes;
    printf("Hello World! It's me, thread id %d with %d palindromes!\n",thread_id, noPalindromes);
    
    fclose(fd);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    /* validate parameters */
    if (argc != 3 || strncmp("-w", argv[1], 2)) {
        printf("Usage: %s -w <number of worker processes>\n", argv[0]);
        exit(0);
    }

    /* variable definitions */
    int noThreads = atoi(argv[2]);
    pthread_t threads[noThreads];
    pthread_attr_t attr;
    struct thread_data thread_data_array[noThreads];
    int i;
    int rc;
    void *status;

    /* create file for palindromics */
    FILE* fd = fopen("palindromics", "w");

    /* Initialize and set thread detached attribute explicitly */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* get start time */
    start_time = read_timer();

    /* create threads */
    for (i=0; i<noThreads; i++) 
    {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].noPalindromes = 0;
        rc = pthread_create(&threads[i], &attr, work, (void *)&thread_data_array[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }


    /*
     * ###############################################
     */

    /* open words file for reading */
    FILE* wordsFile = fopen("words", "r");
    if(!wordsFile) {
        perror("Error opening file in master: ");
        exit(-1);
    }

    /* for each word, deal the words to threads */


    /* close words file */
    fclose(wordsFile);

     /* 
     * ###############################################
     */


    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);

    /* join the threads? */
    for(i=0; i<noThreads; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %d having a status of %ld\n",i,(long)status);
    }

    /* close the palindromics file */
    fclose(fd);

    /* get end time */
    end_time = read_timer();

    /* print results & exit*/
    printf("The execution time is %g sec\n", end_time - start_time);
    exit(0);
}
