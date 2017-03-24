#include "functions.h"

double speed = 0; // not currently used for anything

/* discard all tasks from this tasklist */
void trashAllTasks(struct tasklist** list)
{
    struct tasklist* tmp = *list;
    struct tasklist* tmp2;

    while(tmp) {
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2);
    }

    *list = NULL;
}

/* append task to global tasklist */
void appendTaskList(struct tasklist** list, int floor, int type)
{

    struct tasklist* head = *list;
    struct tasklist* head2 = *list;
    int isEmpty = 1;

    /* check if theres similar task already */
    if(head) {

        isEmpty = 0;

        while(head) {
            head2 = head;
            if(head->floor == floor && head->type == type)
                return; // already identical task in queue
            head = head->next;
        }

    }

    struct tasklist* tmptask = (struct tasklist*) malloc(sizeof(struct tasklist));
    tmptask->next = NULL;
    tmptask->floor = floor;
    tmptask->type = type;


    if(isEmpty)
        *list = tmptask;
    else
        head2->next = tmptask;
}

/* append task to specific elevator's tasklist */
void appendTaskListCabin(struct threaddata* threaddata, int floor, int type)
{
    double position = threaddata->position;

    struct tasklist* tmptask = (struct tasklist*) malloc(sizeof(struct tasklist));
    tmptask->next = NULL;
    tmptask->floor = floor;
    tmptask->type = type;


    struct tasklist* pointer = threaddata->taskList;
    struct tasklist* tmp = NULL;

    if(!pointer) {
        threaddata->taskList = tmptask;
        return;
    }

    /* have we gone by the floor already */
    if(position < floor) {
        while(pointer) {
            if(pointer->floor > floor) {
                if(!tmp) {
                    threaddata->taskList = tmptask;
                    tmptask->next = pointer;
                    return;
                } else {
                    tmp->next = tmptask;
                    tmptask->next = pointer;
                    return;
                }
            } else if (pointer-> floor == floor) {
                return; // duplicate!
            }
            tmp = pointer;
            pointer = pointer->next;
        }
    } else {
        while(pointer) {
            if(pointer->floor < floor) {
                if(!tmp) {
                    threaddata->taskList = tmptask;
                    tmptask->next = pointer;
                    return;
                } else {
                    tmp->next = tmptask;
                    tmptask->next = pointer;
                    return;
                }
            } else if (pointer->floor == floor) {
                return; // duplicate
            }
            tmp = pointer;
            pointer = pointer->next;
        }
    }

    // put as the last one
    tmp->next = tmptask;
}

/* send signal to every elevator thread that is not blocked */
void signalAllElevators(struct threaddata* thread_data_array, int noElevators)
{
    int i;
    for(i=0; i<noElevators; i++) {
        if(!thread_data_array[i].isStopped)
            pthread_cond_signal(thread_data_array[i].condsignal); // dont signal stopped ones
    }
}


int main(int argc, char* argv[])
{
    char *hostname;
    int rc, i, port, noElevators, elevator;
    void* status;
    struct tasklist* head;

    /* validate params */
    if (argc != 4) {
        printf("Defaulting to localhost, port 4711, 3 elevators\n");

        hostname = "localhost";
        port = 4711;
        noElevators = 3;

    } else {
        hostname = argv[1];
        port = atoi(argv[2]);
        noElevators = atoi(argv[3]);
    }

    /* init global task queue to NULL */
    globalTasks = NULL;

    /* We have one worker thread for each elevator*/
    pthread_t threads[noElevators];

    /* create data for all elevators */
    pthread_cond_t condsignal_array[noElevators]; // to send some signals to threads
    pthread_mutex_t mutex_array[noElevators];
    struct threaddata thread_data_array[noElevators]; // 1 of these is sent to each elevator

    /* Initialize and set thread detached attribute explicitly */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* init communication */
    initHW(hostname, port);

    /* init the mutex */
    if (pthread_mutex_init(&mutex, NULL) < 0) {
        perror("pthread_mutex_init");
        exit(1);
    }

    /* create threads (1 thread per elevator) */
    for(i=0; i<noElevators; i++) {
        if (pthread_cond_init(&condsignal_array[i], NULL) < 0 || pthread_mutex_init(&mutex_array[i], NULL) < 0) {
            perror("pthread_cond_init or pthread_mutex_init");
            exit(1);
        }

        thread_data_array[i].id = i+1;
        thread_data_array[i].isStopped = 0;
        thread_data_array[i].position = 0;
        thread_data_array[i].mutex = &mutex_array[i];
        thread_data_array[i].condsignal = &condsignal_array[i];
        thread_data_array[i].taskList = NULL;

        rc = pthread_create(&threads[i], &attr, worker, (void *)&thread_data_array[i]);
        if(rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* free some memory */
    pthread_attr_destroy(&attr);

    /* become event listener */
    EventType e;
    EventDesc ed;
    while (1) {
        e = waitForEvent(&ed);

        switch (e) {
            case FloorButton:

                /* try to assign the task to some elevator */
                elevator = assignToElevator(thread_data_array, noElevators, ed.fbp.floor, (int) ed.fbp.type);

                /* if there's no elevators that can take the task, append it to the global list */
                if(elevator == -1) {
                    pthread_mutex_lock(&mutex);
                    appendTaskList(&globalTasks, ed.fbp.floor, (int)ed.fbp.type); // append the task to list!
                    signalAllElevators(thread_data_array, noElevators); // signal all
                    pthread_mutex_unlock(&mutex);
                } else if(elevator == -2) {
                    printf("duplicate task!\n"); // do nothing, there's duplicate task
                } else {
                    pthread_cond_signal(&condsignal_array[elevator]); // signal the elevator about the new task
                }

#ifdef PRINTOUTS            
                head = globalTasks;
                printf("Global tasklist: ");
                while(head) {
                    printf("%d ->",head->floor);
                    head = head->next;
                }
                printf("\n");
#endif

                break;

            case CabinButton:
                pthread_mutex_lock(&mutex_array[ed.cbp.cabin - 1]);
                if(ed.cbp.floor == 32000) {
                    trashAllTasks(&thread_data_array[ed.cbp.cabin-1].taskList); // delete all tasks
                    thread_data_array[ed.cbp.cabin-1].isStopped = 1; // stop
                } else {
                    appendTaskListCabin(&thread_data_array[ed.cbp.cabin-1], ed.cbp.floor, 0); // append to list!
                    thread_data_array[ed.cbp.cabin-1].isStopped = 0; // no stopped
                }
                pthread_cond_signal(&condsignal_array[ed.cbp.cabin - 1]); // signal the thread
                pthread_mutex_unlock(&mutex_array[ed.cbp.cabin - 1]);


#ifdef PRINTOUTS
                head = thread_data_array[ed.cbp.cabin-1].taskList;
                printf("Elevator %d: ",ed.cbp.cabin);
                while(head) {
                    printf("%d ->",head->floor);
                    head = head->next;
                }
                printf("\n");
#endif

                break;

            case Speed:
                speed = ed.s.speed; // update speed
                break;

            case Error:
                printf("error: \"%s\"\n", ed.e.str);
                break;

            case Position:
                pthread_mutex_lock(&mutex_array[ed.cp.cabin - 1]);
                thread_data_array[ed.cp.cabin - 1].position = ed.cp.position; // update the position
                pthread_mutex_unlock(&mutex_array[ed.cp.cabin - 1]);
                break;
        }
    }

    /* join all threads when done */
    for(i=0; i<noElevators; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    /* exit */
    exit(0);
}
