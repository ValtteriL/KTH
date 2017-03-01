#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// shuffle array, taken from: http://benpfaff.org/writings/clc/shuffle.html
void shuffle(int* array, size_t n)
{
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void beMan(int* array, int len, int rank) 
{
    int i,j;
    int proposal = 1;
    int recbuf = 0;

    MPI_Request reqs[len*2]; // for sending and receiving
    MPI_Status stats[len*2];

    // for each number, from highest to lowest
    for (i=len-1; i>=0; i--) {
        for (j=0; j<len; j++) {
            if(array[j] == i) {

                // propose rank j
                MPI_Isend(&proposal, 1, MPI_INT, len+j, 1, MPI_COMM_WORLD, &reqs[i]);
                
                // get reply from j
                MPI_Irecv(&recbuf, 1, MPI_INT, len+j, 1, MPI_COMM_WORLD, &reqs[len+i]);
                MPI_Wait(&reqs[len+i], &stats[i]); // wait for reply - if woman says maybe we're waiting..
                
                if(recbuf) {
                    // wife found, stop. otherwise continue proposing women
                    printf("%d: I got a wife: %d!\n",rank,len+j);
                    return;
                }
            }
        }
    }
}


void beWoman(int* array, int len, int rank)
{
    int husbandpoints = -1;
    int husbandrank = -1;
    int no = 0;
    int yes = 1;
    int i;
    int noWomenMarried = 0;
    int j = 0;
    int recbuf = 0;

    MPI_Request reqs[len*2]; // for sending and receiving
    MPI_Status stats;
    MPI_Status stats2;


    // while all women are not married
    while(noWomenMarried != len) {
        
        int flag = 0; 
        
        // receive any message
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &stats);

        // if message received
        if(flag) {

            // acknowledge the message
            MPI_Recv(&recbuf, 1, MPI_INT, stats.MPI_SOURCE, stats.MPI_TAG, MPI_COMM_WORLD, &stats2);

            if(stats.MPI_TAG == 1) { // this is proposal msg

                if(array[stats.MPI_SOURCE] > husbandpoints) {

                    // get divorce
                    if(husbandrank != -1) {
                        MPI_Isend(&no, 1, MPI_INT, husbandrank, 1, MPI_COMM_WORLD, &reqs[j]);
                    } else {
                        // tell other women that I am married
                        for(i=0; i<len; i++) {
                            if(len+i != rank) // dont send to self!
                                MPI_Isend(&no, 1, MPI_INT, len+i, 2, MPI_COMM_WORLD, &reqs[len+i]);
                        }

                        noWomenMarried++; // increment the number of married women with self
                    }

                    husbandrank = stats.MPI_SOURCE; // update husband
                    husbandpoints = array[husbandrank];
                } else {

                    // the prososing man was worse
                    MPI_Isend(&no, 1, MPI_INT, stats.MPI_SOURCE, 1, MPI_COMM_WORLD, &reqs[j]); // reply no
                }
                j++;

            } else if(stats.MPI_TAG == 2) { // this is marriage msg

                noWomenMarried++; // increment number of married women
            }
        }
    }


    // tell the man that he's accepted
    MPI_Isend(&yes, 1, MPI_INT, husbandrank, 1, MPI_COMM_WORLD, &reqs[0]);
    MPI_Wait(&reqs[0], &stats); // wait till sent
}


int main(int argc, char *argv[]) {
    int  numtasks, rank;

    // initialize MPI  
    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&numtasks); // get number of tasks 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get my rank

    // seed rand with rank so every process gets different
    srand(time(NULL) + rank);

    // create array of integers 0-n
    int i;
    int array[numtasks/2]; // this is the smaller part if numtasks is odd
    for (i=0; i<numtasks/2; i++) {
        array[i] = i;
    }

    // shuffle the array
    shuffle(array, sizeof(array)/sizeof(int));


    // first half are men, others women
    if(rank < numtasks/2){
        beMan(array, sizeof(array)/sizeof(int), rank);
    } else {

        if(numtasks % 2 && rank == numtasks-1) {
            printf("I am rank %d, a woman who doesn't get partner!\n", rank);
        } else {
            beWoman(array, sizeof(array)/sizeof(int), rank);
        }
    }

    // done with MPI  
    MPI_Finalize();
}
