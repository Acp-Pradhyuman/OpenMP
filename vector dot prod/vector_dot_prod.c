#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000 // Number of double precision values

// Function to perform dot product
double dot_product(double *a, double *b) {
    double result = 0.0;

    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < N; i++) {
        result += a[i] * b[i];
    }

    return result;
}

// Function to perform dot product using critical section
double dot_product_critical(double *a, double *b) {
    double result = 0.0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double temp = a[i] * b[i];

        #pragma omp critical
        {
            result += temp;
        }
    }

    return result;
}

int main() {
    // Dynamically allocate memory for vectors
    double *a = (double*)malloc(N * sizeof(double));
    double *b = (double*)malloc(N * sizeof(double));

    // Check if memory allocation is successful
    if (a == NULL || b == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    double start_time, end_time;
    double time_with_1_thread_dot, time_with_1_thread_critical;

    // Initialize the arrays with random double precision values (for illustration, from 1 to N)
    for (int i = 0; i < N; i++) {
        a[i] = (double)(i + 1);  // Array 'a' values from 1 to N
        b[i] = (double)(N - i);  // Array 'b' values from N to 1
    }

    printf("Performing Dot Product Calculation\n\n");

    // Loop over different thread counts
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        // Dot product without critical section timing
        start_time = omp_get_wtime();
        double result_dot = dot_product(a, b);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_dot = end_time - start_time;
        }

        double time_taken_dot = end_time - start_time;
        double speedup_dot = time_with_1_thread_dot / time_taken_dot;

        // Printing result for dot product without critical section
        printf("Dot Product (without critical section) - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_dot);
        printf("Speedup: %f\n", speedup_dot);

        // Print the result for verification
        printf("Dot product result: %.15f\n\n", result_dot);

        // Dot product with critical section timing
        start_time = omp_get_wtime();
        double result_dot_critical = dot_product_critical(a, b);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_critical = end_time - start_time;
        }

        double time_taken_critical = end_time - start_time;
        double speedup_critical = time_with_1_thread_critical / time_taken_critical;

        // Printing result for dot product with critical section
        printf("Dot Product (with critical section) - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_critical);
        printf("Speedup: %f\n", speedup_critical);

        // Print the result for verification
        printf("Dot product result with critical section: %.15f\n\n", result_dot_critical);
    }

    // Free allocated memory
    free(a);
    free(b);

    return 0;
}