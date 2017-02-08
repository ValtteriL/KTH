#include <stdio.h>
#include <string.h>
#include <omp.h>


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
#pragma omp critical
    // this solution passed all validation!
    // and critical section
    *counter += 1;
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
void permute(char *a, int l, int r, long* counter)
{
    #pragma omp parallel
    {
        #pragma omp single // executed by just one thread!
        {
            int i;
            if (l == r)
            {
                #pragma omp task // tasks are executed in parallel
                {
                    validate(a, counter);
                }
            }
            else
            {
                for (i = l; i <= r; i++)
                {
                    swap((a+l), (a+i));
                    permute(a, l+1, r, counter);
                    swap((a+l), (a+i)); //backtrack
                }
            }
        }
    }
}


/* generate possible solutions */
int main(int argc, char* argv[])
{
    /* number of threads is decided by openmp */
    /* for comparison we might want to set the number of threads */
    //omp_set_num_threads(4);

    /* queens cannot be on same line -> generate all combinations of str, 8! alternatives */
    char str[] = "12345678";
    long amount = 0;

    double t1, t2;
    t1 = omp_get_wtime();
    permute(str, 0, strlen(str)-1, &amount);
    
    t2 = omp_get_wtime();
    printf("Elapsed time: %g\n",t2-t1);
    
    printf("Solutions: %ld\n",amount);

    return 0;
}
