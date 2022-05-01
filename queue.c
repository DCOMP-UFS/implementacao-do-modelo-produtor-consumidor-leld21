#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>


sem_t seminserir;
sem_t semretirar;

#define MAX 2

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
    f->inicio = -1;
    f->fim = -1;
    f->tam = 0;
}


int tam(fila *f)
{
    return f->tam;
}



bool cheia(fila *f)
{
    if(tam(f) == MAX){
        printf("Fila cheia\n");
        return 1;
    }else{
        return 0;
    }
}

bool vazia (fila *f)
{
    if(tam(f) == 0){
        printf("Fila Vazia\n");
        return 1;
    }else{
        return 0;
    }
}



bool inserir(Clock clock, fila *f)
{
    

    if(cheia(f))
    {
        sem_wait(&semretirar);
    }
    if (vazia(f)){
        f->inicio = 0;
        f->fim = 0;
        f->clocks[0] = clock;
        f->tam=1;
        sem_post(&seminserir);
    }else{
        f->fim = (f->fim + 1);
        //% MAX;
        f->clocks[f->fim] = clock;
        f->tam++;
    }
                
    return true;
}

void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}


void retirar(fila *f)
{
    //Clock c;
    if(vazia(f))
    {
        sem_wait(&seminserir);
        
    }if(cheia(f))
        {
        //c = f->clocks[f->inicio];
        f->inicio = (f->inicio + 1) % MAX;
        f->tam--;
        printf("Process: %d, Clock: (%d, %d, %d)\n", 2, f->clocks[f->inicio].p[0], f->clocks[f->inicio].p[1], f->clocks[f->inicio].p[2]);
        sem_post(&semretirar);
        }else{
        printf("Process: %d, Clock: (%d, %d, %d)\n", 2, f->clocks[f->inicio].p[0], f->clocks[f->inicio].p[1], f->clocks[f->inicio].p[2]);
        //c = f->clocks[f->inicio];
        f->inicio = (f->inicio + 1) % MAX;
        f->tam--;
        }
    
    //return c;
}
void *criarthread (void* f){
    Clock c1;
    c1.p[0]=1;c1.p[1]=1;;c1.p[2]=1;
    inserir(c1,(fila*)f);
    inserir(c1,(fila*)f);
    return NULL;
}
 
void *removerthread (void* f){
    removerthread((fila*)f);
    return NULL;
}

int main(void)
{
    
  pthread_t t1, t2;
    
  sem_init(&seminserir, 0, 0);
  sem_init(&semretirar, 0, 0);
 
  fila *f=malloc(sizeof(fila));
  inicializar(f);
  pthread_create(&t1, NULL, criarthread, (void*) f);  
  pthread_create(&t2, NULL, removerthread, (void*) f);  
  
  pthread_join(t1, NULL); 
  pthread_join(t2, NULL); 

}