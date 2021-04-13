#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat; 
float *mat2; 
float *mat_saida; 
int nthreads; 

typedef struct{
   int id; 
   int dim;
} tArgs;

//funcao que as threads executarao
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
   for (int i = args->id; i < args->dim; i += nthreads){
      for (int j = 0; j < args->dim; j += 1){
         for (int k = 0; k < args->dim; k += 1){
            mat_saida[i*args->dim + j] += mat[i*args->dim + k] * mat2[k*args->dim + j];

         }
      }
   }

   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   int dim; 
   pthread_t *tid; 
   tArgs *args; 
   double inicio, fim, delta_a, delta_b, delta_c, delta_total;
   
   GET_TIME(inicio);

   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }

   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim){ 
      nthreads=dim;
   }

   //alocacao de memoria para as estruturas de dados
   mat = (float *) malloc(sizeof(float) * dim * dim);
   if (mat == NULL) {printf("ERRO--malloc\n"); return 2;}

   mat2 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}

   mat_saida = (float *) malloc(sizeof(float) * dim * dim);
   if (mat_saida == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat[i*dim + j] = 2;
         mat2[i*dim + j] = 2;
         mat_saida[i*dim + j] = 0;
      }
   }

   GET_TIME(fim);
   delta_a = fim - inicio;
   printf("Tempo inicializacao:%lf\n", delta_a);

   GET_TIME(inicio);

   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}

   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 

   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }

   GET_TIME(fim)   
   delta_b = fim - inicio;
   printf("Tempo multiplicacao:%lf\n", delta_b);
   /*
   //exibicao dos resultados
   puts("Matriz de saida:");
   for(int i=0; i<dim; i++){
      for (int j = 0; j < dim; j++){
         printf("%.1f ", mat_saida[i*dim + j]);
      }      
      puts("");
   }
   */
   GET_TIME(inicio);
   free(mat);
   free(mat2);
   free(mat_saida);
   free(args);
   free(tid);
   GET_TIME(fim)   
   delta_c = fim - inicio;
   printf("Tempo finalizacao:%lf\n", delta_c);

   delta_total = delta_a + delta_b + delta_c;
   printf("Tempo total: %lf\n", delta_total);
   return 0;
}
