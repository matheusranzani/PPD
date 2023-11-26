#include <stdio.h>
#include <omp.h>

#define SIZE 1000 // Reduzi o tamanho para facilitar a visualização

int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

void multiplyMatrix() {
    #pragma omp parallel for shared(A, B, C)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void printMatrix(int matrix[SIZE][SIZE], char* name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Inicializando as matrizes A e B com valores aleatórios
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
        }
    }

    // printMatrix(A, "A");
    // printMatrix(B, "B");

    double start_time = omp_get_wtime();

    multiplyMatrix();

    double end_time = omp_get_wtime();
    printf("Tempo de execucao: %f segundos\n\n", end_time - start_time);

    // printMatrix(C, "C");

    return 0;
}
