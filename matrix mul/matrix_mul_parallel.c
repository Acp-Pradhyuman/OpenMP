#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000 // Size of NxN matrix (consider N sufficiently large, e.g., 10000)

// Function to perform matrix multiplication
void matrix_multiplication(double **a, double **b, double **result) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0.0;  // Initialize the element to zero before accumulation
            for (int k = 0; k < N; k++) {
                result[i][j] += a[i][k] * b[k][j];  // Perform the dot product
            }
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
    double **result_mul = allocate_matrix(N);

    // Check if memory allocation is successful
    if (a == NULL || b == NULL || result_mul == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    double start_time, end_time;
    double time_with_1_thread_mul;

    // Initialize the matrices with random double precision values (for illustration)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = (double)(i + j);  // Arbitrary initialization
            b[i][j] = (double)(N - i - j);  // Another arbitrary initialization
        }
    }

    printf("Performing Matrix Multiplication\n\n");

    // Loop over different thread counts
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        // Matrix multiplication timing
        start_time = omp_get_wtime();
        matrix_multiplication(a, b, result_mul);
        end_time = omp_get_wtime();

        if (num_threads == 1) {
            time_with_1_thread_mul = end_time - start_time;
        }

        double time_taken_mul = end_time - start_time;
        double speedup_mul = time_with_1_thread_mul / time_taken_mul;

        // Printing result for matrix multiplication
        printf("Matrix Multiplication - Threads: %d\n", num_threads);
        printf("Time: %f seconds\n", time_taken_mul);
        printf("Speedup: %f\n", speedup_mul);

        // Print the last element to verify
        printf("Sum of result (last element): %.15f\n\n", result_mul[N-1][N-1]);
    }

    // Free allocated memory
    free_matrix(a, N);
    free_matrix(b, N);
    free_matrix(result_mul, N);

    return 0;
}