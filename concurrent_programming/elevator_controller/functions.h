#ifndef functions
#define functions

#define PRINTOUTS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#include "hardwareAPI.h"

/* global mutexes for function calls */
pthread_mutex_t mutex;

void *worker(void* p); 

/* struct for double linked tasklistt */
struct tasklist {
    struct tasklist *next; // next and previous tasks, NULL is first/last
    int floor; // which floor to go to
    int type;
}; 

/* struct that is passed to the threads */
struct threaddata {
    int id;
    int isStopped;
    double position;

    pthread_mutex_t* mutex;
    pthread_cond_t* condsignal; // for receiving signals!
    struct tasklist *taskList; // head of the tasklist
};

struct tasklist* globalTasks;

int assignToElevator(struct threaddata* threaddata, int noElevators, int floor, int direction);

#endif
