#include "functions.h"


int cmpfunc (const void * a, const void * b)
{
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;  
}


// check if there's already similar task in queue
int checkForDuplicate(struct threaddata* thread_data_array, int noElevators, int floor, int direction)
{
    int i;
    for(i=0; i<noElevators; i++) {
        pthread_mutex_lock(thread_data_array[i].mutex);

        struct tasklist* task = thread_data_array[i].taskList;
        while(task != NULL) {
            if(task->floor == floor && task->type == direction) {

                /* duplicate found */
                pthread_mutex_unlock(thread_data_array[i].mutex);
                return 1;
            }

            task = task->next;
        }

        pthread_mutex_unlock(thread_data_array[i].mutex);
    }

    return 0;
}

/* sort elevators by distance from given floor */
int assignToElevator(struct threaddata* threaddata, int noElevators, int floor, int direction)
{

    /* check for duplicates */
    if(checkForDuplicate(threaddata, noElevators, floor, direction))
        return -2;

    int i, j, gotAssigned = 0;
    double arrayvalues[noElevators];

    /* calculate the distance to the floor */
    for(i=0; i<noElevators; i++) {
        pthread_mutex_lock(threaddata[i].mutex);
        double distance = fabs(threaddata[i].position - floor);
        pthread_mutex_unlock(threaddata[i].mutex);
        arrayvalues[i] = distance;
    }

    /* sort to ascending order */
    qsort(arrayvalues, noElevators, sizeof(arrayvalues[0]), cmpfunc);

    /* check if the first one is available or going by the floor to the same direction */
    for(i=0; i<noElevators; i++) {

        double distance = arrayvalues[i];

        for(j=0; j<noElevators; j++) {

            pthread_mutex_lock(threaddata[j].mutex);
            if(fabs(threaddata[j].position - floor) == distance) {
                // this is the elevator with that distance

                /* check if stopped - dont assign to stopped elevators */
                if(!threaddata[j].isStopped) {

                    if(threaddata[j].taskList == NULL) {

                        /* no tasks in the list, assign the task to this elevator */
                        struct tasklist* tmptask = (struct tasklist*) malloc(sizeof(struct tasklist));
                        tmptask->next = NULL;
                        tmptask->floor = floor;
                        tmptask->type = direction;
                        threaddata[j].taskList = tmptask;
                        gotAssigned = 1;

                    } else {

                        /* if the elevator is going by the floor to that direction, take it */
                        if(( direction == MotorUp && threaddata[j].position < floor && threaddata[j].taskList->floor > floor) || 
                                (direction == MotorDown && threaddata[j].position > floor && threaddata[j].taskList->floor < floor)) {

                            /* put as the first one */
                            struct tasklist* tmptask = (struct tasklist*) malloc(sizeof(struct tasklist));
                            tmptask->next = threaddata[j].taskList;
                            tmptask->floor = floor;
                            tmptask->type = direction;
                            threaddata[j].taskList = tmptask;
                            gotAssigned = 1;
                        }
                    }
                }
            }

            pthread_mutex_unlock(threaddata[j].mutex);

            if(gotAssigned)
                break;
        }
        if(gotAssigned)
            break;
    }

    // return the elevator the task got assigned to, otherwise -1
    if(gotAssigned)
        return j;

    return -1;
}
