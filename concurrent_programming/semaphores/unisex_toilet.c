#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/time.h>
#define SHARED 1

sem_t queue, toilet, sexcount; /* semaphores for queue, toilet and sexcount respectively */
int sex[2] = {0}; /* array for amount of women (sex[0]) and men (sex[1]), protected by sexcount semaphore */

/* structure used to pass data to threads */
struct worker_data {
    int sex;
    int noToiletVisits;
    double totalTimeWaited;
    double longestWait;
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

/* signal handler to tell workers that the day is over */
void worktime_over(int sig)
{
    pthread_exit(NULL);
}

/* this could be the work which is done by threads */
void* work(void* workdata)
{
    struct worker_data* my_data;
    my_data = (struct worker_data *) workdata;

    /* work, by sleeping and going to toilet.. */
    while(1) {
        sleep(rand() % 4); // sleep random amount

        sem_wait(&queue); // goto toilet queue

        sem_wait(&sexcount); // acquire the counter of ppl in toilet
        
        if(sex[my_data->sex] == 0) {
            sem_post(&sexcount); // release sexcount, otherwise causes deadlock as the one in toilet wants it!

            double start_time = read_timer();
            sem_wait(&toilet); // no same sex ppl in toilet, wait...
            double wait_time = read_timer() - start_time;

            my_data->totalTimeWaited += wait_time; // update total wait time

            if(wait_time > my_data->longestWait)
                my_data->longestWait = wait_time;  // update longest wait if necessary

            my_data->noToiletVisits++; // increment no of toilet visits

            sem_wait(&sexcount); // get the sexcount again
        } else {
            if(my_data->sex)
                printf("I, man can go when there's %d women and %d men in the toilet\n",sex[0],sex[1]);
            else
                printf("I, woman can go when there's %d women and %d men in the toilet\n",sex[0],sex[1]);
        }

        sex[my_data->sex]++; // increase my gender count in toilet

        sem_post(&queue); // release queue
        sem_post(&sexcount); // release number of ppl in toilet

        sleep(rand() % 2); // sleep random amount in toilet

        sem_wait(&sexcount);
        sex[my_data->sex]--; // decrease my gender count in toilet

        if(sex[my_data->sex] == 0)
            sem_post(&toilet); // let anyone come to toilet
        
        sem_post(&sexcount); // release list of ppl in toilet
    }
}

int main(int argc, char* argv[])
{
    /* validate parameters */
    if(argc != 2) {
        printf("Usage: %s <no of workers>\n", argv[0]);
        exit(0);
    }

    time_t t;
    srand((unsigned) time(&t)); /* seed rand */
    if(signal(2, worktime_over) == SIG_ERR) { /* register signal handler */
        perror("Error registering signal handler: ");
        exit(-1);
    }

    int noThreads = atoi(argv[1]);
    pthread_t threads[noThreads]; /* array to store all workers */
    pthread_attr_t attr;
    struct worker_data thread_data_array[noThreads]; /* array to store all worker's data */
    int rc, i;
    void *status;

    /* initialize semaphores - they should all be initially 1 */
    sem_init(&queue, SHARED, 1);
    sem_init(&sexcount, SHARED, 1);
    sem_init(&toilet, SHARED, 1);
    
    /* Initialize and set thread detached attribute explicitly */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* generate workers */
    for (i=0; i<noThreads; i++) 
    {
        /* assemble the data that is given to the thread */
        thread_data_array[i].sex = rand() % 2; // random between 0 and 1
        thread_data_array[i].noToiletVisits = 0;
        thread_data_array[i].totalTimeWaited = 0;
        thread_data_array[i].longestWait = 0;

        /* create thread */
        rc = pthread_create(&threads[i], &attr, work, (void *)&thread_data_array[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr); /* free this attribute */
    
    /* let workers work */
    printf("Work day begins, let me sleep a bit...\n");
    sleep(10);
    printf("Work day over! Signaling workers to stop working now...\n");
    for (i=0; i<noThreads; i++) 
        pthread_kill(threads[i], 2); /* signal that the working day is over */

    int totalToiletVisits = 0;
    double longestWait = 0;
    int longestWaiter = 0;

    /* close the department, join the threads */
    for(i=0; i<noThreads; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        /* print individual results */
        printf("---------------------\n");
        if(thread_data_array[i].sex)
            printf("Results worker %d, (man)\n",i);
        else
            printf("Results worker %d, (woman)\n",i);
        printf("Went to toilet %d times\n", thread_data_array[i].noToiletVisits);
        printf("Waited a total of %f\n",thread_data_array[i].totalTimeWaited);
        printf("Longest wait %f\n",thread_data_array[i].longestWait);
        
        if(longestWait < thread_data_array[i].longestWait) {
            longestWait = thread_data_array[i].longestWait;
            longestWaiter = i;
        }
        totalToiletVisits += thread_data_array[i].noToiletVisits;
    }

    /* print results */
    printf("---------------------\n");
    printf("TOTAL toilet visits: %d\n", totalToiletVisits);
    printf("Longest wait: %f by worker number %d\n", longestWait, longestWaiter);

    /* over and out */
    exit(0);
}
