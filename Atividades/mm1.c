/*
** PPD / DC/UFSCar - Helio
** Programa : multiplicacao de matrizes
** Objetivo: paralelizacao om OpenMP
** Modificado por:
**  - Maria Luiza Edwards Cordeira, 802645
**  - Matheus Goulart Ranzani, 800278
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <omp.h> // Para usar a função omp_get_wtime()

#define MAX_THREADS 128 // Define o máximo de threads utilizadas

float *A, *B, *C;

int main(int argc, char *argv[]) {
  int lin_a, col_a, lin_b, col_b, lin_c, col_c;
  int i, j, k;

  printf("Linhas A: ");
  scanf("%d", &lin_a);
  printf("Colunas A / Linhas B: ");
  scanf("%d", &col_a);
  lin_b = col_a;
  printf("Colunas B: ");
  scanf("%d", &col_b);
  printf("\n");
  lin_c = lin_a;
  col_c = col_b;

  // Alocacao dinâmica das matrizes, com linhas em sequência
  A = (float *)malloc(lin_a * col_a * sizeof(float));
  B = (float *)malloc(lin_b * col_b * sizeof(float));
  C = (float *)malloc(lin_c * col_c * sizeof(float));

  // Atribucao de valores iniciais às matrizes
  srand(time(NULL));

  // Opa! Vai gerar valores aleatórios em paralelo?
  // Talvez seja o caso de usar rand_r() ao invés de rand()...

  int num_threads;

  // Paralelização da geração de valores aleatórios
  #pragma omp parallel private(num_threads)
  {
    num_threads = omp_get_num_threads();
    unsigned int seed = omp_get_thread_num() + 1; // Semente única para cada thread

    #pragma omp for
    for (int i = 0; i < lin_a * col_a; i++) {
      // Uso da função ran_r(), pois ela é thread-safe, ou seja, não gera problemas de concorrência
      A[i] = (float)rand_r(&seed) / (float)RAND_MAX;
    }

    #pragma omp for
    for (int i = 0; i < lin_b * col_b; i++) {
      B[i] = (float)rand_r(&seed) / (float)RAND_MAX;
    }
  }

  // calculo da multiplicacao

  // Qual/quais loop(s) paralelizar? Vale a pena paralelizar todos?
  /**
   * Os loops externo e intermediário podem ser paralelizados para melhorar o desempenho.
   * O loop interno (de índice k) realiza cálculos dependentes dos outros dois loops,
   * portanto paralelizá-lo pode não proporcionar ganhos significativos na questão
   * de desempenho e pode, inclusive, gerar problemas de concorrência.
  */

  // Qual é o efeito de fazer um parallel for em cada um dos fors abaixo?
  /**
   * Ao usar #pragma omp parallel for em cada um dos fors, cada thread paralela assumirá
   * uma parte do trabalho, dividindo as iterações desses loops entre elas. Isso acelera o
   * processamento, pois várias tarefas são realizadas simultaneamente por diferentes threads.
  */

  // É necessários sincronizar alguma operação, garantindo exclusão mútua?
  /**
   * Não há necessidade de garantir exclusão mútua porque cada thread estará trabalhando em
   * diferentes elementos da matriz C. Cada thread terá seu próprio conjunto de i e j, portanto,
   * não haverá conflito direto entre elas. Não há necessidade de proteger o acesso a elementos
   * específicos da matriz C neste caso.
  */

  // Uso da diretiva do OpenMP para paralelizar o loop externo
  #pragma omp parallel for private(i, j, k) shared(A, B, C)
  for (i = 0; i < lin_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * col_c + j] = 0;
      for (k = 0; k < col_a; k++) {
        C[i * col_c + j] += A[i * col_a + k] * B[k * col_b + j];
      }
    }
  }

  // Agora, paralelizando o loop intermediário
  for (i = 0; i < lin_c; i++) {
    #pragma omp parallel for private(j, k) shared(A, B, C)
    for (j = 0; j < col_c; j++) {
      C[i * col_c + j] = 0;
      for (k = 0; k < col_a; k++) {
        C[i * col_c + j] += A[i * col_a + k] * B[k * col_b + j];
      }
    }
  }

  // Por fim, paralelizando o loop interno
  for (i = 0; i < lin_c; i++) {
    for (j = 0; j < col_c; j++) {
      C[i * col_c + j] = 0;
      #pragma omp parallel for private(k) shared(A, B, C)
      for (k = 0; k < col_a; k++) {
        C[i * col_c + j] += A[i * col_a + k] * B[k * col_b + j];
      }
    }
  }

  return (0);
}
