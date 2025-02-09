#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000 // Number of double precision values

// Function to perform vector addition
void vector_addition(double *a, double *b, double *result) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result[i] = a[i] + b[i];
    }
}

// Function to perform vector multiplication
void vector_multiplication(double *a, double *b, double *result) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result[i] = a[i] * b[i];
    }
}

int main() {
    // Dynamically allocate memory for vectors
    double *a = (double*)malloc(N * sizeof(double));
    double *b = (double*)malloc(N * sizeof(double));
    double *result_add = (double*)malloc(N * sizeof(double));
    double *result_mul = (double*)malloc(N * sizeof(double));

    // Check if memory allocation is successful
    if (a == NULL || b == NULL || result_add == NULL || result_mul == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    double start_time, end_time;
    double time_with_1_thread_add, time_with_1_thread_mul;

    // Initialize the arrays with random double precision values (for illustration, from 1 to N)
    for (int i = 0; i < N; i++) {
        a[i] = (double)(i + 1);  // Array 'a' values from 1 to N
        b[i] = (double)(N - i);  // Array 'b' values from N to 1
    }

    printf("Performing Vector Addition and Vector Multiplication\n\n");

    // Loop over different thread counts
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        // Vector addition timing
        start_time = omp_get_wtime();
        vector_addition(a, b, result_add);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_add = end_time - start_time;
        }

        double time_taken_add = end_time - start_time;
        double speedup_add = time_with_1_thread_add / time_taken_add;

        // Printing result for vector addition
        printf("Vector Addition - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_add);
        printf("Speedup: %f\n", speedup_add);

        // Print the last element to verify
        printf("Sum of result: %.15f\n\n", result_add[N-1]);

        // Vector multiplication timing
        start_time = omp_get_wtime();
        vector_multiplication(a, b, result_mul);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_mul = end_time - start_time;
        }

        double time_taken_mul = end_time - start_time;
        double speedup_mul = time_with_1_thread_mul / time_taken_mul;

        // Printing result for vector multiplication
        printf("Vector Multiplication - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_mul);
        printf("Speedup: %f\n", speedup_mul);

        // Print the last element to verify
        printf("Result at last index: %.15f\n\n", result_mul[N-1]);
    }

    // Free allocated memory
    free(a);
    free(b);
    free(result_add);
    free(result_mul);

    return 0;
}