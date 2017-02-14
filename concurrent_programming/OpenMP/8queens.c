#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

long position; // position in the memory blob of possibilities

/* validate solution (check diagonal lines) */
void validate(char* solution, long* counter)
{
    int i, j;
    for(i=0; i<8; i++) {
        for(j=i+1; j<8; j++) {
            if(solution[j] == solution[i]+(j-i)) {
                // invalid!
                return;
            } 
            else if (solution[j] == solution[i]-(j-i)) {
                //invalid
                return;
            }
            else {
                // valid!
            }
        }
    }

    // protect read/update
    #pragma omp atomic
    *counter += 1; // this solution passed all validation!
}


/* Function to swap values at two pointers */
void swap(char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* generate all permutations */
void permute(char *a, int l, int r, long* counter, char* possibilities)
{
    int i;
    if (l == r)
    {
        memcpy(possibilities+8*position, a, 8); // copy the solution to the memory
        position++; // increment position
    }
    else
    {
        for (i = l; i <= r; i++)
        {
            swap((a+l), (a+i));
            permute(a, l+1, r, counter, possibilities);
            swap((a+l), (a+i)); //backtrack
        }
    }
}

void prepermute(double* time, char *a, int l, int r, long* counter)
{
    /* allocate memory for all possible solutions */
    char* possibilities = malloc(sizeof(char)*8 * 8*7*6*5*4*3*2*1);

    /* position initially zero */
    position = 0;

    /* fill with solutions */
    permute(a,l,r,counter,possibilities);
    
    /* use threads to validate all solutions */
    long i;
    double t1 = omp_get_wtime();
#pragma omp parallel for
    for(i=0; i<position; i++)
    {
        validate(possibilities+i*8, counter);
    }

    /* take the time */
    *time = omp_get_wtime()-t1;

    /* free the allocated memory */
    free(possibilities);
}


/* generate possible solutions */
int main(int argc, char* argv[])
{
    /* number of threads is decided by openmp */
    /* for comparison we might want to set the number of threads */
    omp_set_dynamic(0);
    omp_set_num_threads(1);

    /* queens cannot be on same line -> generate all combinations of str, 8! alternatives */
    char str[] = "12345678";
    long amount = 0;

    double time;
    prepermute(&time, str, 0, strlen(str)-1, &amount);

    printf("Elapsed time: %g\n", time);

    printf("Solutions: %ld\n",amount);

    return 0;
}
