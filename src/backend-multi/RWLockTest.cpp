#include "RWLock.h"
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <time.h>

struct timespec timR, timW;




int i = 0;
int readers = 0;
int writers = 0;

int valor = 0;
int veces = 0;
RWLock lock_valor;
unsigned int usecs = 1;


pthread_mutex_t mut;
pthread_cond_t barrera;
bool todos_creados = false;

void* lector(void * arg){
	pthread_detach(pthread_self());
	
	pthread_mutex_lock(&mut);
	while(!todos_creados){
		pthread_cond_wait(&barrera, &mut);
		}
	pthread_mutex_unlock(&mut);
	for(int i =0; i< 2; i++){
		lock_valor.rlock();	
		printf ("leo valor:%d\n", valor);
		lock_valor.runlock();
		nanosleep(&timR, NULL);
	}
	pthread_exit(NULL);
	return NULL;
}

void* escritor(void * arg){
	pthread_detach(pthread_self());	
	
	
	pthread_mutex_lock(&mut);
	while(!todos_creados){
		pthread_cond_wait(&barrera, &mut);
		}
	pthread_mutex_unlock(&mut);
	
	for(int i=0; i < 2; i++){
		lock_valor.wlock();
		printf("Cambio valor\n");
		valor++;
		lock_valor.wunlock();
		nanosleep(&timW, NULL);	
	}
	
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char* argv[]) {

	pthread_mutex_init(&mut, NULL);
	pthread_cond_init(&barrera, NULL);
	
	timR.tv_sec = 0;
	timR.tv_nsec = 100;
	
	timW.tv_sec = 0;
	timW.tv_nsec = 10;

    /* Implementar */
    if (argc < 3) {
        fprintf(stderr, "Faltan argumentos, la forma de uso es: %s R W ------ \n R = cantidad de readers, W = cantidad de writers\n", argv[0]);
        return 3;
	}

	readers = atoi(argv[1]);
	writers = atoi(argv[2]);

	for (i = 0; i< readers; i++){
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, lector, NULL);
		
	}
	for (i = 0; i< writers; i++){
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, escritor, NULL);
	}
	
	printf("ya estan todos creados\n\n");

	
	sleep(2);
	pthread_mutex_lock(&mut);
	todos_creados = true;
	pthread_mutex_unlock(&mut);
	//DESPERTAMOS TODOS LOS THREADS EN BARRERA
	pthread_cond_broadcast(&barrera);
	for(int i = 0; i < 10; i++){
		sleep(1);
		}
	return 0;
}
