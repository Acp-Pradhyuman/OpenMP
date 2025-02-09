#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000 // Number of double precision values

int main() {
    double *arr = (double*)malloc(N * sizeof(double));
    double sum = 0.0;

    // Initialize the array with large double precision values
    for (int i = 0; i < N; i++) {
        // Random numbers between 1 and 1000
        // arr[i] = (double)(rand() % 1000 + 1); 
        arr[i] = i;
    }

    // Parallel region with critical section
    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for
        for (int i = 0; i < N; i++) {
            local_sum += arr[i];
        }

        #pragma omp critical
        {
            sum += local_sum;
        }
    }

    printf("Sum: %f\n", sum);

    free(arr);
    return 0;
}