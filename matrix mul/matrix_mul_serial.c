#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000 // Set N to 10000 or larger for meaningful results

// Function for serial matrix multiplication
void matrix_multiplication_serial(double **a, double **b, double **result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    // Dynamically allocate memory for NxN matrices
    double **a = (double**)malloc(N * sizeof(double*));
    double **b = (double**)malloc(N * sizeof(double*));
    double **result = (double**)malloc(N * sizeof(double*));

    for (int i = 0; i < N; i++) {
        a[i] = (double*)malloc(N * sizeof(double));
        b[i] = (double*)malloc(N * sizeof(double));
        result[i] = (double*)malloc(N * sizeof(double));
    }

    // Initialize matrices with random values (for illustration purposes)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = (double)(i + j);  // Arbitrary initialization
            b[i][j] = (double)(N - i - j);  // Another arbitrary initialization
        }
    }

    printf("Performing Serial Matrix Multiplication\n");

    // Matrix multiplication
    double start_time = clock();
    matrix_multiplication_serial(a, b, result);
    double end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Time taken for Serial Matrix Multiplication: %f seconds\n", time_taken);

    // Print the last element for verification
    printf("Result at last element: %.15f\n", result[N-1][N-1]);

    // Free dynamically allocated memory
    for (int i = 0; i < N; i++) {
        free(a[i]);
        free(b[i]);
        free(result[i]);
    }
    free(a);
    free(b);
    free(result);

    return 0;
}