/*
 ** Universidade Federal de São Carlos
 ** Departamento de Computação
 ** Prof. Hélio Crestana Guardia
 ** Programação Paralela e Distribuída
 */

/*
 ** Programa : ordenação de vetor
 ** Comando: qsort() e mergesort() da glibc
 ** Objetivo:
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// tamanho default
#define LSIZE 1000

// lista de elementos
float *list;


void
init_vet( float * list, int list_size )
{
	int i;

	// não usar srand, caso queira gerar sempre os mesmos valores para comparações
	// srand(time(NULL));

	for( i=0; i < list_size ; i++ ) {
		list[i] = (float)rand() / (float)RAND_MAX;
	}
}

int
compare ( const void *e1, const void *e2 )
{
	float *i1 = (float*) e1;
	float *i2 = (float*) e2;

	return ((*i1 < *i2) ? -1 : (*i1 > *i2) ? +1 : 0);
}


void
print_array(float *list, int list_size)
{
	int i;

	for(i=0; i < list_size; i++) {
		printf("%f ", list[i]);
	}
	printf("\n");
}

int
main(int argc, char *argv[])
{
	int list_size = 0;
	int debug = 0;

	if(argc > 1) {
		list_size = atoi(argv[1]);
		assert (list_size > 0);
	} else
		list_size = LSIZE;

	if(argc > 2)
		debug = !strcmp(argv[2],"-d");

	// aloca memória para vetor
	list = (float *)malloc( list_size * sizeof(float));
	assert (list);

	// inicia valores do vetor
	init_vet(list, list_size);

	if(debug)
		print_array(list, list_size);

	// Ordena a lista com quicksort
	// void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
	// void qsort_r(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *, void *), void *arg);
	qsort((void *)list, list_size, sizeof(float), compare);

	// Ordena a lista com mergesort
	// mergesort() function is a modified merge sort with exponential search intended for sorting data with pre-existing order.
	// int mergesort(void *base, size_t nel, size_t width, int (*compare)(const void *, const void *));
	// mergesort((void*)list, list_size, sizeof(float), compare);

	if(debug)
		print_array(list, list_size);

	// libera área de memória do vetor
	free(list);

	return 0;
}


