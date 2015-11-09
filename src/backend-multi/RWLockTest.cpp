#include "RWLock.h"
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <time.h>

struct timespec timR, timW;

int readers = 0;
int writers = 0;

int valor = 0;
RWLock lock_valor;

pthread_mutex_t mut;
pthread_cond_t barrera;
bool todos_creados = false;

void* lector(void * arg){
	//++++++++++++++ACA CREAMOS UNA BARRERA PARA QUE TODOS LOS THREADS ESPEREN A QUE SEAN TODOS CREADOS
	pthread_mutex_lock(&mut);
	while(!todos_creados){
		pthread_cond_wait(&barrera, &mut);
		}

	pthread_mutex_unlock(&mut);
	//++++++++++++++++++++++++++++
	
	int yo = (int)(long)arg;
	lock_valor.rlock();
	printf ("mi tid---->%d leo valor:%d\n", yo, valor );
	lock_valor.runlock();
	
		
	pthread_exit(NULL);
	return NULL;
}

void* escritor(void * arg){
	//++++++++++++++++++++++++++++BARRERA
	pthread_mutex_lock(&mut);
	while(!todos_creados){
		pthread_cond_wait(&barrera, &mut);
		}
	pthread_mutex_unlock(&mut);
	//++++++++++++++++++++++++++++
	lock_valor.wlock();
	
	int yo = (int)(long)arg;
	printf("mi tid---->%d Cambio valor\n", yo);
	valor++;
	
	
	nanosleep(&timW, NULL);
	lock_valor.wunlock();
			
	
	
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char* argv[]) {
//INICIALIZAMOS VC Y MUTEX
	pthread_mutex_init(&mut, NULL);
	pthread_cond_init(&barrera, NULL);
	
	timR.tv_sec = 0;
	timR.tv_nsec = 500;
	//ES PARA USA EL NANOSLEEP
	timW.tv_sec = 0;
	timW.tv_nsec = 1;

    /* Implementar */
    if (argc < 3) {
        fprintf(stderr, "Faltan argumentos, la forma de uso es: %s R W ------ \n R = cantidad de readers, W = cantidad de writers\n", argv[0]);
        return 3;
	}
	
	//BAJO ARGUMENTOS	
	readers = atoi(argv[1]);
	writers = atoi(argv[2]);
	
	
	int cant_threads = writers + readers;
	pthread_t  thread[cant_threads];		//ACA GUARDAMOS LOS THREAD ID

//SE CREAN TODOS LOS LECTORES Y ESCRITORES
	for(long i = 0; i < (long)cant_threads;  i++){
		if( i <(long) writers){ 
			pthread_create(&thread[i], NULL, escritor, (void *)i);
		}else{
			pthread_create(&thread[i], NULL, lector, (void *)i);
		}
	}
	
	printf("ya estan todos creados\n\n");

//ESPERO UN CACHITO
	//~ sleep(2);
	pthread_mutex_lock(&mut);
	todos_creados = true;		//CAMBIA LA CONDICION 
	pthread_mutex_unlock(&mut);
	
	
	
	//DESPERTAMOS TODOS LOS THREADS EN BARRERA
	pthread_cond_broadcast(&barrera);
	for(int i = 0; i < cant_threads; i++){
		pthread_join(thread[i], NULL);			//CON ESTO HACEMOS QUE EL THREAD "PRINCIPAL" ESPERE A QUE TERMINEN TODOS LOS THREADS
		}
	return 0;
}
