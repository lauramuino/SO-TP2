#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {
    /* 
     * Crear estructuras necesarias, probablemente dos colas de espera
     * una para los lectores que esrten esperando si hay alguien que esta escribiendo
     * y una para los escritores que estan esperando para escribir
     * capaz se puede usar solo una hay que verr bien
     * tal vez contador de cantidad de lectores acutales
     * inicializar todas la variables de condicion y los mutex respectivos
     * 
     * 
     * */
    pthread_rwlock_init(&(this->rwlock),NULL);
    
    
}

void RWLock :: rlock() {
    /* si hay alguien escribiendo espero
     * sino me pongo a leer y sumo uno en lectores actuales?
     * tomo el mutex y cambio el estado de la var de cond
     *  */
    pthread_rwlock_rdlock(&(this->rwlock));
}

void RWLock :: wlock() {
    /* si hay alquien escribiendo o quedan lectores activos espero en la cola?
     * sino tomo el mutex y me pongo a escribir
     *  */
    pthread_rwlock_wrlock(&(this->rwlock));
}

void RWLock :: runlock() {
    /* 
     * resta uno en la cantidad de lectores 
     * si es cero avisa con un signal o algo para que si hay escritores se pongan
     * a escribir
     * sino listo creo*/
    pthread_rwlock_unlock(&(this->rwlock));
}

void RWLock :: wunlock() {
    /* hace signal a los mutex y las vc que use o estaban bloqueadas mejor dicho */
    pthread_rwlock_unlock(&(this->rwlock));
}
