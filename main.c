/**
 *	Argumentos:
 *		1 - Número de threads
 *		2 - Número de Centros
 *		3 - Número de dimensões
 *		4 - Número de elementos do conjunto de dados
 */

#include <stdio.h>
#include <math.h>

#define NUM_ARG 5
#define MAX 100
#define MIN 0

typedef struct Elemento {
	double* atributo;
	int dimensao;
} Elemento;

void init_elemento(Elemento* elemento, int dimensao);
void destroy_elemento(Elemento* elemento);

void modo_de_uso(int causa);

void gerar_dados(Elemento* elementos, unsigned long long qtd_dados, unsigned long long qtd_dimensoes, double max, double min);
void liberar_dados(Elemento* elementos, unsigned long long qtd_dados, unsigned long long qtd_dimensoes);

double distancia(Elemento elemento, Elemento centro, unsigned long long qtd_dimensoes);

void k_means(Elemento* elementos, Elemento* centros, unsigned long long qtd_centros, unsigned long long qtd_dados, unsigned long long qtd_dimensoes);

int main(int argc, char** argv) {
	if (argc < NUM_ARG) {
		modo_de_uso(1);
	}
	// Interpretar argumentos de entrada


	// gerar dados
	gerar_dados(elementos, qtd_dados, qtd_dimensoes, MAX, MIN);
	// iniciar loop de teste

		// iniciar temporizador

		// calcular k-means

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
void gerar_dados(Elemento* elementos, unsigned long long qtd_dados, unsigned long long qtd_dimensoes, double max, double min) {
	elementos = malloc(qtd_dados * sizeof(Elemento));
	unsigned long long i;
	for (i = 0; i < qtd_dados; i++) {
		init_elemento(&elementos[i], qtd_dimensoes);
		int j;
		for (j = 0; j < qtd_dimensoes; j++) {
			elementos[i].atributo[j] = dado_aleatorio;
		}
	}
}

/**
 *	Função para desalocação de memória para conjunto de dados.
 */
void liberar_dados(Elemento* elementos, unsigned long long qtd_dados, unsigned long long qtd_dimensoes) {
	unsigned long long i;
	for (i = 0; i < qtd_dados; i++) {
		free(elementos[i].atributo);
	}
	free(elementos);
}

/**
 *	Função para calculo de distancia entre dois elementos.
 */
double distancia(Elemento elemento, Elemento centro, unsigned long long qtd_dimensoes) {
	unsigned long long i;
	double distancia = 0;
	for (size_t i = 0; i < qtd_dimensoes; i++) {
		distancia += pow(elemento.atributo[i] - centro.atributo[i], 2.0);
	}

	return sqrt(distancia);
}

/**
 *	Implementação do algoritmo K-means.
 */
void k_means(Elemento* elementos, Elemento* centros, unsigned long long qtd_centros, unsigned long long qtd_dados, unsigned long long qtd_dimensoes) {

}
