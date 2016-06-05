/**
 *	Argumentos:
 *		1 - Número de threads
 *		2 - Número de Centros
 *		3 - Número de dimensões
 *		4 - Número de elementos do conjunto de dados
 *	Compilação:
 *		gcc main.c -o k-means -g -Wall -lm !omp!
 *	Execução:
 *		./k-means <num_threads> <num_centros> <num_dim> <num_elem>
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <time.h>

#define NUM_ARG 5
#define MAX 100
#define MIN 0
#define ITERACOES 100

typedef struct Elemento {
	double* atributo;
	unsigned long long int dimensao;
	unsigned long long int centro;
} Elemento;

void init_elemento(Elemento* elemento, int dimensao);
void destroy_elemento(Elemento* elemento);

void modo_de_uso(int causa);

void gerar_dados(Elemento* elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes, double max, double min);
void liberar_dados(Elemento* elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

double distancia(Elemento elemento, Elemento centro, unsigned long long int qtd_dimensoes);

void init_centros(Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dimensoes, double max, double min);

bool agrupar_dados(Elemento* elementos, Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);
void determinar_novos_centros(Elemento* elementos, Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

void k_means(Elemento* elementos, Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes);

int main(int argc, char** argv) {
	printf("%d\n", RAND_MAX);
	if (argc < NUM_ARG) {
		modo_de_uso(1);
	}

	srand(time(NULL));

	// Interpretar argumentos de entrada
	Elemento *elementos, *centros;
	unsigned long long int qtd_dados, qtd_centros, qtd_dimensoes;

	// gerar dados
	gerar_dados(elementos, qtd_dados, qtd_dimensoes, MAX, MIN);
	// iniciar loop de teste

	init_centros(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes, MAX, MIN);
		// iniciar temporizador

		// calcular k-means
		k_means(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes);

		// finalizar temporizador

		// salvar tempo de execução

	// finalizar loop de teste

	// liberar memoria
	liberar_dados(elementos, qtd_dados, qtd_dimensoes);
}

/**
 *	Função para reaizar alocação de memória para estrutura elemento.
 */
void init_elemento(Elemento* elemento, int dimensao) {
	elemento = malloc(sizeof(Elemento));
	elemento->atributo = malloc(dimensao * sizeof(double));
	elemento->centro = -1;
}

/**
 *	Função para realizar desalocação de memória da estrutura elemento.
 */
void destroy_elemento(Elemento* elemento) {
	free(elemento->atributo);
	free(elemento);
}

/**
 *	Função para detalhar motivo de falha e instruir correto funcionamento.
 */
void modo_de_uso(int causa) {
    switch (causa) {
        case 1:
            printf("Número de argumentos inválido, insira %d\n", NUM_ARG);
    }
    exit(causa);
}

/**
 *	Função para geração aleatoria e alocação de memória para conjunto de dados.
 */
void gerar_dados(Elemento* elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes, double max, double min) {
	elementos = malloc(qtd_dados * sizeof(Elemento));
	unsigned long long int i;
	for (i = 0; i < qtd_dados; i++) {
		init_elemento(&elementos[i], qtd_dimensoes);
		int j;
		for (j = 0; j < qtd_dimensoes; j++) {
			elementos[i].atributo[j] = (((double)rand()/(double)RAND_MAX) * (max - min)) + min;
		}
	}
}

/**
 *	Função para desalocação de memória para conjunto de dados.
 */
void liberar_dados(Elemento* elementos, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
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
void init_centros(Elemento *centros, unsigned long long int qtd_centros unsigned, long long int qtd_dimensoes, double max, double min) {
	Elemento *centros;
	centros = malloc(qtd_centros * sizeof(Elemento));
	unsigned long long int i;
	for (i = 0; i < qtd_centros; i++) {
		init_elemento(&centros[i], qtd_dimensoes);
		int j;
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] = (((double)rand()/(double)RAND_MAX) * (max - min)) + min;
		}
	}
}

/**
 *	Definição dos agrupamentos, associação de um elemento a um determinado grupo.
 */
bool agrupar_dados(Elemento* elementos, Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	unsigned long long int i;
	bool mudanca_centro = false;
	for (i = 0; i < qtd_dados; i++) {
		double distancia_centro = distancia(elementos[i], centros[elementos[i].centro], qtd_dimensoes);
		unsigned long long int j;
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
	for (i = 0; i < qtd_centros; i++) {
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] = 0;
		}
	}

	int *dados_por_centro;
	dados_por_centro = malloc(qtd_centros * sizeof(int));
	unsigned long long int i;
	for (i = 0; i < qtd_dados; i++) {
		unsigned long long int j;
		dados_por_centro[i]++;
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[elementos[i].centro].atributo[j] += elementos[i].atributo[j];
		}
	}

	for (i = 0; i < qtd_centros; i++) {
		for (j = 0; j < qtd_dimensoes; j++) {
			centros[i].atributo[j] /= dados_por_centro[i];
		}
	}
}

/**
 *	Implementação do algoritmo K-means.
 */
void k_means(Elemento* elementos, Elemento* centros, unsigned long long int qtd_centros, unsigned long long int qtd_dados, unsigned long long int qtd_dimensoes) {
	int i;
	for (i = 0; i < ITERACOES; i++){
		agrupar_dados(elementos, centros, qtd_centros, qtd_dados, qtd_dimensoes);
		determinar_novos_centros( elementos,  centros, qtd_centros, qtd_dados, qtd_dimensoes);
	}
}
