#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>


#define MaxItems 5 
#define BufferSize 5 

sem_t empty;
sem_t full;

int in = 0;
int out = 0;

pthread_mutex_t mutex;

typedef struct 
{
    int pid;
    int destination;
    int p[3];
} Clock;

Clock buffer[BufferSize];

void *producer(Clock c)
{   
    Clock item=c;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
}
void *consumer()
{   
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        Clock item = buffer[out];
        printf("Consumer : Remove Item %d,%d,%d from %d\n", item.p[0],item.p[1],item.p[2], out);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
}

void *criarthread (void* f){
    Clock c1;
    c1.p[0]=1;c1.p[1]=1;;c1.p[2]=1;
    producer(c1);
    c1.p[0]=2;c1.p[1]=2;;c1.p[2]=2;
    producer(c1);
    producer(c1);
    producer(c1);
    producer(c1);
    c1.p[0]=3;c1.p[1]=3;;c1.p[2]=3;
    producer(c1);
    return NULL;
}
 
void *removerthread (){
    consumer();
    consumer();
    consumer();
    consumer();
    consumer();
    consumer();
    return NULL;
}


int main()
{   

    pthread_t pro,con;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);
    
    
    pthread_create(&pro, NULL, (void *)criarthread, NULL);
    
    pthread_create(&con, NULL, (void *)removerthread, NULL);

    pthread_join(pro, NULL);
    pthread_join(con, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}
