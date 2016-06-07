/**
 *	Compilação:
 *		gcc main.c -o k-means -g -Wall -lm -fopenmp
 *	Execução:
 *		./k-means
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>

#define MAX 100
#define MIN 0
#define ITERACOES 10
#define QTD_PROCESSOS 2
#define TAM_PROBLEMA 18
#define QTD_REPETICOES 10

#define NUM_CENTROS 10
#define NUM_DIMENSOES 3


/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

typedef struct Elemento {
	double *atributo;
	unsigned long long int dimensao;
	unsigned long long int centro;
} Elemento;

void init_elemento(Elemento *elemento, unsigned long long int dimensao);
void destroy_elemento(Elemento *elemento);

void gerar_dados(Elemento *elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes, double max, double min);
void liberar_dados(Elemento *elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

double distancia(Elemento elemento, Elemento centro, unsigned long long int qtd_dimensoes);

void init_centros(Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dimensoes, double max, double min);

bool agrupar_dados(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);
void determinar_novos_centros(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

void k_means(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

int qtd_processos;

int main(int argc, char** argv) {
#	pragma omp parallel num_threads(32)
	{
		// printf("My rank %d\n", omp_get_thread_num());
	}
	srand(time(NULL));

	unsigned long long int qtd_dados, qtd_centros, qtd_dimensoes;
	qtd_centros = NUM_CENTROS;
	qtd_dimensoes = NUM_DIMENSOES;
	Elemento *elementos, *centros;

	elementos = (Elemento *) malloc(pow(2, TAM_PROBLEMA) * sizeof(Elemento));
	gerar_dados(elementos, pow(2, TAM_PROBLEMA), qtd_dimensoes, MAX, MIN);

	centros = (Elemento *) malloc(NUM_CENTROS * sizeof(Elemento));
	gerar_dados(centros, NUM_CENTROS, qtd_dimensoes, MAX, MIN);

	double media_tempo ,t_final, t_inicial;

	unsigned long long int i, j, k;
	for (i = 0; i < QTD_PROCESSOS; i++) {
		qtd_processos = pow(2, i);
		for (j = 5; j < TAM_PROBLEMA; j++) {
			qtd_dados = pow(2, j);
			media_tempo = 0;
			for (k = 0; k < QTD_REPETICOES; k++) {
				init_centros(centros, qtd_centros, qtd_dimensoes, MAX, MIN);
					GET_TIME(t_inicial);
					k_means(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes);
					GET_TIME(t_final);
				media_tempo += (t_final - t_inicial);
			}
			media_tempo /= QTD_REPETICOES;
			printf("%lf\t", media_tempo);
		}
		printf("\n");
	}
	// liberar_dados(elemsentos, qtd_dados, qtd_dimensoes);
	return 0;
}

/**
 *	Função para reaizar alocação de memória para estrutura elemento.
 */
void init_elemento(Elemento *elemento, unsigned long long int dimensao) {
	elemento->atributo = malloc(dimensao * sizeof(double));
	elemento->dimensao = dimensao;
	elemento->centro = 0;
}

/**
 *	Função para realizar desalocação de memória da estrutura elemento.
 */
void destroy_elemento(Elemento *elemento) {
	free(elemento->atributo);
	free(elemento);
}

/**
 *	Função para geração aleatoria e alocação de memória para conjunto de dados.
 */
void gerar_dados(Elemento *elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes, double max, double min) {
	unsigned long long int i, j;
	for (i = 0; i < qtd_dados; i++) {
		init_elemento(&elementos[i], qtd_dimensoes);
		for (j = 0; j < qtd_dimensoes; j++) {
			elementos[i].atributo[j] = (((double)rand()/(double)RAND_MAX) * (max - min)) + min;
		}
	}
}

/**
 *	Função para desalocação de memória para conjunto de dados.
 */
void liberar_dados(Elemento *elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	unsigned long long int i;
	for (i = 0; i < qtd_dados; i++) {
		free(elementos[i].atributo);
	}
	free(elementos);
}

/**
 *	Função para calculo de distancia entre dois elementos.
 */
double distancia(Elemento elemento, Elemento centro, unsigned long long int qtd_dimensoes) {
	unsigned long long int i;
	double distancia = 0;
	for (i = 0; i < qtd_dimensoes; i++) {
		distancia += pow(elemento.atributo[i] - centro.atributo[i], 2.0);
	}
	return sqrt(distancia);
}

/**
 *	Inicialização dos centro do K-means.
 */
void init_centros(Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dimensoes, double max, double min) {
	unsigned long long int i, j;
	for (i = 0; i < qtd_centros; i++) {
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] = (((double)rand()/(double)RAND_MAX) * (max - min)) + min;
		}
	}
}

/**
 *	Definição dos agrupamentos, associação de um elemento a um determinado grupo.
 */
bool agrupar_dados(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	unsigned long long int i, j;
	bool mudanca_centro = false;
#	pragma omp parallel for num_threads(qtd_processos) \
		private (j)
	for (i = 0; i < qtd_dados; i++) {
		double distancia_centro = distancia(elementos[i], centros[elementos[i].centro], qtd_dimensoes);
		for (j = 0; j < qtd_centros; j++) {
			double teste_distancia_centro = distancia(elementos[i], centros[j], qtd_dimensoes);
			if (distancia_centro > teste_distancia_centro) {
				mudanca_centro = true;
				distancia_centro = teste_distancia_centro;
				elementos[i].centro = j;
			}
		}
	}
	return mudanca_centro;
}

/**
 *	Calcular centro associado a todos os grupos.
 */
void determinar_novos_centros(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	unsigned long long int i, j;
	for (i = 0; i < qtd_centros; i++) {
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] = 0;
		}
	}

	int *dados_por_centro;
	dados_por_centro = (int *) malloc(qtd_centros * sizeof(int));

#	pragma omp for \
		private (j)
	for (i = 0; i < qtd_dados; i++) {
		dados_por_centro[elementos[i].centro]++;
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[elementos[i].centro].atributo[j] += elementos[i].atributo[j];
		}
	}

	for (i = 0; i < qtd_centros; i++) {
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] /= dados_por_centro[i];
		}
	}

	// free((void *)dados_por_centro);
}

/**
 *	Implementação do algoritmo K-means.
 */
void k_means(Elemento *elementos, Elemento *centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	int i;
	for (i = 0; i < ITERACOES; i++){
		agrupar_dados(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes);
		// determinar_novos_centros(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes);
	}
}
