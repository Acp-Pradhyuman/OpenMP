#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000 // Number of double precision values

int main() {
    double *arr = (double*)malloc(N * sizeof(double));
    double sum = 0.0;
    double start_time, end_time;

    // Initialize the array with large double precision values
    for (int i = 0; i < N; i++) {
        // You can use random values or simple values for testing
        arr[i] = i + 1;  // Array values from 1 to N (just an example)
    }

    // Variable to store the time with 1 thread for calculating speedup
    double time_with_1_thread;

    // Loop over different thread counts
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        start_time = omp_get_wtime();

        sum = 0.0;

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

        end_time = omp_get_wtime();

        // Capture the time with 1 thread for calculating speedup
        if (num_threads == 1) {
            time_with_1_thread = end_time - start_time;
        }

        double time_taken = end_time - start_time;
        double speedup = time_with_1_thread / time_taken;

        printf("Threads: %d, Time: %f seconds, Speedup: %f\n", 
            num_threads, time_taken, speedup);
        printf("Sum: %.15f\n", sum);
    }

    free(arr);
    return 0;
}