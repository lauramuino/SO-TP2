#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {
      
    cant_lectores = 0;
    cant_escritores = 0;
    cant_escritores_esperando = 0;
    
	pthread_mutex_init(&(this->mtx_RWL), NULL);
	pthread_cond_init(&barrera_lectores, NULL);		
	pthread_cond_init(&barrera_escritores, NULL);

	
    //pthread_rwlock_init(&(this->rwlock),NULL);
    
    
}

void RWLock :: rlock() {
	
     pthread_mutex_lock(&mtx_RWL);		//lock de mutex
     while(cant_escritores > 0){		//si el write tiene prioridad chequeo tambien si cant_escritores_esperando > 0
		pthread_cond_wait(&barrera_lectores, &mtx_RWL);	//suelta el mutex mientras hace wait
		//hay algun thread que esta escribiendo espero
	}	
	//si llegue acá es porque no hay nadie escribiendo y me dieron signal desde wunlock;
	cant_lectores++;						//aumento la cantidad de lectores
	pthread_mutex_unlock(&mtx_RWL);			//libero mtx
	
	
    //pthread_rwlock_rdlock(&(this->rwlock));
}

void RWLock :: wlock() {
     
    pthread_mutex_lock(&mtx_RWL);
    cant_escritores_esperando++;
    
    while(cant_lectores > 0 || cant_escritores > 0){
		pthread_cond_wait(&barrera_escritores, &mtx_RWL);
		//hay threads leyendo o escribiendo
		}
	cant_escritores_esperando--;
    cant_escritores++;					//creo que siempre va a ser 1 o 0, los que estan esperando no suman
    pthread_mutex_unlock(&mtx_RWL);	//libero mtx
    
    
    //pthread_rwlock_wrlock(&(this->rwlock));
}

void RWLock :: runlock() {
	
	pthread_mutex_lock(&mtx_RWL);
	cant_lectores--;
	if(cant_lectores == 0 && cant_escritores_esperando > 0){
		//si no hay mas nadie leyendo y hay escritores esperando escribir, despierto alguno de ellos
		pthread_cond_signal(&barrera_escritores);
		}
		
	pthread_mutex_unlock(&mtx_RWL);
	
//    pthread_rwlock_unlock(&(this->rwlock));
}

void RWLock :: wunlock() {
	
	pthread_mutex_lock(&mtx_RWL);
	cant_escritores--;
	
	if(cant_escritores_esperando > 0){
		pthread_cond_signal(&barrera_escritores);
		}
	//si tienen proridad los escritores: else{broadcast lectores;mutex unlock;}
	
	pthread_cond_broadcast(&barrera_lectores);		//doy signal a todos los lectores que estaban esperando
	pthread_mutex_unlock(&mtx_RWL);
	
    //pthread_rwlock_unlock(&(this->rwlock));
}
