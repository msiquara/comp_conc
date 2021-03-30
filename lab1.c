#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2
#define V_SIZE 10000

int vetor[V_SIZE] = {0};

void *incrementar(void *arg){
	int idThread = * (int*) arg;

	if (idThread == 0){
		for (int i = 0; i < V_SIZE/2; i++){
			vetor[i]+=1;
		}
	}

	if (idThread == 1){
		for (int j = (V_SIZE/2); j < V_SIZE; j++){
			vetor[j]+=1;
		}
	}
	
	pthread_exit(NULL);
}

int main(){
	pthread_t tid_sistema[NTHREADS];
	int thread;
	int tid_local[NTHREADS];
	
	for (thread = 0; thread < NTHREADS; thread++){		
		tid_local[thread] = thread;

		if (pthread_create(&tid_sistema[thread], NULL, incrementar, (void*) &tid_local[thread])) {
      		printf("--ERRO: pthread_create()\n"); 
      		exit(-1);
    	}
	}

	for (int thread=0; thread < NTHREADS; thread++){
    	if (pthread_join(tid_sistema[thread], NULL)){
        	printf("--ERRO: pthread_join() \n"); exit(-1); 
    	} 
  	}

  	int num = 0;
  	while(vetor[num]){
  		num++;  		
  	}

  	if (num == V_SIZE){
  		printf("Todos os valores foram incrementados corretamente!\n");
  	}

  	else{
  		printf("ERRO em %d\n",num);
  	}

  	
  	pthread_exit(NULL);
}
