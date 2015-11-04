#include "RWLock.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
//#include <iostream>

int i = 0;
int readers = 0;
int writers = 0;

int valor = 0;
RWLock lock_valor;

pthread_mutex_t mut;
pthread_cond_t barrera;
bool todos_creados = false;

void* lector(void * arg){
	int j=0;
	int time= rand();

	pthread_mutex_lock(&mut);
	while (!todos_creados){
		pthread_cond_wait(&barrera, &mut);
	}
	pthread_mutex_unlock(&mut);
	sleep(time % 10);

	lock_valor.rlock();
	printf("Tomando Read Lock %u\n", (unsigned int) pthread_self());
	for (j = 0; j< 20; j++){
		printf ("%d", valor);
	}
	printf("\n");
	printf("Liberando Read Lock %u\n", (unsigned int) pthread_self());
	lock_valor.runlock();

	return NULL;
}

void* escritor(void * arg){
	int time = rand();

	printf("Tomando Write Lock, %u\n", (unsigned int) pthread_self());
	pthread_mutex_lock(&mut);
	while (!todos_creados){
		pthread_cond_wait(&barrera, &mut);
	}
	printf("Liberando Write Lock %u\n", (unsigned int) pthread_self());
	pthread_mutex_unlock(&mut);
	sleep(time % 10);

	lock_valor.rlock();
	lock_valor.wlock();
	printf("Cambiando valor\n");
	valor++;
	lock_valor.wunlock();

	return NULL;
}

int main(int argc, char* argv[]) {

	pthread_mutex_init(&mut, NULL);
	pthread_cond_init(&barrera, NULL);


    /* Implementar */
    if (argc < 3) {
        fprintf(stderr, "Faltan argumentos, la forma de uso es: %s R W - R = cantidad de readers, W = cantidad de writers\n", argv[0]);
        return 3;
	}

	readers = atoi(argv[1]);
	writers = atoi(argv[2]);

	for (i = 0; i<= readers; i++){
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, lector, NULL);
		
	}
	for (i = 0; i<= writers; i++){
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, escritor, NULL);
	}

	todos_creados = true;
	printf("Prueba secciones criticas\n\n");
	pthread_cond_broadcast(&barrera);

	return 0;
}
