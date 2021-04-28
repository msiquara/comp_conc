#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<math.h>

#define NUM_RANDOM 10000 //maior numero da lista de aleatorios

int nthreads;

int *array;

typedef struct{
   int id; 
   double rq; //random_quant
   double ml; //media local
   int ig; // contador igual
} tArgs;

void *Calculo(void *arg){
	tArgs *args = (tArgs*) arg;
	double somaLocal = 0;
	//printf("id = %d\n", args->id);

	for (int i = args->id; i < args->rq; i += nthreads){
		somaLocal += (float)array[i];
		// checar se a soma de dois numeros consecutivos é igual ao numero seguinte ao par
		if (i < (args->rq - 2)){
			if ((array[i]+array[i+1]) == array[i+2]){
				args->ig += 1;
			}
		}
	}

	args->ml = somaLocal/args->rq; //calculo da media

	pthread_exit((void*) args);
}

int main(int argc, char* argv[]) {
	double random_quant,soma_medias = 0; // quantidade de numeros aleatorios gerados
   	double inicio, fim;
   	int soma_igual = 0;
   	pthread_t *tid; 
   	tArgs *args;
   	tArgs *retorno;
   	unsigned int seed = time(NULL);
   	
   	//leitura e avaliacao dos parametros de entrada
   	if(argc<3) {
    	printf("Digite: %s <quantidade de numeros aleatorios> <numero de threads>\n", argv[0]);
      	return 1;
   	}

   	random_quant = atoi(argv[1]);
   	nthreads = atoi(argv[2]);

   	tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
   	if(tid==NULL) {puts("ERRO--malloc"); return 2;}	

   	args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
   	if(args==NULL) {puts("ERRO--malloc"); return 2;}

   	array = (int*) malloc(sizeof(int) * random_quant);
	if (array == NULL) {printf("ERRO--malloc\n"); return 2;}

	printf("Criando array...\n");

   	GET_TIME(inicio);
   	// PREENCHENDO ARRAY COM NUMEROS ALEATORIOS
   	for (long int i = 0; i < random_quant; i++){
		array[i] = (int) rand_r(&seed)%NUM_RANDOM+1;
	}

	GET_TIME(fim);

	printf("%f\n\n", fim - inicio);

	// CONCORRENTE
   	GET_TIME(inicio);

	for(int i=0; i < nthreads; i++){
		(args+i)->id = i;
		(args+i)->rq = random_quant;
      	if(pthread_create(tid+i, NULL, Calculo, (void*) (args+i))){
        	puts("ERRO--pthread_create"); return 3;
      	}
   	} 

   	for(int i=0; i<nthreads; i++) {   		
   		if (pthread_join(*(tid+i), (void**) &retorno)){
   			fprintf(stderr, "ERRO--pthread_creat\n");
   			return 3;
      	}

      	soma_medias += retorno->ml;
      	soma_igual += retorno->ig;
  	}

   	GET_TIME(fim);

   	printf("===========================\nCONCORRENTE\nMedia: %.4f\n", soma_medias);
	printf("\narray[i] + array[i+1] = array[i+2]\n*Iguais: %d\n\n", soma_igual);
   	printf("Tempo de calculo: %f\n \n", fim - inicio);

   	// SEQUENCIAL
   	double soma = 0, media = 0;
   	int igual = 0;

	GET_TIME(inicio);
	
	for (long int i = 0; i < random_quant; i++){
		soma += array[i];
		// checar se dois numeros consecutivos são igual, maior ou menor que o seguinte ao par
		if (i < (random_quant - 2)){
			if ((array[i]+array[i+1]) == array[i+2]){
				igual += 1;
			}
		}
	}	

	media = soma/random_quant;

	GET_TIME(fim);

	printf("===========================\nSEQUENCIAL\nMedia: %.4f\n", media);
	printf("\narray[i] + array[i+1] = array[i+2]\n*Iguais: %d\n\n", igual);
	printf("Tempo de calculo: %f\n", fim - inicio);

	free(array);
	free(args);
   	free(tid);

   	return 0;
}
