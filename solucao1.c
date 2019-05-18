#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define NUM_PESSOAS 10

int comendo = 0;
int waiting = 0;
sem_t block;
pthread_mutex_t mutex;

pthread_t pessoas[NUM_PESSOAS];
    

void *sushi_bar(void *identifier){
    
    int i, n;

    while(TRUE){
        pthread_mutex_lock(&mutex);
        if (comendo==5){
            waiting+=1;
            printf("\t%d esperando!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, waiting);
            pthread_mutex_unlock(&mutex);
            sem_wait(&block);
        }
        else{
            comendo+=1;
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        printf("%d comendo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, waiting);
        pthread_mutex_unlock(&mutex);
        sleep(1);
        
        pthread_mutex_lock(&mutex);
        comendo-=1;
        printf("\t\t%d saindo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, waiting);
        
        if(comendo==0 && waiting){
            n = waiting < 5 ? waiting : 5;

            waiting-=n;
            comendo+=n;

            for(i=0;i<n;i++)
                sem_post(&block);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(){

    int i;
    int id_pessoas[NUM_PESSOAS];

    pthread_mutex_init(&mutex,0);
    sem_init(&block,0,0);

    for(i=0;i<NUM_PESSOAS;i++)
        id_pessoas[i] = i;

    for(i=0;i<NUM_PESSOAS;i++)
        pthread_create(&pessoas[i],0,sushi_bar,&id_pessoas[i]);

    while(TRUE);

}