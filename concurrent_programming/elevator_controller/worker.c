#include "functions.h"

/* control elevator */
void *worker(void* p) 
{
    struct threaddata *my_data = (struct threaddata *) p;


    while(1) {

        /* Wait for task */
        pthread_mutex_lock(my_data->mutex);
        while(!my_data->taskList) {

            // if no task, check global queue and take the first one
            pthread_mutex_lock(&mutex);
            if(!my_data->isStopped && globalTasks) {
                my_data->taskList = globalTasks;
                globalTasks = globalTasks->next;
                my_data->taskList->next = NULL; // dont follow these
                printf("%d: I took task from global queue! destination = %d\n",my_data->id, my_data->taskList->floor);
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);

            pthread_cond_wait(my_data->condsignal, my_data->mutex);
        }


        /* start moving to right direction */
        pthread_mutex_lock(&mutex);
        if(my_data->position < my_data->taskList->floor)
            handleMotor(my_data->id, MotorUp); // go upwards
        else 
            handleMotor(my_data->id, MotorDown); // go downwards
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(my_data->mutex);


        /* start listening for the position busy waiting */
        int isStopped = 0;
        while(!isStopped) {
            pthread_mutex_lock(my_data->mutex);

            pthread_mutex_lock(&mutex);
            handleScale(my_data->id, (int) (my_data->position + 0.05)); // update the scale
            pthread_mutex_unlock(&mutex);

            if(!my_data->isStopped) {
                if (!(my_data->position <= my_data->taskList->floor - 0.05 || my_data->position >= my_data->taskList->floor + 0.05)) {
                    pthread_mutex_unlock(my_data->mutex);
                    break; // right spot
                }
            }

            isStopped = my_data->isStopped;
            pthread_mutex_unlock(my_data->mutex);
        }

        /* stop elevator */
        pthread_mutex_lock(&mutex);
        handleScale(my_data->id, (int) (my_data->position + 0.05)); // update the scale
        handleMotor(my_data->id, MotorStop); // stop the elevator

        /* update the value */
        pthread_mutex_lock(my_data->mutex);
        isStopped = my_data->isStopped;
        pthread_mutex_unlock(my_data->mutex);
        
        if(!isStopped) {

            handleDoor(my_data->id, DoorOpen); // open the door
            pthread_mutex_unlock(&mutex);
            pthread_mutex_unlock(my_data->mutex);

            /* Close the doors after a moment */
            sleep(1);
            pthread_mutex_lock(&mutex);
            handleDoor(my_data->id, DoorClose);
            pthread_mutex_unlock(&mutex);
            sleep(1);

            /* next task? */
            pthread_mutex_lock(my_data->mutex);
            struct tasklist* donetask = my_data->taskList;
            my_data->taskList = my_data->taskList->next;
            free(donetask);

            /* if no next task check global queue */
            if(!my_data->taskList && globalTasks) {

                pthread_mutex_lock(&mutex);
                my_data->taskList = globalTasks;
                globalTasks = globalTasks->next;
                my_data->taskList->next = NULL; // dont follow these

#ifdef PRINTOUTS
                printf("%d: I took task from global queue! destination = %d\n",my_data->id, my_data->taskList->floor);
#endif

                pthread_mutex_unlock(&mutex);

            }

            pthread_mutex_unlock(my_data->mutex);
        } else {
            my_data->taskList = NULL;
            pthread_mutex_unlock(&mutex);
            pthread_mutex_unlock(my_data->mutex);
        }
    }

    pthread_exit(NULL);
}
