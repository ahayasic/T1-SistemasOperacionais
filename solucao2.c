#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define NUM_PESSOAS 10

int comendo, esperando, deve_esperar = 0;
sem_t block;
pthread_mutex_t mutex;

void *sushi_bar(void *identificador)
{
    int i, n;

    while(TRUE) {
        pthread_mutex_lock(&mutex);
        if(deve_esperar) {
            esperando++;
            printf("\t%d esperando!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identificador, comendo, esperando);
            pthread_mutex_unlock(&mutex);
            sem_wait(&block);
            esperando--;
        }

        comendo++;
        deve_esperar = (comendo == 5);
        
        if(esperando && !(deve_esperar))
            sem_post(&block);
        else
            pthread_mutex_unlock(&mutex);

        printf("%d comendo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identificador, comendo, esperando);
        sleep(rand() % 5 + 2);
        
        pthread_mutex_lock(&mutex);
        comendo--;
        printf("\t\t%d saindo!\t#Comendo: %d\t$Esperando: %d\n", *(int *) identificador, comendo, esperando);
        if(comendo == 0)
            deve_esperar = 0;

        if(esperando && !(deve_esperar))
            sem_post(&block);
        else
            pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void main()
{
    int i;
    int id_pessoas[NUM_PESSOAS];

    pthread_t pessoas[NUM_PESSOAS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&block, 0, 0);

    for(i=0;i<NUM_PESSOAS;i++)
        id_pessoas[i] = i;

    printf("##### ESTADO INICIAL #####\n%d pessoas,\t%d comendo,\t%d esperando.\n", NUM_PESSOAS, comendo, esperando);

    for(i=0;i<NUM_PESSOAS;i++)
        pthread_create(&pessoas[i], 0, sushi_bar, &id_pessoas[i]);

    while(TRUE);
}