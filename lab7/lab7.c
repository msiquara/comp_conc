#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t condt1, condt2; 
int x = 0;

void *T1(void *t){	
	printf("T1 pausou\n");
	sem_wait(&condt2);
	
	printf("Volte sempre!\n");

	pthread_exit(NULL);
}

void *T2(void *t){
	sem_wait(&condt1);

	if (x == 0){
		x++;
		sem_post(&condt1);//T2 libera T3
	}

	else{
		sem_post(&condt2);//T2 libera T1
	}

	printf("Fique a vontade.\n");

	pthread_exit(NULL);
}

void *T3(void *t){
	sem_wait(&condt1);

	if (x == 0){
		x++;
		sem_post(&condt1);//T3 libera T2
	}

	else{
		sem_post(&condt2);//T3 libera T1
	}

	printf("Sente-se por favor.\n");
	
	pthread_exit(NULL);
}

void *T4(void *t){	
	printf("Seja bem-vindo!\n");
	sem_post(&condt1);

	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
  	sem_init(&condt1, 0, 0);
  	sem_init(&condt2, 0, 0);
	pthread_t threads[NTHREADS];

	if (pthread_create(&threads[3], NULL, T1, NULL)) {
	  	printf("--ERRO: pthread_create()\n"); exit(-1);
	}

	if (pthread_create(&threads[0], NULL, T3, NULL)) {
	  	printf("--ERRO: pthread_create()\n"); exit(-1);
	}

	if (pthread_create(&threads[1], NULL, T2, NULL)) {
	  	printf("--ERRO: pthread_create()\n"); exit(-1);
	}

	if (pthread_create(&threads[2], NULL, T4, NULL)) {
	  	printf("--ERRO: pthread_create()\n"); exit(-1);
	}

	for (int i = 0; i < NTHREADS; i++) {
	    pthread_join(threads[i], NULL);
	}

	pthread_exit(NULL);
}
