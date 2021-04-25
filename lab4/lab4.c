#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

int x, y = 0; //x gerencia a ordem das threads, y identifica a necessidade de desbloquear T1

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void *T1(void *t){	
	pthread_mutex_lock(&x_mutex);
	//printf("iniciou thread 1\n");

	while (x < 3){
		//printf("thread 1 bloqueada\n");		
		y++;
		pthread_cond_wait(&x_cond, &x_mutex);
		pthread_mutex_unlock(&x_mutex);
		//printf("thread 1 desbloqueada\n");
	}

	printf("Volte sempre!\n");

	pthread_exit(NULL);
}

void *T2(void *t){
	pthread_mutex_lock(&x_mutex);
	//printf("iniciou thread 2\n");

	if (x == 0){
		//printf("thread 2 bloqueada\n");
		pthread_cond_wait(&x_cond, &x_mutex);
		//printf("thread 2 desbloqueada\n");
	}

	x++;

	if ((y > 0) && (x == 3)){ //dentro da seçao critica pois estamos trabalhando com variaveis globais
		pthread_cond_signal(&x_cond);
	}

	printf("Fique a vontade.\n");
	
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

void *T3(void *t){
	pthread_mutex_lock(&x_mutex);
	//printf("iniciou thread 3\n");

	if (x == 0){
		//printf("thread 3 bloqueada\n");
		pthread_cond_wait(&x_cond, &x_mutex);
		//printf("thread 3 desbloqueada\n");
	}

	x++;
	
	if ((y > 0) && (x == 3)){ //dentro da seçao critica pois estamos trabalhando com variaveis globais
		pthread_cond_signal(&x_cond);
	}

	printf("Sente-se por favor.\n");

	pthread_mutex_unlock(&x_mutex);
	
	pthread_exit(NULL);
}

void *T4(void *t){	
	pthread_mutex_lock(&x_mutex);
	//printf("iniciou thread 4\n");

	printf("Seja bem-vindo!\n");
	x++;

	pthread_cond_broadcast(&x_cond);
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	int i;
	pthread_t threads[NTHREADS];

	pthread_mutex_init(&x_mutex, NULL);
  	pthread_cond_init (&x_cond, NULL);

	pthread_create(&threads[3], NULL, T4, NULL);
	pthread_create(&threads[2], NULL, T3, NULL);
  	pthread_create(&threads[1], NULL, T2, NULL);
  	pthread_create(&threads[0], NULL, T1, NULL);

	for (i = 0; i < NTHREADS; i++) {
	    pthread_join(threads[i], NULL);
	}


	pthread_mutex_destroy(&x_mutex);
  	pthread_cond_destroy(&x_cond);
}
