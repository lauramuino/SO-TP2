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
    hay_escribiendo = false;
    hay_escritor_esperando = false;
    lectores_esperando = false;
    
	pthread_mutex_init(&mtx_RWL, NULL);
	pthread_cond_init(&barrera_lectores, NULL);		
	pthread_cond_init(&primera_barrera_lectores, NULL);		
	pthread_cond_init(&sem_escritores, NULL);

    
}

void RWLock :: rlock() {
	
     pthread_mutex_lock(&mtx_RWL);		//lock de mutex
     
     //PRIMERO ME FIJO SI YA HABIA ALGUIEN ESPERANDO PARA HACER WRITE
     while(hay_escritor_esperando){
		 pthread_cond_wait(&primera_barrera_lectores, &mtx_RWL);
		 }
     
     //AHORA ESPERO A QUE TERMINE DE HACER WRITE
     while(hay_escribiendo){
		 lectores_esperando = true;
		pthread_cond_wait(&barrera_lectores, &mtx_RWL);			//suelta el mutex mientras hace wait
		//hay algun thread que esta escribiendo espero
	}	
	
	//ACA PASARON TODOS LOS LECTORES HASTA EL ULTIMO WRITE;
	cant_lectores++;						//aumento la cantidad de lectores
	pthread_mutex_unlock(&mtx_RWL);			//libero mtx
	
}

void RWLock :: wlock() {
    
    pthread_mutex_lock(&mtx_RWL);
   
    
    //SI NO HAY NADIE LEYENDO Y NADIE ESCRIBIENDO PUEDO ESCRBIR YO
    while(cant_lectores > 0 || hay_escribiendo){
		hay_escritor_esperando = true;	//YO PASO A SER UN ESCRITOR ESPERANDO
		pthread_cond_wait(&sem_escritores, &mtx_RWL);
		//hay threads leyendo o escribiendo
		}
	
	//NO ESTOY ESPERANDO MAS. AHORA ESTOY ESCRIBIENDO
	hay_escritor_esperando = false;
    hay_escribiendo = true;	
    
    pthread_mutex_unlock(&mtx_RWL);	//libero mtx
    pthread_cond_broadcast(&primera_barrera_lectores);	//LE DIGO A LOS LECTORES EN LA PRIMERA BARRERA QUE PASEN
    
}

void RWLock :: runlock() {
	
	pthread_mutex_lock(&mtx_RWL);
	cant_lectores--;
	if(cant_lectores == 0){
		//si no hay mas nadie leyendo y hay escritores esperando escribir, despierto alguno de ellos
		lectores_esperando = false;
		pthread_cond_signal(&sem_escritores);    //LE DIGO AL SIGUIENTE ESCRITOR QUE PASE
		}
	
	pthread_mutex_unlock(&mtx_RWL);
}

void RWLock :: wunlock() {
	
	pthread_mutex_lock(&mtx_RWL);
	hay_escribiendo = false;
	pthread_mutex_unlock(&mtx_RWL);
	
 // BROADCAST A LOS LECTORES EN LA ULTIMA BARRERA.
 	pthread_cond_broadcast(&barrera_lectores);		
 // SI HAY ESCRITORES ESPERANDO Y NO HAY MAS LECTORES, LE MANDO SINGAL AL SEM PORQUE NO SE HACE DESDE RUNLOCK	
 	if(hay_escritor_esperando && !lectores_esperando) pthread_cond_signal(&sem_escritores);
	
	
}
