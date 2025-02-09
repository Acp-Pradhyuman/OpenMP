#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 10000 // Size of NxN matrix (consider N sufficiently large, e.g., 10000)

// Function to perform matrix addition
void matrix_addition(double **a, double **b, double **result) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

// Function to allocate a 2D matrix dynamically
double **allocate_matrix(int size) {
    double **matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (double *)malloc(size * sizeof(double));
    }
    return matrix;
}

// Function to free the dynamically allocated memory for a matrix
void free_matrix(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    // Dynamically allocate memory for matrices
    double **a = allocate_matrix(N);
    double **b = allocate_matrix(N);
    double **result_add = allocate_matrix(N);

    // Check if memory allocation is successful
    if (a == NULL || b == NULL || result_add == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    double start_time, end_time;
    double time_with_1_thread_add;

    // Initialize the matrices with random double precision values (for illustration)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = (double)(i + j);  // Arbitrary initialization
            b[i][j] = (double)(N - i - j);  // Another arbitrary initialization
        }
    }

    printf("Performing Matrix Addition\n\n");

    // Loop over different thread counts
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        // Matrix addition timing
        start_time = omp_get_wtime();
        matrix_addition(a, b, result_add);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_add = end_time - start_time;
        }

        double time_taken_add = end_time - start_time;
        double speedup_add = time_with_1_thread_add / time_taken_add;

        // Printing result for matrix addition
        printf("Matrix Addition - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_add);
        printf("Speedup: %f\n", speedup_add);

        // Print the last element to verify
        printf("Sum of result (last element): %.15f\n\n", result_add[N-1][N-1]);
    }

    // Free allocated memory
    free_matrix(a, N);
    free_matrix(b, N);
    free_matrix(result_add, N);

    return 0;
}