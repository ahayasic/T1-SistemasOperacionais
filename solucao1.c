#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define NUM_PESSOAS 10

int comendo = 0;
int esperando = 0;
sem_t block;
pthread_mutex_t mutex;

pthread_t pessoas[NUM_PESSOAS];
    

void *sushi_bar(void *identifier)
{    
    int i, n;

    while(TRUE) {
        pthread_mutex_lock(&mutex);
        if(comendo == 5) {
            esperando++;
            printf("\t%d esperando!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, esperando);
            pthread_mutex_unlock(&mutex);
            sem_wait(&block);
        }
        else {
            comendo++;
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        printf("%d comendo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, esperando);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5 + 2);
        
        pthread_mutex_lock(&mutex);
        comendo--;
        printf("\t\t%d saindo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identifier, comendo, esperando);
        
        if(comendo == 0 && esperando) {
            n = esperando < 5 ? esperando : 5;

            esperando -= n;
            comendo += n;

            for(i=0;i<n;i++)
                sem_post(&block);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main() 
{
    int i;
    int id_pessoas[NUM_PESSOAS];

    pthread_mutex_init(&mutex, 0);
    sem_init(&block, 0, 0);

    for(i=0;i<NUM_PESSOAS;i++)
        id_pessoas[i] = i;

    for(i=0;i<NUM_PESSOAS;i++)
        pthread_create(&pessoas[i], 0, sushi_bar, &id_pessoas[i]);

    while(TRUE);
}