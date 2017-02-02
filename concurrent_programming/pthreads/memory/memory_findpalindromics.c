#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>


double start_time, end_time; /* start and end times */
pthread_mutex_t palindromicsMutex; /* mutex for writing the file palindromics*/
pthread_mutex_t wordsMutex; /* mutex for altering position in words array */
long amountWords; /* amount on words in the words file */
long position; /* current position in words array*/

/* structure used to pass data to threads */
struct thread_data {
    long noPalindromes;
    char** words;
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

/* count lines in file */
long countlines(FILE* fp)
{
    long lines = 0;
    int ch;

    while (EOF != (ch=fgetc(fp)))
        if (ch=='\n')
            ++lines;

    rewind(fp);
    return lines;
}

/* worker */
void* work(void *threadarg) 
{
    struct thread_data* my_data;
    my_data = (struct thread_data *) threadarg;

    while(1) { 

        char* word;
        long i;

        /* start of critical section, use mutex */
        pthread_mutex_lock(&wordsMutex);
        if(position < amountWords) {
            word = *(my_data->words+position); 
            position++;
        } else {
            pthread_mutex_unlock(&wordsMutex); // end of file, unlock for others!
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&wordsMutex);
        /* end of critical section */

        /* make a copy of the string and reverse it */
        char* copy = malloc(strlen(word)+1);
        strcpy(copy, word);
        reverseStr(copy);

        //long i;
        for(i=0; i<amountWords; i++) {
            if(!strcasecmp(copy, *(my_data->words+i))) {
                // match found!
                my_data->noPalindromes += 1;

                /* start of critical section, use mutex */
                pthread_mutex_lock(&palindromicsMutex);
                fprintf(my_data->palindromicsfd, "%s\n", word); // write to the file of palindromics
                pthread_mutex_unlock (&palindromicsMutex);
                /* end of critical section */

                break;
            }
        }
        free(copy); // free the allocated memory
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
    int rc;
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

    /* create array for all words in the file */
    amountWords = countlines(wordsFile); // count lines in the file first
    position = 0; // in the beginning the position is 0
    char* words[amountWords];

    ssize_t read;
    char* line = NULL;
    size_t len = 0;
    long i = 0;

    /* read whole file into heap */
    while((read = getline(&line, &len, wordsFile)) != -1) {
        line[read-1] = 0; // eliminate line break
        words[i] = line; 
        i++;
        line = NULL;
        len = 0;
    }

    /* close the file */
    fclose(wordsFile);

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
        thread_data_array[i].words = &words[0];
        thread_data_array[i].palindromicsfd = fd;

        /* create thread */
        rc = pthread_create(&threads[i], &attr, work, (void *)&thread_data_array[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Free attribute and wait for the threads */
    pthread_attr_destroy(&attr);
    long noPalindromics = 0;

    /* join the threads */
    for(i=0; i<noThreads; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Thread %ld found %ld palindromics\n",i,thread_data_array[i].noPalindromes);
        noPalindromics += thread_data_array[i].noPalindromes;
    }
    
    /* get end time */
    end_time = read_timer();

    /* close the palindromics file */
    fclose(fd);

    /* free allocated memory */
    for (i=0; i<amountWords; i++) {
        free(words[i]);
    }

    /* print results & exit*/
    printf("Found total of %ld palindromics\n", noPalindromics);
    printf("The execution time is %g sec\n", end_time - start_time);
    exit(0);
}
