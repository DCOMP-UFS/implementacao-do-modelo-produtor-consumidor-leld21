#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>



pthread_mutex_t mutex;
pthread_cond_t filaCheia;
pthread_cond_t filaVazia;

//sem_t seminserir;
//sem_t semretirar;

#define MAX 50
#define TAMANHOFILA 3

typedef struct 
{
    //int pid;
    //int destination;
    int p[3];
} Clock;

typedef struct
{
   Clock clocks[MAX];
   int inicio;
   int fim;
   int tam;
} fila;

void inicializar(fila *f)
{
    f->inicio = 0;
    f->fim = 0;
    f->tam = 0;
}


int tam(fila *f)
{
    return f->tam;
}



bool cheia(fila *f)
{
    if(tam(f) == TAMANHOFILA){
        return 1;
    }else{
        return 0;
    }
}

bool vazia (fila *f)
{
    if(tam(f) == 0){
        return 1;
    }else{
        return 0;
    }
}

bool inserir(Clock clock, fila *f)
{
    
    pthread_mutex_lock(&mutex);
    if(cheia(f))
    {
        printf("Fila cheia\n");
        pthread_cond_wait(&filaCheia, &mutex);
        
    }
    f->fim = (f->fim + 1);
    f->clocks[f->fim] = clock;
    f->tam++;
    
    if (vazia(f)){
        f->clocks[0] = clock;
        f->tam=1;
        
        //sem_post(&seminserir);
    }else{
        f->fim = (f->fim + 1);
        f->clocks[f->fim] = clock;
        f->tam++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&filaVazia);
    }
     
    return true;
}

void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}

void retirar(fila *f)
{
    if(vazia(f))
    {
        printf("Fila Vazia\n");
        pthread_cond_wait(&filaVazia, &mutex);
        
    }
    if(cheia(f)){
        //printf("Process: %d, Clock: (%d, %d, %d)\n", 2, f->clocks[f->inicio].p[0], f->clocks[f->inicio].p[1], f->clocks[f->inicio].p[2]);
        f->inicio = (f->inicio + 1);
        f->tam--;
        //sem_post(&semretirar);
        
        }else{
        //printf("Process: %d, Clock: (%d, %d, %d)\n", 2, f->clocks[f->inicio].p[0], f->clocks[f->inicio].p[1], f->clocks[f->inicio].p[2]);
        f->inicio = (f->inicio + 1);
        f->tam--;
        pthread_cond_signal(&filaCheia);
        
    }
    
   
}


void mostrarFila(fila *f)
{
    int k = f->inicio;

    for (int i = 0; i < tam(f); i++)
    {
        printf("(%d, %d, %d) - ", f->clocks[k].p[0], f->clocks[k].p[1], f->clocks[k].p[2]);
        k = (k + 1) % MAX;
    }
}

void *criarthread (void* f){
    Clock c1;
    c1.p[0]=1;c1.p[1]=1;;c1.p[2]=1;
    inserir(c1,(fila*)f);
    c1.p[0]=2;c1.p[1]=2;;c1.p[2]=2;
    inserir(c1,(fila*)f);
    inserir(c1,(fila*)f);
    return NULL;
}
 
void *removerthread (void* f){
    retirar((fila*)f);
    retirar((fila*)f);
    retirar((fila*)f);
    retirar((fila*)f);
    return NULL;
}

int main(void)
{
    
  pthread_t t1;
  pthread_t t2;
  //sem_init(&seminserir, 0, 0);
  //sem_init(&semretirar, 0, 0);
  pthread_cond_init(&filaCheia, NULL);
  pthread_cond_init(&filaVazia, NULL);
  pthread_mutex_init(&mutex, NULL);
 
  fila *f=malloc(sizeof(fila));
  inicializar(f);
  
  pthread_create(&t1, NULL, criarthread, (void*) f);  
  pthread_create(&t2, NULL, removerthread, (void*) f);  
  
  pthread_join(t1, NULL); 
  pthread_join(t2, NULL);
  mostrarFila(f);
  return 0;

}