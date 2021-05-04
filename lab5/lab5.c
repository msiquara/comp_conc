#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int salto = 1;
int cont = 0; //contador de chamadas de barreira()
int bloqueadas = 0;
int *vet;
int NTHREADS;

void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica

    if (bloqueadas == (nthreads-1)) { 
      	cont ++;
      	
      	if (cont%2 == 0){ //altera o salto a cada duas chamadas de barreira()
      	   	salto *= 2;
      	}

      	pthread_cond_broadcast(&x_cond);
     	bloqueadas=0;
    } 

    else {
      	bloqueadas++;
      	pthread_cond_wait(&x_cond, &x_mutex);
    }

    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

void *tarefa (void *arg) {
	int id = *(int*)arg;
	int aux;

	for (int i = 0; i < (NTHREADS - 1); i++){
		if ((id - salto) >= 0){
			aux = vet[id-salto];
		}	

		else{
			aux = 0;
		}

		//printf("id: %d aux: %d\n", id, aux);

		barreira(NTHREADS);

		vet[id] += aux;

		barreira(NTHREADS);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	if(argc<2) {
      	printf("Digite: %s <potencia de 2>\n", argv[0]);
      	return 1;
    }

    NTHREADS = atoi(argv[1]);

    vet = (int *) malloc(sizeof(int) * NTHREADS);
   	if (vet == NULL) {printf("ERRO--malloc\n"); return 2;}

   	for (int i = 0; i < NTHREADS; i++){
   		vet[i] = pow(2, i);
   	}

   	for (int i = 0; i < NTHREADS; i++){
		printf("%d ", vet[i]);
	}

	printf("\n");

	pthread_t threads[NTHREADS];
	int id[NTHREADS];

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);

	/* Cria as threads */
	for(int i=0;i<NTHREADS;i++) {
		id[i]=i;
		pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
	}

	/* Espera todas as threads completarem */
	for (int i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < NTHREADS; i++){
		printf("%d ", vet[i]);
	}
	
	printf("\n");

	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x_cond);
	return 0;
}
