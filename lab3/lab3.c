//Soma todos os elementos de um vetor de inteiro
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include <math.h>

long long int dim; 
int nthreads; 

typedef struct {
    int id;
    double sl;//soma local
} tArgs;

void * tarefa(void * arg) {
   tArgs *args = (tArgs*) arg;
   args->sl = 0;
   double somaLocal = 0;
   long long int tamBloco = dim/nthreads;
   long long int ini = args->id * tamBloco + 1;
   long long int fim; 

   if(args->id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   
   if (nthreads%2 == 1 && args->id%2 == 1){      
      for (double i = ini; i < fim; i += 4){
         somaLocal -= (1/i);
      }
      for (double j = ini+2; j < fim; j += 4){
         somaLocal += (1/j);
      }
   }

   else{
      for (double i = ini; i < fim; i += 4){
         somaLocal += (1/i);
      }
      for (double j = ini+2; j < fim; j += 4){
         somaLocal -= (1/j);
      }
   }
   
   args->sl = somaLocal;
   pthread_exit((void*) args); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double ini, fim; //tomada de tempo
   double compara = 0;
   pthread_t *tid;
   double pi_conc = 4;
   double pi_seq = 4;
   double nElementos_conc = 0;
   double nElementos_seq = 0;
   tArgs *retorno;
   tArgs *arg;
  
   retorno = (tArgs*) malloc(sizeof(tArgs)*sizeof(double));
   if(retorno==NULL) {puts("ERRO--malloc"); return 2;}

   arg = (tArgs*) malloc(sizeof(tArgs)*sizeof(double));
   if(arg==NULL) {puts("ERRO--malloc"); return 2;}

   if(argc < 3) {
      fprintf(stderr, "Digite: %s <N> <numero threads>\n", argv[0]);
      return 1;
   }

   dim = atoi(argv[1]);
   dim = dim*2;
   nthreads = atoi(argv[2]);

   GET_TIME(ini);

   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

   for(long int i=0; i<nthreads; i++) {
      (arg+i)->id = i;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (arg+i))){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }

   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //soma global
      nElementos_conc += retorno->sl;
   }

   pi_conc = pi_conc * nElementos_conc;

   GET_TIME(fim);

   printf("CONCORRENTE:\nmeu PI:    %.15lf\nM_PI:      %.15lf\n", pi_conc, M_PI);
   compara = M_PI - pi_conc;
   printf("Diferença: %.15lf\n", compara);
   printf("Tempo concorrente: %lf\n", fim-ini);

   // SEQUENCIAL
   GET_TIME(ini);

   for (double i = 1; i < dim; i += 4){
      nElementos_seq += (1/i);   
   }

   for (double j = 3; j < dim; j += 4){
       nElementos_seq -= (1/j);  
   }
   
   if (nElementos_seq < 0){
      nElementos_seq = nElementos_seq*(-1);
   }
   
   GET_TIME(fim);

   pi_seq = pi_seq * nElementos_seq;
   printf("\nSEQUENCIAL:\nmeu PI:    %.15lf\nM_PI:      %.15lf\n", pi_seq, M_PI);
   compara = M_PI - pi_seq;
   printf("Diferença: %.15lf\n", compara);
   printf("Tempo sequencial: %lf\n", fim-ini);

   free(arg);
   free(tid);

   return 0;
}
